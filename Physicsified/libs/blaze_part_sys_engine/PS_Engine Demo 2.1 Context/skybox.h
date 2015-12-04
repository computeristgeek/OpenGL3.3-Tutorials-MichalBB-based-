#pragma once

#include "texture.h"


/********************************

Class:	CSkybox

Purpose: Class for using skybox.

********************************/

class CSkybox
{
public:
	void loadSkybox(string a_sDirectory, string a_sFront, string a_sBack, string a_sLeft, string a_sRight, string a_sTop, string a_sBottom);
	void renderSkybox(glm::vec3 vPos);

	void releaseSkybox();
private:
	CTexture tTextures[6];
	string sDirectory;
	string sFront, sBack, sLeft, sRight, sTop, sBottom;
};