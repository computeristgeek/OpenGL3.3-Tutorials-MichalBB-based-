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
	static GLboolean LoadTerrainShaderProgram();
	static GLvoid ReleaseTerrainShaderProgram();

	GLboolean LoadHeightMapFromImage(string sImagePath);
	GLvoid ReleaseHeightmap();

	GLvoid RenderHeightmap();
	GLvoid RenderHeightmapForNormals();

	GLvoid SetRenderSize(GLfloat fQuadSize, GLfloat fHeight);
	GLvoid SetRenderSize(GLfloat fRenderX, GLfloat fHeight, GLfloat fRenderZ);

	GLint GetNumHeightmapRows();
	GLint GetNumHeightmapCols();

	glm::mat4 GetScaleMatrix();

	static CShaderProgram* GetShaderProgram();
	static CShaderProgram* GetGrassShaderProgram();

	GLfloat GetHeightFromRealVector(glm::vec3 vRealPosition);

	CMultiLayeredHeightmap();

private:
	UINT uiVAO, uiGrassVAO;

	GLboolean bLoaded;
	GLboolean bShaderProgramLoaded;
	GLint iRows;
	GLint iCols;

	glm::vec3 vRenderScale;

	vector< vector< glm::vec3> > vVertexData;

	CVertexBufferObject vboHeightmapData;
	CVertexBufferObject vboHeightmapIndices;
	CVertexBufferObject vboGrassData;

	GLint iNumGrassTriangles;
	GLfloat fTimePassed;

	static CShaderProgram spTerrain;
	static CShaderProgram spGrass;
	static CShader shTerrainShaders[NUMTERRAINSHADERS];
};

GLvoid RenderWorldHeightmap(CShaderProgram* spTerrain);