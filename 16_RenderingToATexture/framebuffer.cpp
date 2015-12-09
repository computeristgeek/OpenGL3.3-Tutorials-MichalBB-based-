#include "common_header.h"

#include "framebuffer.h"

#include <glm/gtc/matrix_transform.hpp>

CFramebuffer::CFramebuffer()
{
	uiFramebuffer = 0;
	uiDepthRenderbuffer = 0;
}

/*-----------------------------------------------

Name:	CreateFramebufferWithTexture

Params:	a_iWidth - framebuffer width
		a_iHeight - framebuffer height

Result:	Creates a framebuffer and a texture to
		render to.

/*---------------------------------------------*/

GLboolean CFramebuffer::CreateFramebufferWithTexture(GLint a_iWidth, GLint a_iHeight)
{
	if(uiFramebuffer != 0)return false;

	glGenFramebuffers(1, &uiFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, uiFramebuffer);

	tFramebufferTex.CreateEmptyTexture(a_iWidth, a_iHeight, 24, GL_RGB);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tFramebufferTex.GetTextureID(), 0);

	iWidth = a_iWidth;
	iHeight = a_iHeight;

	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

/*-----------------------------------------------

Name:	AddDepthBuffer

Params:	none

Result:	Adds depth renderbuffer to framebuffer,
		so rendering can perform depth testing.

/*---------------------------------------------*/

GLboolean CFramebuffer::AddDepthBuffer()
{
	if(uiFramebuffer == 0)return false;

	glBindFramebuffer(GL_FRAMEBUFFER, uiFramebuffer);

	glGenRenderbuffers(1, &uiDepthRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, uiDepthRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, iWidth, iHeight);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, uiDepthRenderbuffer);

	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

/*-----------------------------------------------

Name:	BindFramebuffer

Params:	bSetFullViewport - set full framebuffer
		viewport, default is true

Result:	Binds this framebuffer.

/*---------------------------------------------*/

GLvoid CFramebuffer::BindFramebuffer(GLboolean bSetFullViewport)
{
	glBindFramebuffer(GL_FRAMEBUFFER, uiFramebuffer);
	if(bSetFullViewport)glViewport(0, 0, iWidth, iHeight);
}

/*-----------------------------------------------

Name:	BindFramebufferTexture

Params:	iTextureUnit - texture unit to bind to (defaul 0)
		bRegenMipMaps - regenerate mipmaps? (default false)

Result:	Binds framebuffer texture, where renderings
		are stored.

/*---------------------------------------------*/

GLvoid CFramebuffer::BindFramebufferTexture(GLint iTextureUnit, GLboolean bRegenMipMaps)
{
	tFramebufferTex.BindTexture(iTextureUnit);
	if(bRegenMipMaps)glGenerateMipmap(GL_TEXTURE_2D);
}

/*-----------------------------------------------

Name:	SetFramebufferTextureFiltering

Params:	a_tfMagnification - magnification filter
		a_tfMinification - miniification filter

Result:	Sets filtering of framebuffer texture,
		poor texturing here results in greater
		performance.

/*---------------------------------------------*/

GLvoid CFramebuffer::SetFramebufferTextureFiltering(GLint a_tfMagnification, GLint a_tfMinification)
{
	tFramebufferTex.SetFiltering(a_tfMagnification, a_tfMinification);
}

/*-----------------------------------------------

Name:	DeleteFramebuffer

Params:	none

Result:	Deletes framebuffer and frees memory.

/*---------------------------------------------*/

GLvoid CFramebuffer::DeleteFramebuffer()
{
	if(uiFramebuffer)
	{
		glDeleteFramebuffers(1, &uiFramebuffer);
		uiFramebuffer = 0;
	}
	if(uiDepthRenderbuffer)
	{
		glDeleteRenderbuffers(1, &uiDepthRenderbuffer);
		uiDepthRenderbuffer = 0;
	}
	tFramebufferTex.DeleteTexture();
}

/*-----------------------------------------------

Name:	CalculateProjectionMatrix

Params:	fFOV - field of view angle
		fNear, fFar - distance of near and far clipping plane

Result:	Calculates projection matrix.

/*---------------------------------------------*/

glm::mat4 CFramebuffer::CalculateProjectionMatrix(GLfloat fFOV, GLfloat fNear, GLfloat fFar)
{
	return glm::perspective(fFOV, float(iWidth)/float(iHeight), fNear, fFar);
}

/*-----------------------------------------------

Name:	CalculateOrthoMatrix

Params:	none

Result:	Calculates ortho 2D projection matrix.

/*---------------------------------------------*/

glm::mat4 CFramebuffer::CalculateOrthoMatrix()
{
	return glm::ortho(0.0f, float(iWidth), 0.0f, float(iHeight));
}

/*-----------------------------------------------

Name:	Getters

Params:	none

Result:	... They get something :D

/*---------------------------------------------*/

GLint CFramebuffer::GetWidth()
{
	return iWidth;
}

GLint CFramebuffer::GetHeight()
{
	return iHeight;
}