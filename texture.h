#pragma once

#include <GL/gl3w.h>

#include <assimp/scene.h>


class Texture
{
public:
	Texture();
	~Texture();

	bool LoadTextureNearest(const std::string fileLoc, GLuint unit);
	bool LoadTextureNearest(const aiTexture* texture, GLuint unit);

	bool LoadTextureLinear(const std::string fileLoc, GLuint unit);
	bool LoadTextureLinear(const aiTexture* texture, GLuint unit);

	void UseTexture();
	void clearTexture();

	GLuint getTexID() { return textureID; }

private:
	GLuint textureID;
	GLuint textUnit;

	int width, height, bitDepth;

	std::string fileLocation;
};
