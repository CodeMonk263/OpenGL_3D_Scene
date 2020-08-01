#pragma once

#include <GL\glew.h>

#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(const char* fileLoc);

	bool LoadTexture();
	/*!< Loads texture files using stbi library without alpha channel */

	bool LoadTextureA();
	/*!< Loads texture files using stbi library with alpha channel */

	void UseTexture();
	/*!< Sets given texture as active texture and binds it */

	void ClearTexture();
	/*!< Sets all values to zero and deletes the texture */

	~Texture();

private:
	GLuint textureID;
	int width, height, bitDepth;

	const char* fileLocation;
};

