#include <iostream>

#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


Texture::Texture() {
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}


bool Texture::LoadTextureNearest(const std::string fileLoc, GLuint unit) {
	unsigned char* texData;

	texData = stbi_load(fileLoc.c_str(), &width, &height, &bitDepth, 0);

	if (texData == nullptr || (bitDepth != 1 && bitDepth != 3 && bitDepth != 4)) {
		std::cout << "couldn't load " << fileLoc << std::endl;

		texData = stbi_load("./assets/tile_diff.jpg", &width, &height, &bitDepth, 0);

		if (texData == nullptr || (bitDepth != 1 && bitDepth != 3 && bitDepth != 4)) {
			std::cout << "couldn't load backup texture" << std::endl;

			return 0;
		}
	}

	this->textUnit = unit;
	glActiveTexture(GL_TEXTURE0 + unit);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 8.f);

	if (bitDepth == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	else if (bitDepth == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
	else if (bitDepth == 1)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, texData);

	//glGenerateMipmap(GL_TEXTURE_2D);


	return true;
}

bool Texture::LoadTextureNearest(const aiTexture* texture, GLuint unit) {
	unsigned char* texData;

	// Compressed texture
	if (texture->mHeight == 0)
		texData = stbi_load_from_memory(reinterpret_cast<unsigned char*>(texture->pcData), texture->mWidth, &width, &height, &bitDepth, 0);
	else
		texData = stbi_load_from_memory(reinterpret_cast<unsigned char*>(texture->pcData), texture->mWidth * texture->mHeight, &width, &height, &bitDepth, 0);

	if (texData == nullptr || (bitDepth != 1 && bitDepth != 3 && bitDepth != 4)) {
		std::cout << "couldn't load " << texture->mFilename.C_Str() << std::endl;

		texData = stbi_load("./assets/tile_diff.jpg", &width, &height, &bitDepth, 0);

		if (texData == nullptr || (bitDepth != 1 && bitDepth != 3 && bitDepth != 4)) {
			std::cout << "couldn't load backup texture" << std::endl;

			return 0;
		}
	}

	this->textUnit = unit;
	glActiveTexture(GL_TEXTURE0 + unit);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 8.f);

	if (bitDepth == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	else if (bitDepth == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
	else if (bitDepth == 1)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, texData);

	//glGenerateMipmap(GL_TEXTURE_2D);


	return true;
}


bool Texture::LoadTextureLinear(const std::string fileLoc, GLuint unit) {
	unsigned char* texData;

	texData = stbi_load(fileLoc.c_str(), &width, &height, &bitDepth, 0);

	if (texData == nullptr || (bitDepth != 1 && bitDepth != 3 && bitDepth != 4)) {
		std::cout << "couldn't load " << fileLoc << std::endl;

		texData = stbi_load("./assets/tile_diff.jpg", &width, &height, &bitDepth, 0);

		if (texData == nullptr || (bitDepth != 1 && bitDepth != 3 && bitDepth != 4)) {
			std::cout << "couldn't load backup texture" << std::endl;

			return 0;
		}
	}

	this->textUnit = unit;
	glActiveTexture(GL_TEXTURE0 + unit);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 8.f);

	if (bitDepth == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	else if (bitDepth == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
	else if (bitDepth == 1)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, texData);

	glGenerateMipmap(GL_TEXTURE_2D);


	return true;
}

bool Texture::LoadTextureLinear(const aiTexture* texture, GLuint unit) {
	unsigned char* texData;

	// Compressed texture
	if (texture->mHeight == 0)
		texData = stbi_load_from_memory(reinterpret_cast<unsigned char*>(texture->pcData), texture->mWidth, &width, &height, &bitDepth, 0);
	else
		texData = stbi_load_from_memory(reinterpret_cast<unsigned char*>(texture->pcData), texture->mWidth * texture->mHeight, &width, &height, &bitDepth, 0);

	if (texData == nullptr || (bitDepth != 1 && bitDepth != 3 && bitDepth != 4)) {
		std::cout << "couldn't load " << texture->mFilename.C_Str() << std::endl;

		texData = stbi_load("./assets/tile_diff.jpg", &width, &height, &bitDepth, 0);

		if (texData == nullptr || (bitDepth != 1 && bitDepth != 3 && bitDepth != 4)) {
			std::cout << "couldn't load backup texture" << std::endl;

			return 0;
		}
	}

	this->textUnit = unit;
	glActiveTexture(GL_TEXTURE0 + unit);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 8.f);

	if (bitDepth == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	else if (bitDepth == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
	else if (bitDepth == 1)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, texData);

	glGenerateMipmap(GL_TEXTURE_2D);


	return true;
}

void Texture::UseTexture() {
	glActiveTexture(GL_TEXTURE0 + this->textUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::clearTexture() {
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}


Texture::~Texture() {
	clearTexture();
}
