#include "common_header.h"

#include "Lin_OpenGLApp.h"
#include "flyingCamera.h"

#include <glm/gtx/rotate_vector.hpp>

const GLfloat PI = atan(1.0)*4.0;

CFlyingCamera::CFlyingCamera()
{
	vEye = glm::vec3(0.0f, 0.0f, 0.0f);
	vView = glm::vec3(0.0f, 0.0, -1.0f);
	vUp = glm::vec3(0.0f, 1.0f, 0.0f);
	fSpeed = 25.0f;
	fSensitivity = 0.1f;
}

CFlyingCamera::CFlyingCamera(glm::vec3 a_vEye, glm::vec3 a_vView, glm::vec3 a_vUp, GLfloat a_fSpeed, GLfloat a_fSensitivity)
{
	vEye = a_vEye; vView = a_vView; vUp = a_vUp;
	fSpeed = a_fSpeed;
	fSensitivity = a_fSensitivity;
}

/*-----------------------------------------------

Name:	rotateWithMouse

Params:	none

Result:	Checks for moving of mouse and rotates
		camera.

/*---------------------------------------------*/

GLvoid CFlyingCamera::rotateWithMouse()
{
	glfwGetCursorPos(appMain.hWnd, &pCur[0],&pCur[1]);
	GLint width, height;
	glfwGetFramebufferSize(appMain.hWnd,&width,&height);
	GLint iCentX = width/2,
		iCentY = height/2;

	GLfloat deltaX = (float)(iCentX-pCur.x)*fSensitivity;
	GLfloat deltaY = (float)(iCentY-pCur.y)*fSensitivity;

	if(deltaX != 0.0f)
	{
		vView -= vEye;
		vView = glm::rotate(vView, deltaX, glm::vec3(0.0f, 1.0f, 0.0f));
		vView += vEye;
	}
	if(deltaY != 0.0f)
	{
		glm::vec3 vAxis = glm::cross(vView-vEye, vUp);
		vAxis = glm::normalize(vAxis);
		GLfloat fAngle = deltaY;
		GLfloat fNewAngle = fAngle+getAngleX();
		if(fNewAngle > -89.80f && fNewAngle < 89.80f)
		{
			vView -= vEye;
			vView = glm::rotate(vView, deltaY, vAxis);
			vView += vEye;
		}
	}
	glfwSetCursorPos(appMain.hWnd, iCentX, iCentY);
}

/*-----------------------------------------------

Name:	getAngleY

Params:	none

Result:	Gets Y angle of camera (head turning left
		and right).

/*---------------------------------------------*/

GLfloat CFlyingCamera::getAngleY()
{
	glm::vec3 vDir = vView-vEye; vDir.y = 0.0f;
	glm::normalize(vDir);
	GLfloat fAngle = acos(glm::dot(glm::vec3(0, 0, -1), vDir))*(180.0f/PI);
	if(vDir.x < 0)fAngle = 360.0f-fAngle;
	return fAngle;
}

/*-----------------------------------------------

Name:	getAngleX

Params:	none

Result:	Gets X angle of camera (head turning up
		and down).

/*---------------------------------------------*/

GLfloat CFlyingCamera::getAngleX()
{
	glm::vec3 vDir = vView-vEye;
	vDir = glm::normalize(vDir);
	glm::vec3 vDir2 = vDir; vDir2.y = 0.0f;
	vDir2 = glm::normalize(vDir2);
	GLfloat fAngle =  acos(glm::dot(vDir2, vDir))*(180.0f/PI);
	if(vDir.y < 0)fAngle *= -1.0f;
	return fAngle;
}

/*-----------------------------------------------

Name:	setMovingKeys

Params:	a_iForw - move forward key
		a_iBack - move backward key
		a_iLeft - strafe left key
		a_iRight - strafe right key

Result:	Sets keys for moving camera.

/*---------------------------------------------*/

GLvoid CFlyingCamera::setMovingKeys(GLint a_iForw, GLint a_iBack, GLint a_iLeft, GLint a_iRight)
{
	iForw = a_iForw;
	iBack = a_iBack;
	iLeft = a_iLeft;
	iRight = a_iRight;
}

/*-----------------------------------------------

Name:	update

Params:	none

Result:	Performs updates of camera - moving and
		rotating.

/*---------------------------------------------*/

GLvoid CFlyingCamera::update()
{
	rotateWithMouse();

////////// Get view direction
////////glm::vec3 vMove = vView-vEye;
////////vMove = glm::normalize(vMove);
////////vMove *= fSpeed;

////////glm::vec3 vStrafe = glm::cross(vView-vEye, vUp);
////////vStrafe = glm::normalize(vStrafe);
////////vStrafe *= fSpeed;

////////GLint iMove = 0;
////////glm::vec3 vMoveBy;
////////// Get vector of move
////////if(Keys::key(iForw))vMoveBy += vMove*appMain.sof(1.0f);
////////if(Keys::key(iBack))vMoveBy -= vMove*appMain.sof(1.0f);
////////if(Keys::key(iLeft))vMoveBy -= vStrafe*appMain.sof(1.0f);
////////if(Keys::key(iRight))vMoveBy += vStrafe*appMain.sof(1.0f);
////////vEye += vMoveBy; vView += vMoveBy;
}

/*-----------------------------------------------

Name:	resetMouse

Params:	none

Result:	Sets mouse cursor back to the center of
		window.

/*---------------------------------------------*/

GLvoid CFlyingCamera::resetMouse()
{
	GLint width, height;
	glfwGetFramebufferSize(appMain.hWnd,&width,&height);
	glfwSetCursorPos(appMain.hWnd, 0, 0);
}

/*-----------------------------------------------

Name:	look

Params:	none

Result:	Returns proper modelview matrix to make
		camera look.

/*---------------------------------------------*/

glm::mat4 CFlyingCamera::look()
{
	return glm::lookAt(vEye, vView, vUp);
}
