#pragma once

#include "vertexBufferObject.h"
#include "shaders.h"

#define NUMTERRAINSHADERS 4

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

	GLfloat GetHeightFromRealVector(glm::vec3 vRealPosition);

	CMultiLayeredHeightmap();

private:
	GLuint uiVAO;

	GLboolean bLoaded;
	GLboolean bShaderProgramLoaded;
	GLint iRows;
	GLint iCols;

	glm::vec3 vRenderScale;

	vector< vector< glm::vec3> > vVertexData;

	CVertexBufferObject vboHeightmapData;
	CVertexBufferObject vboHeightmapIndices;

	static CShaderProgram spTerrain;
	static CShader shTerrainShaders[NUMTERRAINSHADERS];
};

GLvoid RenderWorldHeightmap(CShaderProgram* spTerrain);