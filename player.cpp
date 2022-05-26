#include "Player.h"


Player::Player() {
	position = glm::vec3(0);
	velocity = glm::vec3(0);
	acceleration = glm::vec3(0);
	onGround = false;
}

void Player::update(unsigned int timeDelta) {
	velocity += acceleration;
	position += velocity * glm::vec3(double(timeDelta) * .000001);
	velocity *= 0.9f;
	acceleration = glm::vec3(0.f);

	//position = glm::clamp(position, glm::vec3(1.f), glm::vec3(300.f));

	camera.position = position + glm::vec3(0.f, 1.f, 0.f);
}

void Player::handleInputs(GLFWwindow* window) {
	camera.handleInputs(window);

	glm::mat3 basis = camera.getMatrix();

	float speedMod = 1.f;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		speedMod = 0.5f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		acceleration += camera.facingRay() * speedMod;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		acceleration -= camera.facingRay() * speedMod;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		acceleration -= basis[0] * speedMod;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		acceleration += basis[0] * speedMod;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		acceleration += glm::vec3(0.f, speedMod, 0.f);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		acceleration += glm::vec3(0.f, -speedMod, 0.f);
}
