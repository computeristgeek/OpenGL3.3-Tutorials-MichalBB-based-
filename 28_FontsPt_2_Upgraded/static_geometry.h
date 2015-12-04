#pragma once

#include "vertexBufferObject.h"

GLint GenerateTorus(CVertexBufferObject &vboDest, GLfloat fRadius, GLfloat fTubeRadius, GLint iSubDivAround, GLint iSubDivTube);

extern UINT uiVAOSceneObjects;
extern UINT uiVAOOccluders;
extern CVertexBufferObject vboSceneObjects;
extern CVertexBufferObject vboOccluders;

extern GLfloat fCubeHalfSize;

GLvoid PrepareStaticSceneObjects();