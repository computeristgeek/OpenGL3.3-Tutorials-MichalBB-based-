#pragma once

#include "texture.h"
#include "vertexBufferObject.h"

/********************************

Class:	CObjModel

Purpose: Class for handling obj
		 model files.

********************************/

class CObjModel
{
public:
	GLboolean loadModel(string sFileName, string sMtlFileName);
	GLvoid renderModel();
	GLvoid releaseModel();

	GLint getPolygonCount();

	CObjModel();
private:
	GLboolean bLoaded;
	GLint iAttrBitField;
	GLint iNumFaces;

	GLboolean loadMaterial(string sFullMtlFileName);

	CVertexBufferObject vboModelData;
	UINT uiVAO;
	CTexture tAmbientTexture;
};