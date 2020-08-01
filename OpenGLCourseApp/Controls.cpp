#include "Controls.h"

Controls::Controls() {
	rotationSpeed = 1.0f;
	rotationCount = 0.0f;
}

GLfloat Controls::controlFanSpeed(bool* keys)
{
	if (keys[GLFW_KEY_F]) {
		if (rotationSpeed == 4.0f) {
			rotationSpeed = 0.0f;
		}
		else {
			rotationSpeed++;
		}
	}
	if (rotationCount == 360.0f) {
		rotationCount = 0.0f;
	}
	else {
		rotationCount += rotationSpeed;
	}
	return rotationCount;
}

unsigned int Controls::controlLight(bool* keys, unsigned int lightCount)
{
	
	if (keys[GLFW_KEY_L]) {
		if (lightCount < 7) {
			lightCount++;
		}
		else if (lightCount > 7){
			lightCount--;
		}
		return lightCount;
	}
	else {
		return lightCount;
	}
}

Controls::~Controls() {

}