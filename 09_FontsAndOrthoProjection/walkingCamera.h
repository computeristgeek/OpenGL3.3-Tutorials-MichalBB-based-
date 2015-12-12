#pragma once

/********************************

Class:	CWalkingCamera

Purpose:	Camera that can walk
			around the scene.

********************************/

class CWalkingCamera
{
public:
	glm::mat4 look();
	GLvoid update();
	GLfloat getfSpeed();

	GLvoid rotateViewY(GLfloat fAngle);
	GLvoid move(GLfloat fBy);

	CWalkingCamera();
	CWalkingCamera(glm::vec3 a_vEye, glm::vec3 a_vView, glm::vec3 a_vUp, GLfloat a_fSpeed);

private:
	glm::vec3 vEye, vView, vUp;
	GLfloat fSpeed;
};
