#pragma once

#include <GL/gl3w.h>

#include <assimp/scene.h>


class Texture
{
public:
	Texture();
	~Texture();

	bool LoadTextureNearest(const std::string fileLoc);
	bool LoadTextureNearest(const aiTexture* texture);

	bool LoadTextureLinear(const std::string fileLoc);
	bool LoadTextureLinear(const aiTexture* texture);

	void UseTexture(int unit);
	void clearTexture();

	GLuint getTexID() { return textureID; }

private:
	GLuint textureID;
	int width, height, bitDepth;

	std::string fileLocation;
};
