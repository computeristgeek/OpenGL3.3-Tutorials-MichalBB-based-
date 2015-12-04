#pragma once

#include "texture.h"
#include "shaders.h"

/********************************

Class:		CFramebuffer

Purpose:	Manages framebuffer objects.

********************************/

class CFramebuffer
{
public:

	GLboolean CreateFrameBufferForDepthShadow();
	

	GLboolean CreateFramebufferWithTexture(GLint a_iWidth, GLint a_iHeight);

	GLboolean AddDepthBuffer();
	GLvoid BindFramebuffer(GLboolean bSetFullViewport = true);
	GLvoid BindFramebufferShadowMap();
	
	GLvoid SetFramebufferTextureFiltering(GLint a_tfMagnification, GLint a_tfMinification);
	GLvoid BindFramebufferTexture(GLint iTextureUnit = 0, GLboolean bRegenMipMaps = false);

	glm::mat4 CalculateProjectionMatrix(GLfloat fFOV, GLfloat fNear, GLfloat fFar);
	glm::mat4 CalculateOrthoMatrix();

	GLvoid DeleteFramebuffer();

	GLint GetWidth();
	GLint GetHeight();

	CFramebuffer();
private:
	GLint iWidth, iHeight;
	UINT uiFramebuffer;
	UINT uiDepthRenderbuffer;
	CTexture tFramebufferTex;
	UINT depthTexture;
};
