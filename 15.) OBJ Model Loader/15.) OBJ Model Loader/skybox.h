#pragma once

#include "texture.h"
#include "vertexBufferObject.h"

/********************************

Class:	CSkybox

Purpose: Class for using skybox.

********************************/

class CSkybox
{
public:
	void loadSkybox(string a_sDirectory, string a_sFront, string a_sBack, string a_sLeft, string a_sRight, string a_sTop, string a_sBottom);
	void renderSkybox();

	void releaseSkybox();
private:
	UINT uiVAO;
	CVertexBufferObject vboRenderData;
	CTexture tTextures[6];
	string sDirectory;
	string sFront, sBack, sLeft, sRight, sTop, sBottom;
};