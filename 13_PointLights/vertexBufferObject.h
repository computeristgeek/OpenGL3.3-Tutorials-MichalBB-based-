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
	void* mapSubBufferToMemory(GLint iUsageHint, GLuint uiOffset, GLuint uiLength);
	GLvoid unmapBuffer();

	GLvoid bindVBO(GLint a_iBufferType = GL_ARRAY_BUFFER);
	GLvoid uploadDataToGPU(GLint iUsageHint);
	
	GLvoid addData(void* ptrData, GLuint uiDataSize);

	void* getDataPointer();
	GLuint getBuffer();

	CVertexBufferObject();

private:
	GLuint uiBuffer;
	GLint iSize;
	GLint iBufferType;
	vector<uint8_t> data;

	GLboolean bDataUploaded;
};
