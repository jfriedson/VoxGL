#pragma once

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <memory>



namespace voxgl {
	// enable smart pointer with GLFW window
	struct window_deleter { void operator()(GLFWwindow* w) { glfwDestroyWindow(w); }; };
	typedef std::unique_ptr<GLFWwindow, window_deleter> GLFWwindow_ptr;

	// create window and OpenGL context
	GLFWwindow_ptr createWindow(const std::string& windowName, int windowWidth, int windowHeight, bool fullscreen);

	// shader creation
	GLint createShader(const std::string& filepath, GLenum type);
	GLint createProgram(std::vector<GLuint>& shaders);

	// GLFW callback handlers
	void windowFocusHndlr(GLFWwindow* window, int focused);
	void framebufferSizeHndlr(GLFWwindow* window, int framebufferWidth, int framebufferHeight);

	void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);
}
