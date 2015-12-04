#pragma once

#define NUMTEXTURES 6
#define NUMSKYBOXES 3

#include "../include/blaze_part_sys_engine.h"
#include "../include/blaze_effects.h"

#include "skybox.h"

extern BlazeParticleSystemEngine* psEngine;
extern CSkybox sbSkyboxes[NUMSKYBOXES];

extern CTexture tFireTexture, tParticleTexture;
extern CTexture tWorldTextures[NUMTEXTURES];

void prepareFireDemoScene();
void renderFireDemoScene();

void prepareSpaceDemoScene();
void renderSpaceDemoScene();

void prepareFountainDemoScene();
void renderFountainDemoScene();

void renderWall(glm::vec3 vCorner, glm::vec3 vDir, float fLength, float fWidth, float fHeight, UINT uiTex);