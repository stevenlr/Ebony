#ifndef EBONY_GRAPHICS_TRANSFORM_PIPELINE_H_
#define EBONY_GRAPHICS_TRANSFORM_PIPELINE_H_

#include <deque>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ebony {

class TransformPipeline {
public:
	TransformPipeline();
	TransformPipeline(const TransformPipeline &tp) = delete;
		
	TransformPipeline &operator=(const TransformPipeline &tp) = delete;

	void perspective(float fov, int width, int height, float near, float far);
	void orthographic(float left, float right, float bottom, float top, float near, float far);

	void lookat(const glm::vec3 &position, const glm::vec3 &target, const glm::vec3 &up);
		
	TransformPipeline &translate(const glm::vec3 &offset);
	TransformPipeline &translate(float x, float y, float z);

	TransformPipeline &rotate(float rx, float ry, float rz);
	TransformPipeline &rotate(const glm::vec3 &axis, float angle);

	TransformPipeline &scale(float scale);
	TransformPipeline &scale(float sx, float sy, float sz);
	TransformPipeline &scale(const glm::vec3 &scale);

	TransformPipeline &identity();

	void save();
	void restore();

	const glm::mat4 &getProjection();
	const glm::mat4 &getModel();
	const glm::mat4 &getView();
	const glm::mat4 &getModelView();
	const glm::mat4 &getMvp();
	const glm::mat3 &getNormal();
private:
	void updateCache();

	glm::mat4				_projection;
	glm::mat4				_view;
	glm::mat4				_model;
	std::deque<glm::mat4>	_modelStack;

	bool					_isDirty;
	glm::mat4				_modelView;
	glm::mat4				_mvp;
	glm::mat3				_normal;
};

}

#endif
