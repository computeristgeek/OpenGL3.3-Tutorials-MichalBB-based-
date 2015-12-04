#include "common_header.h"

#include "skybox.h"

/*-----------------------------------------------

Name:	loadSkybox

Params:	paths of skybox pictures

Result:	Loads skybox and creates VAO and VBO for it.

/*---------------------------------------------*/

void CSkybox::loadSkybox(string a_sDirectory, string a_sFront, string a_sBack, string a_sLeft, string a_sRight, string a_sTop, string a_sBottom)
{
	tTextures[0].loadTexture2D(a_sDirectory+a_sFront);
	tTextures[1].loadTexture2D(a_sDirectory+a_sBack);
	tTextures[2].loadTexture2D(a_sDirectory+a_sLeft);
	tTextures[3].loadTexture2D(a_sDirectory+a_sRight);
	tTextures[4].loadTexture2D(a_sDirectory+a_sTop);
	tTextures[5].loadTexture2D(a_sDirectory+a_sBottom);

	sDirectory = a_sDirectory;

	sFront = a_sFront;
	sBack = a_sBack;
	sLeft = a_sLeft;
	sRight = a_sRight;
	sTop = a_sTop;
	sBottom = a_sBottom;

	FOR(i, 6)
	{
		tTextures[i].setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR);
		tTextures[i].setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		tTextures[i].setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

}

/*-----------------------------------------------

Name:	renderSkybox

Params:	none

Result: Guess what it does :)

/*---------------------------------------------*/

void CSkybox::renderSkybox(glm::vec3 vPos)
{
	glColor3ub(255, 255, 255);
	glDepthMask(0);
	glPushMatrix();
	glTranslatef(vPos.x, vPos.y, vPos.z);

	tTextures[0].bindTexture();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-20.0f, 20.0f, -20.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(20.0f, 20.0f, -20.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(20.0f, -20.0f, -20.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-20.0f, -20.0f, -20.0f);
	glEnd();

	tTextures[1].bindTexture();
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-20.0f, 20.0f, 20.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(20.0f, 20.0f, 20.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(20.0f, -20.0f, 20.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-20.0f, -20.0f, 20.0f);
	glEnd();

	tTextures[2].bindTexture();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(20.0f, 20.0f, -20.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(20.0f, 20.0f, 20.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(20.0f, -20.0f, 20.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(20.0f, -20.0f, -20.0f);
	glEnd();

	tTextures[3].bindTexture();
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-20.0f, 20.0f, -20.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-20.0f, 20.0f, 20.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-20.0f, -20.0f, 20.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-20.0f, -20.0f, -20.0f);
	glEnd();

	tTextures[4].bindTexture();
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-20.0f, 20.0f, -20.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(20.0f, 20.0f, -20.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(20.0f, 20.0f, 20.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-20.0f, 20.0f, 20.0f);
	glEnd();

	tTextures[5].bindTexture();
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-20.0f, -20.0f, -20.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(20.0f, -20.0f, -20.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(20.0f, -20.0f, 20.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-20.0f, -20.0f, 20.0f);
	glEnd();

	glPopMatrix();
	glDepthMask(1);
}

/*-----------------------------------------------

Name:	loadSkybox

Params:	paths of skybox pictures

Result:	Loads skybox and creates VAO and VBO for it.

/*---------------------------------------------*/

void CSkybox::releaseSkybox()
{
	FOR(i, 6)tTextures[i].releaseTexture();
}