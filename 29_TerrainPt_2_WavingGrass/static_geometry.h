#pragma once

#include "vertexBufferObject.h"

GLint GenerateTorus(CVertexBufferObject &vboDest, GLfloat fRadius, GLfloat fTubeRadius, GLint iSubDivAround, GLint iSubDivTube);

extern GLuint uiVAOSceneObjects;
extern GLuint uiVAOOccluders;
extern CVertexBufferObject vboSceneObjects;
extern CVertexBufferObject vboOccluders;

extern GLfloat fCubeHalfSize;

GLvoid PrepareStaticSceneObjects();