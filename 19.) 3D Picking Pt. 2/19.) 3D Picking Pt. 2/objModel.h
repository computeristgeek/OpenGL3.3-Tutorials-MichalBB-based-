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
	bool LoadModel(string sFileName, string sMtlFileName);
	void RenderModel();
	void RenderBoundingBox();
	void DeleteModel();

	int GetPolygonCount();

	CObjModel();
private:
	bool bLoaded;
	int iAttrBitField;
	int iNumFaces;

	bool LoadMaterial(string sFullMtlFileName);

	CVertexBufferObject vboModelData, vboModelBB, vboModelBBIndices;
	UINT uiVAO;
	UINT uiBBVAO;
	CTexture tAmbientTexture;
};