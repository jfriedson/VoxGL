#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>


class Camera {
private:
	glm::vec2 lastMousePos;
	bool focused;

public:
	glm::vec3 position;
	glm::vec2 direction;

	Camera();

	glm::vec3 facingRay();
	glm::mat3 getMatrix();
	glm::mat4 getMatrix4();

	void handleInputs(GLFWwindow*);
};
