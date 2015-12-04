#pragma once

class CFlyingCamera
{
public:
	glm::vec3 vEye, vView, vUp;
	GLfloat fSpeed;
	GLfloat fSensitivity; // How many degrees to rotate per pixel moved by mouse (nice value is 0.10)

	// Main functions
	GLvoid rotateWithMouse();
	GLvoid update();
	glm::mat4 look();

	GLvoid setMovingKeys(GLint a_iForw, GLint a_iBack, GLint a_iLeft, GLint a_iRight);
	GLvoid resetMouse();

	// Functions that get viewing angles
	GLfloat getAngleX(), getAngleY();

	// Constructors
	CFlyingCamera();
	CFlyingCamera(glm::vec3 a_vEye, glm::vec3 a_vView, glm::vec3 a_vUp, GLfloat a_fSpeed, GLfloat a_fSensitivity);

private:
	POINT pCur; // For mosue rotation
	GLint iForw, iBack, iLeft, iRight;
};
