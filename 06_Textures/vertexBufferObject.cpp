#include "common_header.h"

#include "vertexBufferObject.h"

CVertexBufferObject::CVertexBufferObject()
{
	bDataUploaded = false;
}

/*-----------------------------------------------

Name:		createVBO

Params:	a_iSize - initial size of buffer

Result:	Creates vertex buffer object.

/*---------------------------------------------*/

GLvoid CVertexBufferObject::createVBO(GLint a_iSize)
{
	glGenBuffers(1, &uiBuffer);
	data.reserve(a_iSize);
	iSize = a_iSize;
}

/*-----------------------------------------------

Name:		releaseVBO

Params:	none

Result:	Releases VBO and frees all memory.

/*---------------------------------------------*/

GLvoid CVertexBufferObject::releaseVBO()
{
	glDeleteBuffers(1, &uiBuffer);
	bDataUploaded = false;
	data.clear();
}

/*-----------------------------------------------

Name:		mapBufferToMemory

Params:	iUsageHGLint - GL_READ_ONLY, GL_WRITE_ONLY...

Result:	Maps whole buffer data to memory and
			returns pointer to data.

/*---------------------------------------------*/

void* CVertexBufferObject::mapBufferToMemory(GLint iUsageHint)
{
	if(!bDataUploaded)return NULL;
	void* ptrRes = glMapBuffer(iBufferType, iUsageHint);
	return ptrRes;
}

/*-----------------------------------------------

Name:		mapSubBufferToMemory

Params:	iUsageHGLint - GL_READ_ONLY, GL_WRITE_ONLY...
			uiOffset - data offset (from where should
							data be mapped).
			uiLength - length of data

Result:	Maps specified part of buffer to memory.

/*---------------------------------------------*/

void* CVertexBufferObject::mapSubBufferToMemory(GLint iUsageHint, GLuint uiOffset, GLuint uiLength)
{
	if(!bDataUploaded)return NULL;
	void* ptrRes = glMapBufferRange(iBufferType, uiOffset, uiLength, iUsageHint);
	return ptrRes;
}

/*-----------------------------------------------

Name:		unmapBuffer

Params:	none

Result:	Unmaps previously mapped buffer.

/*---------------------------------------------*/

GLvoid CVertexBufferObject::unmapBuffer()
{
	glUnmapBuffer(iBufferType);
}

/*-----------------------------------------------

Name:		bindVBO

Params:	a_iBufferType - buffer type (GL_ARRAY_BUFFER, ...)

Result:	Binds this VBO.

/*---------------------------------------------*/

GLvoid CVertexBufferObject::bindVBO(GLint a_iBufferType)
{
	iBufferType = a_iBufferType;
	glBindBuffer(iBufferType, uiBuffer);
}

/*-----------------------------------------------

Name:		uploadDataToGPU

Params:	iUsageHGLint - GL_STATIC_DRAW, GL_DYNAMIC_DRAW...

Result:	Sends data to GPU.

/*---------------------------------------------*/

GLvoid CVertexBufferObject::uploadDataToGPU(GLint iDrawingHint)
{
	glBufferData(iBufferType, data.size(), &data[0], iDrawingHint);
	bDataUploaded = true;
	data.clear();
}

/*-----------------------------------------------

Name:		addData

Params:	ptrData - pointer to arbitrary data
			uiDataSize - data size in bytes

Result:	Adds arbitrary data to VBO.

/*---------------------------------------------*/

GLvoid CVertexBufferObject::addData(void* ptrData, GLuint uiDataSize)
{
	data.insert(data.end(), (BYTE*)ptrData, (BYTE*)ptrData+uiDataSize);
}

/*-----------------------------------------------

Name:		getDataPointer

Params:	none

Result:	Returns data pointer (only before uplading).

/*---------------------------------------------*/

void* CVertexBufferObject::getDataPointer()
{
	if(bDataUploaded)return NULL;
	return (void*)data[0];
}

/*-----------------------------------------------

Name:		getBuffer

Params:	none

Result:	Returns VBO ID.

/*---------------------------------------------*/

GLuint CVertexBufferObject::getBuffer()
{
	return uiBuffer;
}


