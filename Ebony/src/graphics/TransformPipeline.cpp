#include "graphics/TransformPipeline.h"

#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

namespace ebony {

TransformPipeline::TransformPipeline()
{
	_isDirty = true;
}

void TransformPipeline::perspective(float fov, int width, int height, float near, float far)
{
	_projection = glm::perspectiveFov(fov, static_cast<float>(width), static_cast<float>(height), near, far);
	_isDirty = true;
}

void TransformPipeline::orthographic(float left, float right, float bottom, float top, float near, float far)
{
	_projection = glm::ortho(left, right, bottom, top, near, far);
	_isDirty = true;
}

void TransformPipeline::lookat(const vec3 &position, const vec3 &target, const vec3 &up)
{
	_view = glm::lookAt(position, target, up);
	_isDirty = true;
}
		
TransformPipeline &TransformPipeline::translate(const vec3 &offset)
{
	_model = glm::translate(_model, offset);
	_isDirty = true;

	return *this;
}

TransformPipeline &TransformPipeline::translate(float x, float y, float z)
{
	_model = glm::translate(_model, vec3(x, y, z));
	_isDirty = true;

	return *this;
}

TransformPipeline &TransformPipeline::rotate(float rx, float ry, float rz)
{
	_model = glm::rotate(_model, rx, vec3(1, 0, 0));
	_model = glm::rotate(_model, ry, vec3(0, 1, 0));
	_model = glm::rotate(_model, rz, vec3(0, 0, 1));
	_isDirty = true;

	return *this;
}

TransformPipeline &TransformPipeline::rotate(const vec3 &axis, float angle)
{
	_model = glm::rotate(_model, angle, axis);
	_isDirty = true;

	return *this;
}

TransformPipeline &TransformPipeline::scale(float scale)
{
	_model = glm::scale(_model, vec3(scale, scale, scale));
	_isDirty = true;

	return *this;
}

TransformPipeline &TransformPipeline::scale(float sx, float sy, float sz)
{
	_model = glm::scale(_model, vec3(sx, sy, sz));
	_isDirty = true;

	return *this;
}

TransformPipeline &TransformPipeline::scale(const vec3 &scale)
{
	_model = glm::scale(_model, scale);
	_isDirty = true;

	return *this;
}

TransformPipeline &TransformPipeline::identity()
{
	_model = mat4();
	_isDirty = true;

	return *this;
}

void TransformPipeline::save()
{
	_modelStack.push_back(_model);
}

void TransformPipeline::restore()
{
	if (_modelStack.empty()) {
		_model = mat4();
	} else {
		_model = _modelStack.back();
		_modelStack.pop_back();
	}

	_isDirty = true;
}

const mat4 &TransformPipeline::getProjection()
{
	updateCache();
	return _projection;
}

const mat4 &TransformPipeline::getModel()
{
	updateCache();
	return _model;
}

const mat4 &TransformPipeline::getView()
{
	updateCache();
	return _view;
}

const mat4 &TransformPipeline::getModelView()
{
	updateCache();
	return _modelView;
}

const mat4 &TransformPipeline::getMvp()
{
	updateCache();
	return _mvp;
}

const mat3 &TransformPipeline::getNormal()
{
	updateCache();
	return _normal;
}

void TransformPipeline::updateCache()
{
	if (!_isDirty) {
		return;
	}

	_isDirty = false;
	_modelView = _view * _model;
	_mvp = _projection * _modelView;
	_normal = mat3(transpose(inverse(_modelView)));
}

}
