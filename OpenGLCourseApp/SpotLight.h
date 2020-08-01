#pragma once
#include "PointLight.h"
class SpotLight :
	public PointLight
{
public:
	SpotLight();

	SpotLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat xDir, GLfloat yDir, GLfloat zDir,
		GLfloat con, GLfloat lin, GLfloat exp,
		GLfloat edg);
	/*!< Initializes spot lights and its properties */

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
		GLuint edgeLocation);
	/*!< Assigns the location of uniforms to their variables */

	void SetFlash(glm::vec3 pos, glm::vec3 dir);
	/*!< Toggles if a light is to be used as a flash light and moves with camera movement */

	~SpotLight();

private:
	glm::vec3 direction;

	GLfloat edge, procEdge;
};

