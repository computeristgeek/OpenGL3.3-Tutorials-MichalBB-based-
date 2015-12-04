#pragma once

#include "texture.h"
#include "vertexBufferObject.h"

/********************************

Class:		CObjModel

Purpose:	Class for handling obj
			model files.

********************************/

class CObjModel
{
public:
	GLboolean LoadModel(string sFileName, string sMtlFileName);
	GLvoid RenderModel();
	GLvoid RenderBoundingBox();
	GLvoid DeleteModel();

	GLint GetPolygonCount();

	CObjModel();
private:
	GLboolean bLoaded;
	GLint iAttrBitField;
	GLint iNumFaces;

	GLboolean LoadMaterial(string sFullMtlFileName);

	CVertexBufferObject vboModelData, vboModelBB, vboModelBBIndices;
	UINT uiVAO;
	UINT uiBBVAO;
	CTexture tAmbientTexture;
};