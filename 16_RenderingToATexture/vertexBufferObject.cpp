#include "common_header.h"

#include "vertexBufferObject.h"

CVertexBufferObject::CVertexBufferObject()
{
	bDataUploaded = false;
}

/*-----------------------------------------------

Name:	CreateVBO

Params:	a_iSize - initial size of buffer

Result:	Creates vertex buffer object.

/*---------------------------------------------*/

GLvoid CVertexBufferObject::CreateVBO(GLint a_iSize)
{
	glGenBuffers(1, &uiBuffer);
	data.reserve(a_iSize);
	iSize = a_iSize;
}

/*-----------------------------------------------

Name:	DeleteVBO

Params:	none

Result:	Releases VBO and frees all memory.

/*---------------------------------------------*/

GLvoid CVertexBufferObject::DeleteVBO()
{
	glDeleteBuffers(1, &uiBuffer);
	bDataUploaded = false;
	data.clear();
}

/*-----------------------------------------------

Name:	mapBufferToMemory

Params:	iUsageHGLint - GL_READ_ONLY, GL_WRITE_ONLY...

Result:	Maps whole buffer data to memory and
		returns pointer to data.

/*---------------------------------------------*/

void* CVertexBufferObject::MapBufferToMemory(GLint iUsageHint)
{
	if(!bDataUploaded)return NULL;
	void* ptrRes = glMapBuffer(iBufferType, iUsageHint);
	return ptrRes;
}

/*-----------------------------------------------

Name:	MapSubBufferToMemory

Params:	iUsageHGLint - GL_READ_ONLY, GL_WRITE_ONLY...
		uiOffset - data offset (from where should
					data be mapped).
		uiLength - length of data

Result:	Maps specified part of buffer to memory.

/*---------------------------------------------*/

void* CVertexBufferObject::MapSubBufferToMemory(GLint iUsageHint, GLuint uiOffset, GLuint uiLength)
{
	if(!bDataUploaded)return NULL;
	void* ptrRes = glMapBufferRange(iBufferType, uiOffset, uiLength, iUsageHint);
	return ptrRes;
}

/*-----------------------------------------------

Name:	UnmapBuffer

Params:	none

Result:	Unmaps previously mapped buffer.

/*---------------------------------------------*/

GLvoid CVertexBufferObject::UnmapBuffer()
{
	glUnmapBuffer(iBufferType);
}

/*-----------------------------------------------

Name:	BindVBO

Params:	a_iBufferType - buffer type (GL_ARRAY_BUFFER, ...)

Result:	Binds this VBO.

/*---------------------------------------------*/

GLvoid CVertexBufferObject::BindVBO(GLint a_iBufferType)
{
	iBufferType = a_iBufferType;
	glBindBuffer(iBufferType, uiBuffer);
}

/*-----------------------------------------------

Name:	UploadDataToGPU

Params:	iUsageHGLint - GL_STATIC_DRAW, GL_DYNAMIC_DRAW...

Result:	Sends data to GPU.

/*---------------------------------------------*/

GLvoid CVertexBufferObject::UploadDataToGPU(GLint iDrawingHint)
{
	glBufferData(iBufferType, data.size(), &data[0], iDrawingHint);
	bDataUploaded = true;
	data.clear();
}

/*-----------------------------------------------

Name:	AddData

Params:	ptrData - pointer to arbitrary data
		uiDataSize - data size in bytes

Result:	Adds arbitrary data to VBO.

/*---------------------------------------------*/

GLvoid CVertexBufferObject::AddData(void* ptrData, GLuint uiDataSize)
{
	data.insert(data.end(), (BYTE*)ptrData, (BYTE*)ptrData+uiDataSize);
}

/*-----------------------------------------------

Name:	GetDataPointer

Params:	none

Result:	Returns data pointer (only before uploading).

/*---------------------------------------------*/

void* CVertexBufferObject::GetDataPointer()
{
	if(bDataUploaded)return NULL;
	return (void*)data[0];
}

/*-----------------------------------------------

Name:	GetBufferID

Params:	none

Result:	Returns VBO ID.

/*---------------------------------------------*/

GLuint CVertexBufferObject::GetBufferID()
{
	return uiBuffer;
}


