#pragma once

#include "shaders.h"
#include "vertexBufferObject.h"
#include "texture.h"

class CMaterial
{
public:
	GLint iTexture;
};

class CAssimpModel
{
public:
	GLboolean LoadModelFromFile(char* sFilePath);

	static GLvoid FinalizeVBO();
	static GLvoid BindModelsVAO();

	GLvoid RenderModel();
	CAssimpModel();
private:
	GLboolean bLoaded;
	static CVertexBufferObject vboModelData;
	static GLuint uiVAO;
	static vector<CTexture> tTextures;
	vector<int> iMeshStartIndices;
	vector<int> iMeshSizes;
	vector<int> iMaterialIndices;
	GLint iNumMaterials;
};
