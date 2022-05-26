#pragma once

#include <glm/glm.hpp>

#include "timer.h"
#include "voxgl.h"

#include "DAG.h"
#include "camera.h"

#include <GLFW/glfw3.h>


class Player
{
public:
	bool onGround;

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	Camera camera;

	Player();

	void update(unsigned int timeDelta);
	void handleInputs(GLFWwindow* window);
};
