#ifndef EBONY_FPS_CAMERA_H_
#define EBONY_FPS_CAMERA_H_

#include <glm/glm.hpp>

#include "graphics/TransformPipeline.h"

namespace ebony {

class FPSCamera {
public:
	FPSCamera();

	void update(float dt);
	void setTransformPipeline(TransformPipeline &transform) const;

private:
	glm::vec3 _position;
	float _vangle;
	float _hangle;
};

}

#endif
