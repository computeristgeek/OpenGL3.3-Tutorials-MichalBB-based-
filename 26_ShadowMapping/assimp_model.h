#pragma once

#include "shaders.h"
#include "vertexBufferObject.h"
#include "texture.h"

class CAssimpModel
{
public:
	GLboolean LoadModelFromFile(char* sFilePath);

	static GLvoid FinalizeVBO();
	static GLvoid BindModelsVAO();
	static GLvoid ReleaseAllModelData();

	GLvoid RenderModel(GLenum = GL_TRIANGLES);

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
