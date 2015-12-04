#pragma once

/********************************

Class:		CVertexBufferObject

Purpose:	Wraps OpenGL vertex buffer
			object.

********************************/

class CVertexBufferObject
{
public:
	GLvoid CreateVBO(GLint a_iSize = 0);
	GLvoid DeleteVBO();

	void* MapBufferToMemory(GLint iUsageHint);
	void* MapSubBufferToMemory(GLint iUsageHint, GLuint uiOffset, GLuint uiLength);
	GLvoid UnmapBuffer();

	GLvoid BindVBO(GLint a_iBufferType = GL_ARRAY_BUFFER);
	GLvoid UploadDataToGPU(GLint iUsageHint);
	
	GLvoid AddData(void* ptrData, GLuint uiDataSize);

	void* GetDataPointer();
	GLuint GetBufferID();

	GLint GetCurrentSize();

	CVertexBufferObject();

private:
	GLuint uiBuffer;
	GLint iSize;
	GLint iCurrentSize;
	GLint iBufferType;
	vector<BYTE> data;

	GLboolean bDataUploaded;
};