#include "graphics/TransformPipelineStereo.h"

#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "utils/maths.h"

using namespace std;
using namespace glm;

namespace ebony {

	TransformPipelineStereo::TransformPipelineStereo()
	{
		_isDirty = true;
	}

	void TransformPipelineStereo::perspective(float fov, int width, int height, float near, float far, float screen, float eyeDistance)
	{
		_eyeDistance = eyeDistance;
		
		float screenX = screen * tan(fov * PI_F / 360);
		float top = tan(fov * PI_F / 360) * near * height / width;
		float bottom = -top;
		float left = -near * (screenX - _eyeDistance / 2) / screen;
		float right = near * (screenX + _eyeDistance / 2) / screen;
		
		_projection[Left] = glm::frustum(left, right, bottom, top, near, far);
		_projection[Right] = glm::frustum(-right, -left, bottom, top, near, far);
		
		_isDirty = true;
	}

	void TransformPipelineStereo::lookat(const vec3 &position, const vec3 &target, const vec3 &up)
	{
		_view[Left] = glm::translate(glm::mat4(1.0f), glm::vec3(-_eyeDistance / 2, 0, 0));
		_view[Right] = glm::translate(glm::mat4(1.0f), glm::vec3(_eyeDistance / 2, 0, 0));
		
		_view[Left] *= glm::lookAt(position, target, up);
		_view[Right] *= glm::lookAt(position, target, up);
		
		_isDirty = true;
	}
		
	TransformPipelineStereo &TransformPipelineStereo::translate(const vec3 &offset)
	{
		_model = glm::translate(_model, offset);
		_isDirty = true;

		return *this;
	}

	TransformPipelineStereo &TransformPipelineStereo::translate(float x, float y, float z)
	{
		_model = glm::translate(_model, vec3(x, y, z));
		_isDirty = true;

		return *this;
	}

	TransformPipelineStereo &TransformPipelineStereo::rotate(float rx, float ry, float rz)
	{
		_model = glm::rotate(_model, rx, vec3(1, 0, 0));
		_model = glm::rotate(_model, ry, vec3(0, 1, 0));
		_model = glm::rotate(_model, rz, vec3(0, 0, 1));
		_isDirty = true;

		return *this;
	}

	TransformPipelineStereo &TransformPipelineStereo::rotate(const vec3 &axis, float angle)
	{
		_model = glm::rotate(_model, angle, axis);
		_isDirty = true;

		return *this;
	}

	TransformPipelineStereo &TransformPipelineStereo::scale(float scale)
	{
		_model = glm::scale(_model, vec3(scale, scale, scale));
		_isDirty = true;

		return *this;
	}

	TransformPipelineStereo &TransformPipelineStereo::scale(float sx, float sy, float sz)
	{
		_model = glm::scale(_model, vec3(sx, sy, sz));
		_isDirty = true;

		return *this;
	}

	TransformPipelineStereo &TransformPipelineStereo::scale(const vec3 &scale)
	{
		_model = glm::scale(_model, scale);
		_isDirty = true;

		return *this;
	}

	TransformPipelineStereo &TransformPipelineStereo::identity()
	{
		_model = mat4();
		_isDirty = true;

		return *this;
	}

	void TransformPipelineStereo::save()
	{
		_modelStack.push_back(_model);
	}

	void TransformPipelineStereo::restore()
	{
		if (_modelStack.empty()) {
			_model = mat4();
		} else {
			_model = _modelStack.back();
			_modelStack.pop_back();
		}

		_isDirty = true;
	}

	const mat4 &TransformPipelineStereo::getProjection(Eye eye)
	{
		updateCache();
		return _projection[eye];
	}

	const mat4 &TransformPipelineStereo::getModel()
	{
		updateCache();
		return _model;
	}

	const mat4 &TransformPipelineStereo::getView(Eye eye)
	{
		updateCache();
		return _view[eye];
	}

	const mat4 &TransformPipelineStereo::getModelView(Eye eye)
	{
		updateCache();
		return _modelView[eye];
	}

	const mat4 &TransformPipelineStereo::getMvp(Eye eye)
	{
		updateCache();
		return _mvp[eye];
	}

	const mat3 &TransformPipelineStereo::getNormal(Eye eye)
	{
		updateCache();
		return _normal[eye];
	}

	void TransformPipelineStereo::updateCache()
	{
		if (!_isDirty) {
			return;
		}

		_isDirty = false;
		
		for (int i = 0; i < 2; ++i) {
			_modelView[i] = _view[i] * _model;
			_mvp[i] = _projection[i] * _modelView[i];
			_normal[i] = mat3(transpose(inverse(_modelView[i])));
		}
	}

}
