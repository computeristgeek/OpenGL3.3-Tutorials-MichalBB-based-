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
	void* MapSubBufferToMemory(GLint iUsageHint, UINT uiOffset, UINT uiLength);
	GLvoid UnmapBuffer();

	GLvoid BindVBO(GLint a_iBufferType = GL_ARRAY_BUFFER);
	GLvoid UploadDataToGPU(GLint iUsageHint);
	
	GLvoid AddData(void* ptrData, UINT uiDataSize);

	void* GetDataPointer();
	UINT GetBufferID();

	CVertexBufferObject();

private:
	UINT uiBuffer;
	GLint iSize;
	GLint iBufferType;
	vector<BYTE> data;

	GLboolean bDataUploaded;
};