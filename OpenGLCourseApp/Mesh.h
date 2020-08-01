#pragma once

#include <GL\glew.h>

class Mesh
{
public:
	Mesh();

	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	/*!< Creates the mesh by assigning VBOs and IBOs to the object */

	void RenderMesh();
	/*!< Renders the mesh triangle by triangle using draw element function */

	void ClearMesh();
	/*!< Clears the mesh by deleting the buffers */

	~Mesh();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};

