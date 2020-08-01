#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>

#include "CommonValues.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	/*!< Compiling shader from given file and assigning shaderID to each shader */

	std::string ReadFile(const char* fileLocation);
	/*!< Reads file and returns the string output */

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetAmbientColourLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetDirectionLocation();
	GLuint GetSpecularIntensityLocation();
	GLuint GetShininessLocation();
	GLuint GetEyePositionLocation();
	/*!< List of return functions for each uniform variables from the location */

	void SetDirectionalLight(DirectionalLight * dLight);
	/*!< Calls the useLight function and sets all the lights in the given list of directional lights */
	void SetPointLights(PointLight * pLight, unsigned int lightCount);
	/*!< Calls the useLight function and sets all the lights in the given list of point lights */
	void SetSpotLights(SpotLight * sLight, unsigned int lightCount);
	/*!< Calls the useLight function and sets all the lights in the given list of spot lights */

	void UseShader();
	/*!< Uses shader based on its shaderID */

	void ClearShader();
	/*!< Assigns each shader to zero and deletes them and the projection and view matrix */

	~Shader();

private:

	int pointLightCount;
	/*!< To keep track of number of point lights */
	int spotLightCount;
	/*!< To keep track of number of spot lights */

	GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformEyePosition,
		uniformSpecularIntensity, uniformShininess;

	/*!< Different structs for each type of light */
	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} uniformDirectionalLight;

	GLuint uniformPointLightCount;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	GLuint uniformSpotLightCount;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;
	} uniformSpotLight[MAX_SPOT_LIGHTS];

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	/*!< Takes data from shader file and assigns them to uniform variables */

	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	/*!< Creates shaders and attaches them to given program */
};

