#pragma once

#include "vertexBufferObject.h"

GLint GenerateTorus(CVertexBufferObject &vboDest, GLfloat fRadius, GLfloat fTubeRadius, GLint iSubDivAround, GLint iSubDivTube);
GLint GenerateCylinder(CVertexBufferObject &vboDest, GLfloat fRadius, GLfloat fHeight, GLint iSubDivAround, GLfloat fMapU = 1.0f, GLfloat fMapV = 1.0f);
GLvoid CreateStaticSceneObjects(UINT* uiVAO, CVertexBufferObject& vboDest);

extern GLint iSphereFaces;