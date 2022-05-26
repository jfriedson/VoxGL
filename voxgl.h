#pragma once

#include <string>
#include <vector>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>


namespace voxgl {
	void windowFocusCallback(GLFWwindow* window, int focused);
	void framebufferSizeCallback(GLFWwindow* window, int framebufferWidth, int framebufferHeight);

	void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);

	GLFWwindow* createWindow(const char* windowName, int windowWidth, int windowHeight);
	void destroyWindow(GLFWwindow* window);

	GLuint createShader(std::string filepath, GLenum type);
	GLuint createProgram(std::vector<GLuint> &shaders);
}
