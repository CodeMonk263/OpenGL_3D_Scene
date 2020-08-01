#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <GLFW\glfw3.h>

class Controls
{
public:
	Controls();

	GLfloat controlFanSpeed(bool* keys);
	/*!< Controls the fan when key F is pressed */

	unsigned int controlLight(bool* keys, unsigned int lightCount);
	/*!< Toggles the torch when key L is pressed */

	~Controls();

private:
	GLfloat rotationSpeed;
	GLfloat rotationCount;
};

