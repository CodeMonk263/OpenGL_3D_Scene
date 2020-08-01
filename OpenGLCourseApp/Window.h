#pragma once

#include "stdio.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Window
{
public:
	Window();

	Window(GLint windowWidth, GLint windowHeight);
	/*!< Initialize window with the given dimensions and the keys for control */

	int Initialise();
	/*!< Uses GLFW functions to initialize window */

	GLint getBufferWidth() { return bufferWidth; }
	/*!< Returns dimensions */
	GLint getBufferHeight() { return bufferHeight; }
	/*!< Returns dimensions */

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* getsKeys() { return keys; }
	/*!< Returns array of boolean for toggling keys */

	GLfloat getXChange();
	/*!< Change in X direction for mouse control */
	GLfloat getYChange();
	/*!< Change in Y direction for mouse control */

	void swapBuffers() { glfwSwapBuffers(mainWindow); }
	/*!< Swaps current buffer with the underlying buffer */

	~Window();

private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;

	void createCallbacks();
	/*!< Sets callbacks for key and cursor positions */

	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	/*!< Sets action for each key when pressed and return boolean */

	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
	/*!< Handles mouse movement by tracking changes in x and y */
};