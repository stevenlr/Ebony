#include <iostream>
#include <memory>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <stb_image.h>

#include "graphics/TransformPipeline.h"
#include "graphics/opengl/opengl.h"
#include "utils/io.h"
#include "utils/perlin.h"
#include "utils/maths.h"

using namespace std;
using namespace ebony;

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_TIMER);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_Window *window = SDL_CreateWindow("Ebony", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
	SDL_GLContext glContext = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		return 1;
	}

	SDL_GL_MakeCurrent(window, glContext);
	SDL_GL_SetSwapInterval(1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glClearColor(0, 0, 0, 1);

	bool running = true;

	string source, error;
	shared_ptr<gl::Program> program(new gl::Program());

	readFile("assets/programs/default.xml", source);

	if (!gl::linkProgramFromXml(*program, source, error)) {
		cerr << error << endl;
	}

	glUseProgram(*program);

	shared_ptr<gl::Sampler> sampler(new gl::Sampler());
	shared_ptr<gl::Texture> texture(new gl::Texture());

	glSamplerParameteri(*sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(*sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glSamplerParameteri(*sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(*sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	if (!gl::loadTextureFromFile(*texture, "assets/textures/uvgrid1024-color.png", error)) {
		cerr << error << endl;
	}

	gl::generateMipmaps(*texture);

	TransformPipeline pipeline;

	pipeline.perspective(70, 1280, 720, 0.1f, 1000);

	GLint uniformTexture = glGetUniformLocation(*program, "uTexture");
	GLint uniformMvp = glGetUniformLocation(*program, "uMvp");

	glUniform1i(uniformTexture, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glBindSampler(0, *sampler);

	int width = 512;
	float *perlinData = new float[width * width];

	generatePerlinNoise(perlinData, width, 0, 0, 2, 8, 256, 1467, 0.5f);

	for (int y = 0; y < width; ++y) {
		for (int x = 0; x < width; ++x) {
			perlinData[y * width + x] += 0.5f;
			perlinData[y * width + x] *= 0.5f * (1 - cosf(2 * PI_F * x / (width - 1))) * 0.5f * (1 - cosf(2 * PI_F * y / (width - 1)));
		}
	}

	float *terrainData = new float[width * width * 3];
	float *terrainNormals = new float[width * width * 3];
	int nbIndex = (width - 1) * (width * 2 + 2);
	unsigned int *terrainIndex = new unsigned int[nbIndex];
	unsigned int *terrainIndexPtr = terrainIndex;

	for (int y = 0; y < width - 1; ++y) {
		unsigned baseIndex = y * width;
		unsigned baseIndex2 = (y + 1) * width;

		for (int x = 0; x < width; ++x) {
			*terrainIndexPtr++ = baseIndex2 + x;
			*terrainIndexPtr++ = baseIndex + x;
		}

		*terrainIndexPtr++ = baseIndex2 + width - 1;
		*terrainIndexPtr++ = baseIndex2;
	}

	float altitudeScale = width / 4;

	for (int y = 0; y < width; ++y) {
		for (int x = 0; x < width; ++x) {
			int index = y * width + x;

			terrainData[index * 3 + 0] = x - width / 2;
			terrainData[index * 3 + 1] = y - width / 2;
			terrainData[index * 3 + 2] = perlinData[index] * altitudeScale;

			if (y == 0 || x == 0 || y == width - 1 || x == width - 1) {
				terrainNormals[index * 3 + 0] = 0;
				terrainNormals[index * 3 + 1] = 0;
				terrainNormals[index * 3 + 2] = 1;
			} else {
				float dhdx = (perlinData[index + 1] - perlinData[index - 1]) / 2  * altitudeScale;
				float dhdy = (perlinData[index + width] - perlinData[index - width]) / 2  * altitudeScale;
				float nx = -dhdx / sqrtf(1.0f + dhdx * dhdx);
				float ny = -dhdy / sqrtf(1.0f + dhdy * dhdy);
				float nz = 1.0;
				float len = sqrtf(nx * nx + ny * ny + nz * nz);

				terrainNormals[index * 3 + 0] = nx / len;
				terrainNormals[index * 3 + 1] = ny / len;
				terrainNormals[index * 3 + 2] = nz / len;
			}
		}
	}

	shared_ptr<gl::VertexArray> vao(new gl::VertexArray());
	shared_ptr<gl::Buffer> posBuffer(new gl::Buffer());
	shared_ptr<gl::Buffer> normalBuffer(new gl::Buffer());
	shared_ptr<gl::Buffer> indexBuffer(new gl::Buffer());

	glBindVertexArray(*vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, *posBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * width * width * 3, terrainData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, *normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * width * width * 3, terrainNormals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * nbIndex, terrainIndex, GL_STATIC_DRAW);

	delete[] perlinData;
	delete[] terrainData;
	delete[] terrainNormals;
	delete[] terrainIndex;

	float t = 0;

	while (running) {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		t += 0.01f;

		pipeline.identity();

		pipeline.lookat(glm::vec3(cos(t) * 300, sin(t) * 300, 150), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(uniformMvp, 1, GL_FALSE, glm::value_ptr(pipeline.getMvp()));

		glDrawElements(GL_TRIANGLE_STRIP, nbIndex, GL_UNSIGNED_INT, nullptr);

		SDL_GL_SwapWindow(window);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
