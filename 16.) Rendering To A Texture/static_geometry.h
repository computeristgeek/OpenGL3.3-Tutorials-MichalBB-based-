#pragma once

#include "vertexBufferObject.h"

extern glm::vec3 vCubeVertices[36];
extern glm::vec2 vCubeTexCoords[6];
extern glm::vec3 vCubeNormals[6];
extern glm::vec3 vGround[6];

extern glm::vec3 vBuildingNormals[4];
extern glm::vec3 vBuilding[24];

extern glm::vec3 vCinema[4];
extern glm::vec2 vCinemaCoords[4]; 

int GenerateTorus(CVertexBufferObject &vboDest, float fRadius, float fTubeRadius, int iSubDivAround, int iSubDivTube);