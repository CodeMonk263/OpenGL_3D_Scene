#pragma once

#include <vector>
#include <string>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Mesh.h"
#include "Texture.h"

class Model
{
public:
	Model();

	void LoadModel(const std::string& fileName);
	/*!< Loads the scene of the model using assimp library */

	void RenderModel();
	/*!< Renders the mesh and sets the textures */

	void ClearModel();
	/*!< Clears mesh by assigning to null pointer */

	~Model();

private:

	void LoadNode(aiNode *node, const aiScene *scene);
	/*!< Loads all meshes in the scene and calls child nodes recursively */

	void LoadMesh(aiMesh *mesh, const aiScene *scene);
	/*!< Loads each mesh by assigning coordinates to vectors of indices and vertices */

	void LoadMaterials(const aiScene *scene);
	/*!< Finds the Model obj files and loads the textures in them */

	std::vector<Mesh*> meshList;
	std::vector<Texture*> textureList;
	std::vector<unsigned int> meshToTex;
};

