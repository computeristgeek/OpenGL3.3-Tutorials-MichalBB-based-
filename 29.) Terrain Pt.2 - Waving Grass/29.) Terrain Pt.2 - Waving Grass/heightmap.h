#pragma once

#include "vertexBufferObject.h"
#include "shaders.h"

#define NUMTERRAINSHADERS 6

/****************************************************************************************

Class:		CMultiLayeredHeightmap

Purpose:	Wraps FreeType heightmap loading and rendering, also allowing
			to use multiple layers of textures with transitions between them.

****************************************************************************************/

class CMultiLayeredHeightmap
{
public:
	static bool LoadTerrainShaderProgram();
	static void ReleaseTerrainShaderProgram();

	bool LoadHeightMapFromImage(string sImagePath);
	void ReleaseHeightmap();

	void RenderHeightmap();
	void RenderHeightmapForNormals();

	void SetRenderSize(float fQuadSize, float fHeight);
	void SetRenderSize(float fRenderX, float fHeight, float fRenderZ);

	int GetNumHeightmapRows();
	int GetNumHeightmapCols();

	glm::mat4 GetScaleMatrix();

	static CShaderProgram* GetShaderProgram();
	static CShaderProgram* GetGrassShaderProgram();

	float GetHeightFromRealVector(glm::vec3 vRealPosition);

	CMultiLayeredHeightmap();

private:
	UINT uiVAO, uiGrassVAO;

	bool bLoaded;
	bool bShaderProgramLoaded;
	int iRows;
	int iCols;

	glm::vec3 vRenderScale;

	vector< vector< glm::vec3> > vVertexData;

	CVertexBufferObject vboHeightmapData;
	CVertexBufferObject vboHeightmapIndices;
	CVertexBufferObject vboGrassData;

	int iNumGrassTriangles;
	float fTimePassed;

	static CShaderProgram spTerrain;
	static CShaderProgram spGrass;
	static CShader shTerrainShaders[NUMTERRAINSHADERS];
};

void RenderWorldHeightmap(CShaderProgram* spTerrain);