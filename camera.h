#pragma once

#include "voxgl.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>


class Camera {
public:
	Camera();

	glm::vec3 facingRay();
	glm::mat3 getMatrix();
	glm::mat4 getMatrix4();

	void handleInputs(voxgl::GLFWwindow_ptr& window);

	// data
	glm::vec3 position;
	glm::vec2 direction;

private:
	glm::vec2 lastMousePos;
	bool focused;
};
