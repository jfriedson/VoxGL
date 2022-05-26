#pragma once

#include <vector>
#include <string>

#include <GL/gl3w.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Model
{
private:
	struct Vertex {
		glm::vec3 position;
		glm::vec2 texCoords;
		//glm::vec3 normal;
		//glm::vec3 tangent;

		Vertex(glm::vec3 position, glm::vec2 texCoords) : //, glm::vec3 normal, glm::vec3 tangent) :
			position(position),
			texCoords(texCoords)
			//normal(normal),
			//tangent(tangent)
		{
		}
	};

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	GLuint VAO, VBO, IBO;

public:
	Model(const char* modelFile);
	~Model();

	void render();
};
