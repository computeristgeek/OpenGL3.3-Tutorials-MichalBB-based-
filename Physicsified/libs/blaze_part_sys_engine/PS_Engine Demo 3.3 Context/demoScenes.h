#pragma once

#include "skybox.h"

#include "../include/blaze_part_sys_engine.h"
#include "../include/blaze_effects.h"

#define NUMTEXTURES 4
#define NUMSHADERS 7
#define NUMVAOS 3
#define NUMSKYBOXES 3

extern BlazeParticleSystemEngine* psEngine;

extern UINT uiVAOs[NUMVAOS];
extern CSkybox sbSkyboxes[NUMSKYBOXES];

extern CTexture tFireTexture, tParticleTexture;
extern CTexture tWorldTextures[NUMTEXTURES];

extern CShaderProgram spDirectionalLight, spOrtho2D, spFont2D, spColor;

void prepareFireDemoScene();
void renderFireDemoScene(COpenGLControl* oglControl);

void prepareSpaceDemoScene();
void renderSpaceDemoScene(COpenGLControl* oglControl);

void prepareFountainDemoScene();
void renderFountainDemoScene(COpenGLControl* oglControl);

void generateWalls(CVertexBufferObject& stor, glm::vec3 vCorner, glm::vec3 vDir, float fLength, float fWidth, float fHeight);

extern glm::vec3 vCubeVertices[36];