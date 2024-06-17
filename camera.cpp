#include "camera.hpp"

void Camera::calculateEye() {
	front.x = cos(yaw * CL_M_PI_F / 180.0f) * cos(pitch * CL_M_PI_F / 180.0f);
	front.y = sin(pitch * CL_M_PI_F / 180.0f);
	front.z = sin(yaw * CL_M_PI_F / 180.0f) * cos(pitch * CL_M_PI_F / 180.0f);
	front.normalize();
}

const cl_Camera& Camera::getProjection(float screenWidth, float screenHeight) {
	const float VIEWPORT_HEIGHT = 2.0f;
	const float VIEWPORT_WIDTH = 2.0f * (screenWidth / screenHeight);

	// calculate vectors from camera
	vec3 xAxis = normalize(cross(front, up));
	vec3 yAxis = cross(xAxis, front);

	// use vectors to determine projection coordinates
	const vec3 VIEWPORT_U = -xAxis * VIEWPORT_WIDTH;
	const vec3 VIEWPORT_V = -yAxis * VIEWPORT_HEIGHT;
	const vec3 PIXEL_DELTA_U = VIEWPORT_U / screenWidth;
	const vec3 PIXEL_DELTA_V = VIEWPORT_V / screenHeight;
	const vec3 VIEWPORT00 = position - front - VIEWPORT_U * 0.5f - VIEWPORT_V * 0.5f;

	// calculate final projection data
	return {
		VIEWPORT00 + (PIXEL_DELTA_U + PIXEL_DELTA_V) * 0.5f, // pixel00Center
		PIXEL_DELTA_U, PIXEL_DELTA_V, position 
	};
}