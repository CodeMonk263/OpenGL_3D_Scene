#pragma once

#include <GL\glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <GLFW\glfw3.h>

class Camera
{
public:
	Camera();

	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
	/*!< Initializing camera and its properties */
	
	void keyControl(bool* keys, GLfloat deltaTime);
	/*!< For controlling camera movement via keyboard */

	void mouseControl(GLfloat xChange, GLfloat yChange);
	/*!< For controlling camera movement via mouse */

	glm::vec3 getCameraPosition();
	/*!< Gets Camera Position */

	glm::vec3 getCameraDirection();
	/*!< Gets Camera Direction */

	glm::mat4 calculateViewMatrix();
	/*!< Calculatess the view matrix using lookAt function */

	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void update();
};

