#pragma once

#include "vertexBufferObject.h"

int GenerateTorus(CVertexBufferObject &vboDest, float fRadius, float fTubeRadius, int iSubDivAround, int iSubDivTube);

extern UINT uiVAOSceneObjects;
extern UINT uiVAOOccluders;
extern CVertexBufferObject vboSceneObjects;
extern CVertexBufferObject vboOccluders;
extern int iSphereFaces;

extern float fCubeHalfSize;

void PrepareStaticSceneObjects();