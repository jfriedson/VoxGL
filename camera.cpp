#include "camera.h"
#include <iostream>


constexpr float pi = 3.14159265f;

Camera::Camera() {
	position = glm::vec3(0.f);
	direction = glm::vec2(0.f, 0.f);

	lastMousePos = glm::vec3(0.f);
	focused = false;
}

glm::vec3 Camera::facingRay() {
	return glm::vec3(glm::cos(direction.y) * glm::sin(direction.x),
					 glm::sin(direction.y),
					 glm::cos(direction.y) * glm::cos(direction.x));
}

glm::mat3 Camera::getMatrix() {
	glm::vec3 Z = facingRay();
	glm::vec3 Y(0.f, 1.f, 0.f);
	glm::vec3 X = glm::normalize(glm::cross(Z, Y));

	Y = -glm::cross(Z, X);

	return glm::mat3(X, Y, Z);
}

glm::mat4 Camera::getMatrix4() {
	glm::mat4 trans(1.f);
	trans[3] = glm::vec4(-position, 1.f);

	glm::mat3 camMat = getMatrix();

	glm::mat4 dir;
	dir[0] = glm::vec4(camMat[0], 0.f);
	dir[1] = glm::vec4(camMat[1], 0.f);
	dir[2] = glm::vec4(camMat[2], 0.f);
	dir[3] = glm::vec4(0.f, 0.f, 0.f, 1.f);

	return trans * dir;
}

void Camera::handleInputs(voxgl::GLFWwindow_ptr& window) {
	if (glfwGetInputMode(window.get(), GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
		if(focused)
			focused = false;

		return;
	}

	if (!focused) {
		focused = true;
		glfwSetCursorPos(window.get(), lastMousePos.x, lastMousePos.y);
	}

	double xpos, ypos;
	glfwGetCursorPos(window.get(), &xpos, &ypos);

	glm::vec2 mouse((float)xpos, (float)ypos);
	mouse -= lastMousePos;
	lastMousePos = glm::vec2((float)xpos, (float)ypos);

	const float mouseSensitivity = -0.001f;
	direction.x += mouseSensitivity * mouse.x;
	direction.y += mouseSensitivity * mouse.y;

	if (direction.x < -pi)
		direction.x += 2.f * pi;
	else if (direction.x > pi)
		direction.x -= 2.f * pi;

	const float tolerance = 0.01f;
	if (direction.y > pi / 2.f - tolerance)
		direction.y = pi / 2.f - tolerance;
	else if (direction.y < -pi / 2.f + tolerance)
		direction.y = -pi / 2.f + tolerance;
}
