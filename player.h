#pragma once

#include <glm/glm.hpp>

#include "timer.h"
#include "voxgl.h"

#include "DAG.h"
#include "camera.h"

#include <GLFW/glfw3.h>

#include <memory>


class Player
{
public:
	Player();

	void update(const unsigned int timeDelta);
	void handleInputs(voxgl::GLFWwindow_ptr& window);

	// data
	Camera camera;

	bool onGround;

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
};
