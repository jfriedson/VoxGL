#include "voxgl.h"

#include <fstream>
#include <iostream>


namespace voxgl
{
	GLFWwindow_ptr createWindow(const std::string& windowName, int windowWidth, int windowHeight, bool fullscreen) {
		// GLFW window creation
		if (!glfwInit()) {
			std::cerr << "unable to initialize GLFW" << std::endl;
			exit(false);
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		if (fullscreen)
			glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
		else
			glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
		glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);

		glfwWindowHint(GLFW_SAMPLES, 0);	// ray tracing can't utilize MSAA

		GLFWwindow_ptr window{ glfwCreateWindow(windowWidth, windowHeight, windowName.c_str(), fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL)};

		if (window == nullptr) {
			std::cerr << "unable to create window" << std::endl;
			glfwTerminate();
			exit(false);
		}

		// OpenGL context creation
		glfwMakeContextCurrent(window.get());
		glfwSwapInterval(0);
		glfwSetWindowFocusCallback(window.get(), windowFocusHndlr);
		glfwSetFramebufferSizeCallback(window.get(), framebufferSizeHndlr);

		if (gl3wInit()) {
			std::cerr << "unable to initialize GL3W" << std::endl;
			glfwTerminate();
			exit(false);
		}

		glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(window.get(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);


		glFrontFace(GL_CCW);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glDisable(GL_MULTISAMPLE);

		return std::move(window);
	}

	void destroyWindow(const std::shared_ptr<GLFWwindow> window) {
		glfwDestroyWindow(window.get());
	}


	GLint createShader(const std::string& filepath, GLenum type) {
		unsigned int shaderHandle = glCreateShader(type);
		std::string line;
		std::string shaderStr = "";
		std::ifstream fileStream(filepath);
		if (fileStream.is_open()) {
			while (std::getline(fileStream, line))
				shaderStr += line + '\n';

			fileStream.close();
		}
		else
			std::cerr << "unable to read shader file " << filepath << std::endl;


		const char* cShaderStr = shaderStr.c_str();

		//std::cout << cShaderStr << std::endl;
		glShaderSource(shaderHandle, 1, &cShaderStr, NULL);
		glCompileShader(shaderHandle);

		int  success;
		glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(shaderHandle, 512, NULL, infoLog);
			std::cerr << "error in shader compilation" << std::endl;
			std::cerr << filepath << std::endl;
			std::cerr << infoLog << std::endl;
			return -1;
		}

		return shaderHandle;
	}

	GLint createProgram(std::vector<GLuint>& shaders) {
		unsigned int program = glCreateProgram();

		for (const auto& shader : shaders)
			glAttachShader(program, shader);

		glLinkProgram(program);

		int  success;
		char infoLog[512];
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			std::cerr << "shader program: failed to link shader...\n" << infoLog << std::endl;
			return -1;
		}

		for (const auto& shader : shaders)
			glDeleteShader(shader);

		return program;
	}



	void windowFocusHndlr(GLFWwindow* window, int focused) {
		if (focused)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void framebufferSizeHndlr(GLFWwindow* window, int framebufferWidth, int framebufferHeight) {
		glViewport(0, 0, framebufferWidth, framebufferHeight);
	}


	void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam) {
		std::cout << "---------------" << std::endl;
		std::cout << "debug message " << id << std::endl;
		std::cout << message << std::endl;

		std::cout << "source ";
		switch (source)
		{
			case GL_DEBUG_SOURCE_API:             std::cout << "API"; break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Window System"; break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Shader Compiler"; break;
			case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Third Party"; break;
			case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Application"; break;
			case GL_DEBUG_SOURCE_OTHER:           std::cout << "Other"; break;
		}
		std::cout << std::endl;

		std::cout << "type ";
		switch (type) {
			case GL_DEBUG_TYPE_ERROR:               std::cout << "Error"; break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Deprecated Behaviour"; break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Undefined Behaviour"; break;
			case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Portability"; break;
			case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Performance"; break;
			case GL_DEBUG_TYPE_MARKER:              std::cout << "Marker"; break;
			case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Push Group"; break;
			case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Pop Group"; break;
			case GL_DEBUG_TYPE_OTHER:               std::cout << "Other"; break;
		}
		std::cout << std::endl;

		std::cout << "severity ";
		switch (severity) {
			case GL_DEBUG_SEVERITY_HIGH:         std::cout << "high"; break;
			case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "medium"; break;
			case GL_DEBUG_SEVERITY_LOW:          std::cout << "low"; break;
			case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "notification"; break;
		}
		std::cout << std::endl << std::endl;
	}
}
