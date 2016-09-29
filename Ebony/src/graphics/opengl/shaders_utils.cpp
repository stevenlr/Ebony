#include "stdafx.h"
#include "graphics/opengl/shaders_utils.h"

#include "utils/io.h"

using namespace tinyxml2;

namespace ebony { namespace gl {

bool compileShaderFromSource(const Shader &shader, const std::string &source, std::string *error)
{
	const GLchar *sourceString = &*source.cbegin();
	GLint size = source.size();
	GLint status;

	glShaderSource(shader, 1, &sourceString, &size);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE) {
		if (error) {
			GLint logLength;

			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
			error->resize(logLength);
			glGetShaderInfoLog(shader, logLength, nullptr, &*error->begin());
		}

		return false;
	}

	return true;
}

bool linkProgramFromXml(const Program &program, const std::string &xmlSource, std::string *error)
{
	const char *sourceString = &*xmlSource.cbegin();
	size_t size = xmlSource.size();
	XMLDocument document;
	std::vector<std::unique_ptr<gl::Shader>> shaders;
	std::string subError, subSource;

	document.Parse(sourceString, size);

	const XMLElement *root = document.RootElement();
	const XMLElement *element = root->FirstChildElement("shader");

	do {
		std::string shaderPath = "assets/shaders/";
		const char *typeString = element->Attribute("type");
		ShaderType type;

		if (!typeString) {
			continue;
		}

		shaderPath += element->GetText();
			
		switch (typeString[0]) {
			case 'v':
				type = VertexShader;
				shaderPath += ".vert";
				break;
			case 'f':
				type = FragmentShader;
				shaderPath += ".frag";
				break;
			case 'g':
				type = GeometryShader;
				shaderPath += ".geom";
				break;
			default:
				continue;
		}

		std::unique_ptr<gl::Shader> shader(new gl::Shader(type));

		subSource.clear();

		if (!readFile(shaderPath, &subSource)) {
			if (error) {
				std::stringstream sstr;

				sstr << "Error reading file " << shaderPath << std::endl;
				*error = sstr.str();
			}

			return false;
		}
			
		if (!compileShaderFromSource(*shader, subSource, &subError)) {
			if (error) {
				std::stringstream sstr;

				sstr << "Error compiling " << typeString << " shader " << element->GetText() << ": " << std::endl;
				sstr << subError << std::endl;
				*error = sstr.str();
			}

			return false;
		}

		glAttachShader(program, *shader);
		shaders.push_back(std::move(shader));
	} while (element = element->NextSiblingElement("shader"));

	element = root->FirstChildElement("attribute");

	do {
		const char *name = element->GetText();
		int index = element->IntAttribute("index");

		glBindAttribLocation(program, index, name);
	} while (element = element->NextSiblingElement("attribute"));

	element = root->FirstChildElement("fragdata");

	do {
		const char *name = element->GetText();
		int index = element->IntAttribute("index");

		glBindFragDataLocation(program, index, name);
	} while (element = element->NextSiblingElement("fragdata"));

	GLint status;

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	shaders.clear();

	if (status != GL_TRUE) {
		if (error) {
			GLint logLength;

			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
			error->resize(logLength);
			glGetProgramInfoLog(program, logLength, nullptr, &*error->begin());
		}

		return false;
	}

	return true;
}

}}
