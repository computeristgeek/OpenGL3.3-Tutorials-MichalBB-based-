#pragma once

#include "vertexBufferObject.h"

extern glm::vec3 vCubeVertices[36];
extern glm::vec2 vCubeTexCoords[6];
extern glm::vec3 vCubeNormals[6];
extern glm::vec3 vGround[6];

extern glm::vec3 vBuildingNormals[4];
extern glm::vec3 vBuilding[24];

GLint generateTorus(CVertexBufferObject &vboDest, GLfloat fRadius, GLfloat fTubeRadius, GLint iSubDivAround, GLint iSubDivTube);