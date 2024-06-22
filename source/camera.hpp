#ifndef CAMERA_HPP_INCLUDED
#define CAMERA_HPP_INCLUDED

#include "vec3.hpp"
#include "hostStructs.hpp"

class Camera {
private:
	cl_Camera clCamera;
public:
	vec3 position{ 1.0f, 1.0f, 1.0f };
	vec3 up{ 0.0f, 1.0f, 0.0f };
	vec3 front{ 0.0f, 0.0f, -1.0f };
	float yaw = 70.0f;
	float pitch = 45.0f;

	Camera() = default;
	Camera(vec3 position_, float yaw_, float pitch_) :
		position(position_), yaw(yaw_), pitch(pitch_) {
		calculateEye();
	}

	void calculateEye();
	const cl_Camera& getProjection(float screenWidth, float screenHeight);
};

#endif