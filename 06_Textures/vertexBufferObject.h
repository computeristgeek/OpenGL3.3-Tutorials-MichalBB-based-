#pragma once

/********************************

Class:	CVertexBufferObject

Purpose:	Wraps OpenGL vertex buffer
			object.

********************************/

class CVertexBufferObject
{
public:
	GLvoid createVBO(GLint a_iSize = 0);
	GLvoid releaseVBO();

	void* mapBufferToMemory(GLint iUsageHint);
	void* mapSubBufferToMemory(GLint iUsageHint, UINT uiOffset, UINT uiLength);
	GLvoid unmapBuffer();

	GLvoid bindVBO(GLint a_iBufferType = GL_ARRAY_BUFFER);
	GLvoid uploadDataToGPU(GLint iUsageHint);
	
	GLvoid addData(void* ptrData, UINT uiDataSize);

	void* getDataPointer();
	UINT getBuffer();

	CVertexBufferObject();

private:
	UINT uiBuffer;
	GLint iSize;
	GLint iBufferType;
	vector<BYTE> data;

	GLboolean bDataUploaded;
};