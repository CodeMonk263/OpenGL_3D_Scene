#pragma once

#include <GL\glew.h>

class Material
{
public:
	Material();

	Material(GLfloat sIntensity, GLfloat shine);
	/*!< Sets the shininess and specular intensity of the material */

	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);
	/*!< Assigns uniforms to variable locations passed as parameters */

	~Material();

private: 
	GLfloat specularIntensity;
	GLfloat shininess;
};

