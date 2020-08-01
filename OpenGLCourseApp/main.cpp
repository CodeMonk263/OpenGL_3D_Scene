#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "CommonValues.h"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Controls.h"

#include "Model.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Controls controller;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture woodenTexture;
Texture ceilingArtTexture;

Material shinyMaterial;
Material dullMaterial;

Model tablechair;
Model greenwall;
Model sidetable;
Model ceilingLight;
Model lamp;
Model fan;
Model bed;
Model cabinet;
Model tv;
Model sofa;
Model frame;
Model wallLamp;
Model vase;
Model bookshelf;

glm::vec3 lowerLight;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

static const char* vShader = "Shaders/shader.vert";
/*!< Vertex Shader */

static const char* fShader = "Shaders/shader.frag";
/*!< Fragment Shader */


void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount, 
						unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);
		
		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}
/*!< Calculates normals for complex shapes used for reflecting lights */

void CreateObjects() 
{
	unsigned int indices[] = {		
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
	//	x      y      z			u	  v			nx	  ny    nz
		-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	GLfloat wallOneVertices[] = {
	-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
	10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
	-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
	10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj1);
}
/*!<Creates all objects defined by creating meshes and pushing to meshlist*/

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}
/*!<Creates shaders from files and pushes to shaderlist*/

int main() 
{

	mainWindow = Window(1366, 768);	/*!<Window initialized*/
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);
	/*!<Camera defined*/

	controller = Controls();
	/*!<Controller for fan and lights*/

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	woodenTexture = Texture("Textures/woodfloor.png");
	woodenTexture.LoadTexture();
	ceilingArtTexture = Texture("Textures/art.jpg");
	ceilingArtTexture.LoadTexture();
	/*!<Textures Defined*/

	shinyMaterial = Material(4.0f, 256);
	dullMaterial = Material(0.3f, 4);
	/*!<Materials Defined*/

	tablechair = Model();
	tablechair.LoadModel("Models/tablechair.obj");

	greenwall = Model();
	greenwall.LoadModel("Models/greenwall.obj");

	sidetable = Model();
	sidetable.LoadModel("Models/side_table.obj");

	ceilingLight = Model();
	ceilingLight.LoadModel("Models/Ligne_Roset_Destructuree.obj");

	lamp = Model();
	lamp.LoadModel("Models/lamp.obj");

	fan = Model();
	fan.LoadModel("Models/fan.obj");

	bed = Model();
	bed.LoadModel("Models/bed.obj");

	cabinet = Model();
	cabinet.LoadModel("Models/cabinet.obj");

	tv = Model();
	tv.LoadModel("Models/tv.obj");

	sofa = Model();
	sofa.LoadModel("Models/sofa.obj");

	frame = Model();
	frame.LoadModel("Models/frame.obj");

	wallLamp = Model();
	wallLamp.LoadModel("Models/wall_lamp.obj");

	vase = Model();
	vase.LoadModel("Models/vase.obj");

	bookshelf = Model();
	bookshelf.LoadModel("Models/bookcase.obj");
	/*!<Models defined*/

	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 
								0.3f, 0.6f,
								0.0f, 0.0f, -1.0f);
	/*!<Universal Lights*/

	unsigned int pointLightCount = 0;
	pointLights[0] = PointLight(0.8f, 0.8f, 0.0f,
								0.3f, 0.3f,
								-2.0f, 0.5f, -8.0f,
								0.3f, 0.2f, 0.1f);
	/*!< Point Lights */
	pointLightCount++;

	unsigned int spotLightCount = 0;
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
							0.6f, 0.6f,
							-5.0f, 5.9f, 5.0f,
							0.0f, -1.0f, 0.0f,
							1.0f, 0.0f, 0.0f,
							20.0f);
	spotLightCount++;

	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
							0.6f, 0.6f,
							5.0f, 5.9f, -5.0f,
							0.0f, -1.0f, 0.0f,
							1.0f, 0.0f, 0.0f,
							20.0f);
	spotLightCount++;

	spotLights[2] = SpotLight(1.0f, 1.0f, 1.0f,
							0.6f, 0.6f,
							-5.0f, 5.9f, -5.0f,
							0.0f, -1.0f, 0.0f,
							1.0f, 0.0f, 0.0f,
							20.0f);
	spotLightCount++;

	spotLights[3] = SpotLight(1.0f, 1.0f, 1.0f,
							0.6f, 0.6f,
							5.0f, 5.9f, 5.0f,
							0.0f, -1.0f, 0.0f,
							1.0f, 0.0f, 0.0f,
							20.0f);
	spotLightCount++;

	spotLights[4] = SpotLight(1.0f, 1.0f, 1.0f,
							0.6f, 0.6f,
							5.0f, 7.93f, 9.4f,
							0.0f, -1.0f, 0.0f,
							1.0f, 0.0f, 0.0f,
							20.0f);
	spotLightCount++;

	spotLights[5] = SpotLight(1.0f, 1.0f, 1.0f,
						0.6f, 0.6f,
						9.5f, 6.0f, 5.0,
						0.0f, -1.0f, 0.0f,
						1.0f, 0.0f, 0.0f,
						20.0f);
	spotLightCount++;
	/*!< Spot Lights */

	spotLights[6] = SpotLight(1.0f, 1.0f, 1.0f,
							0.6f, 0.6f,
							0.0f, 0.0f, 0.0f,
							0.0f, -1.0f, 0.0f,
							1.0f, 0.0f, 0.0f,
							20.0f);
	/*!<Flash Light*/

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	while (!mainWindow.getShouldClose())
		/*!< Loop until window closed */
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime; 
		lastTime = now;

		glfwPollEvents();
		/*!< Get + Handle User Input */

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[6].SetFlash(lowerLight, camera.getCameraDirection());
		/*!< Set flash Light */

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		/*!< Clear the window */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, controller.controlLight(mainWindow.getsKeys(), spotLightCount));
		/*!< Setting all types of lights */

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		/*!< Setting projection and view matrix */

		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		/*!< Setting camera view */

		glm::mat4 model;

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		woodenTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 8.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ceilingArtTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(6.0f, -2.0f, -10.8f));
		model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		tablechair.RenderModel();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0, -2.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		greenwall.RenderModel();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-2.0f, -2.0f, -8.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		sidetable.RenderModel();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-5.0f, 8.0f, 5.0f));
		model = glm::rotate(model, (toRadians * 90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		ceilingLight.RenderModel();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(5.0f, 8.0f, -5.0f));
		model = glm::rotate(model, (toRadians * 90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		ceilingLight.RenderModel();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-5.0f, 8.0f, -5.0f));
		model = glm::rotate(model, (toRadians * 90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		ceilingLight.RenderModel();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(5.0f, 8.0f, 5.0f));
		model = glm::rotate(model, (toRadians * 90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		ceilingLight.RenderModel();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-2.0f, 0.45f, -8.0f));
		model = glm::rotate(model, (toRadians * 90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		lamp.RenderModel();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
		model = glm::rotate(model, (toRadians * controller.controlFanSpeed(mainWindow.getsKeys())), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		fan.RenderModel();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-6.0f, -2.0f, -8.0f));
		model = glm::rotate(model, (toRadians * 90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bed.RenderModel();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-5.7f, -2.0f, 8.0f));
		model = glm::rotate(model, (toRadians * 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.004f, 0.004f, 0.004f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		cabinet.RenderModel();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-9.3f, 0.28f, 8.0f));
		model = glm::rotate(model, (toRadians * 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.004f, 0.004f, 0.004f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		tv.RenderModel();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-5.0f, -1.7f, 4.0f));
		model = glm::rotate(model, (toRadians * 90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.003f, 0.003f, 0.003f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		sofa.RenderModel();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(8.0f, -16.0f, 9.8f));
		model = glm::scale(model, glm::vec3(0.008f, 0.008f, 0.008f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		frame.RenderModel();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(5.0f, 6.0f, 9.5f));
		model = glm::rotate(model, (toRadians * 180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wallLamp.RenderModel();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(9.5f, 6.0f, 5.0f));
		model = glm::rotate(model, (toRadians * 90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wallLamp.RenderModel();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(6.0f, -0.7f, -4.5f));
		model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		vase.RenderModel();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(9.0f, -2.0f, 5.0f));
		model = glm::rotate(model, (toRadians * 90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bookshelf.RenderModel();
		/*!< Applying transformations on model matrix and rendering all meshes of objects in scene */

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}