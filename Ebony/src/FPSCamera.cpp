#include "FPSCamera.h"

#include "input/InputHandler.h"
#include "utils/assert.h"
#include "utils/maths.h"

using namespace glm;
using namespace std;

namespace ebony {

FPSCamera::FPSCamera()
{
}

void FPSCamera::update(float dt)
{
	input::InputHandler *input = input::InputHandler::getInstance();
	float mouseSpeed = 0.002f;
	float moveSpeed = 2.0f;
	int mouseMoveX, mouseMoveY;

	input->getMouseMotion(&mouseMoveX, &mouseMoveY);

	_hangle -= mouseMoveX * mouseSpeed;
	_vangle -= mouseMoveY * mouseSpeed;
	_vangle = clamp(_vangle, -PI_F / 2 + 0.01f, PI_F / 2 - 0.01f);

	vec3 forward(cosf(_hangle), sinf(_hangle), 0);
	vec3 right(sinf(_hangle), -cosf(_hangle), 0);
	float changeForward = 0;
	float changeRight = 0;
	float changeUp = 0;

	if (input->isKeyDown(input::KeyboardKeys::Forward)) {
		changeForward += 1.0f;
	}

	if (input->isKeyDown(input::KeyboardKeys::Backward)) {
		changeForward -= 1.0f;
	}

	if (input->isKeyDown(input::KeyboardKeys::Right)) {
		changeRight += 1.0f;
	}

	if (input->isKeyDown(input::KeyboardKeys::Left)) {
		changeRight -= 1.0f;
	}

	if (input->isKeyDown(input::KeyboardKeys::Up)) {
		changeUp += 1.0f;
	}

	if (input->isKeyDown(input::KeyboardKeys::Down)) {
		changeUp -= 1.0f;
	}

	_position += (changeForward * forward + changeRight * right + changeUp * vec3(0, 0, 1)) * dt * moveSpeed;
}

void FPSCamera::setTransformPipeline(TransformPipeline *transform) const
{
	vec3 direction(cosf(_hangle) * cosf(_vangle), sinf(_hangle) * cosf(_vangle), sinf(_vangle));
	vec3 target(_position + direction);

	ASSERT(transform, "null transform");

	transform->lookat(_position, target, vec3(0, 0, 1));
}

}
