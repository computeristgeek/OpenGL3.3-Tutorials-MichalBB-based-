#include "common_header.h"

#include "win_OpenGLApp.h"
#include "picking.h"

#include "flyingCamera.h"

/*-----------------------------------------------

Name:    GetColorByIndex

Params:  index - index of color you want to generate

Result:  Returns i-th RGB color.

/*---------------------------------------------*/

glm::vec4 GetColorByIndex(GLint index)
{
	GLint r = index&0xFF;
	GLint g = (index>>8)&0xFF;
	GLint b = (index>>16)&0xFF;

	return glm::vec4(float(r)/255.0f, float(g)/255.0f, float(b)/255.0f, 1.0f);
}

/*-----------------------------------------------

Name:    GetIndexByColor

Params:  r, g, b - RGB values of color

Result:  Kind of inverse to previous function,
		 gets index from selected color.

/*---------------------------------------------*/

GLint GetIndexByColor(GLint r, GLint g, GLint b)
{
	return (r)|(g<<8)|(b<<16);
}

/*-----------------------------------------------

Name:    GetColorByIndex

Params:  index - index of color you want to generate

Result:  Returns i-th RGB color.

/*---------------------------------------------*/

GLint GetPickedColorIndexUnderMouse()
{
	glm::vec2 mp; GetCursorPos(&mp);
	ScreenToClient(appMain.hWnd, &mp);
	RECT rect; GetClientRect(appMain.hWnd, &rect);
	mp.y = rect.bottom-mp.y;
	GL_UNSIGNED_BYTE bArray[4];
	glReadPixels(mp.x, mp.y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, bArray);
	GLint iResult = GetIndexByColor(bArray[0], bArray[1], bArray[2]) - 1;

	return iResult;
}

#include <glm/gtc/matrix_transform.hpp>

/*-----------------------------------------------

Name:    Get3DRayUnderMouse

Params:  v1, v2 - results storage

Result:  Retrieves 3D ray under cursor from near
		 to far plane.

/*---------------------------------------------*/

GLvoid Get3DRayUnderMouse(glm::vec3* v1, glm::vec3* v2)
{
	glm::vec2 mp; GetCursorPos(&mp);
	ScreenToClient(appMain.hWnd, &mp);
	RECT rect; GetClientRect(appMain.hWnd, &rect);
	mp.y = rect.bottom-mp.y;

	glm::vec4 viewport = glm::vec4(0.0f, 0.0f, appMain.oglControl.GetViewportWidth(), appMain.oglControl.GetViewportHeight());

	*v1 = glm::unProject(glm::vec3(float(mp.x), float(mp.y), 0.0f), cCamera.Look(), *appMain.oglControl.GetProjectionMatrix(), viewport);
	*v2 = glm::unProject(glm::vec3(float(mp.x), float(mp.y), 1.0f), cCamera.Look(), *appMain.oglControl.GetProjectionMatrix(), viewport);
}

/*-----------------------------------------------

Name:    RaySphereCollision

Params:  vSphereCenter - guess what it is
	     fSphereRadius - guess what it is
		 vA, vB - two points of ray

Result:  Checks if a ray given by two points
		 collides with sphere.

/*---------------------------------------------*/

GLboolean RaySphereCollision(glm::vec3 vSphereCenter, GLfloat fSphereRadius, glm::vec3 vA, glm::vec3 vB)
{
	// Create the vector from end poGLint vA to center of sphere
	glm::vec3 vDirToSphere = vSphereCenter - vA;

	// Create a normalized direction vector from end poGLint vA to end poGLint vB
	glm::vec3 vLineDir = glm::normalize(vB-vA);

	// Find length of line segment
	GLfloat fLineLength = glm::distance(vA, vB);

	// Using the dot product, we project the vDirToSphere onto the vector vLineDir
	GLfloat t = glm::dot(vDirToSphere, vLineDir);

	glm::vec3 vClosestPoint;
	// If our projected distance from vA is less than or equal to 0, the closest poGLint is vA
	if (t <= 0.0f)
		vClosestPoGLint = vA;
	// If our projected distance from vA is greater thatn line length, closest poGLint is vB
	else if (t >= fLineLength)
		vClosestPoGLint = vB;
	// Otherwise calculate the poGLint on the line using t and return it
	else
		vClosestPoGLint = vA+vLineDir*t;

	// Now just check if closest poGLint is within radius of sphere
	return glm::distance(vSphereCenter, vClosestPoint) <= fSphereRadius;
}