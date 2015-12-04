#ifndef _CAMERA_H
#define _CAMERA_H

#define kSpeed	0.2f
extern GLfloat angleY, angleZ;
// This is our camera class
class CCamera {

public:

	// Our camera constructor
	CCamera();	


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

	// These are are data access functions for our camera's private data
	CVector3 Position() {	return m_vPosition;		}
	CVector3 View()		{	return m_vView;			}
	CVector3 UpVector() {	return m_vUpVector;		}
	CVector3 Strafe()	{	return m_vStrafe;		}

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
	
	// This changes the position, view, and up vector of the camera.
	// This is primarily used for initialization
	GLvoid PositionCamera(GLfloat positionX, GLfloat positionY, GLfloat positionZ,
			 		    GLfloat viewX,     GLfloat viewY,     GLfloat viewZ,
						GLfloat upVectorX, GLfloat upVectorY, GLfloat upVectorZ);

	// This rotates the camera's view around the position depending on the values passed in.
	GLvoid RotateView(GLfloat angle, GLfloat X, GLfloat Y, GLfloat Z);

	// This moves the camera's view by the mouse movements (First person view)
	GLvoid SetViewByMouse(); 

	// This rotates the camera around a poGLint (I.E. your character).
	GLvoid RotateAroundPoint(CVector3 vCenter, GLfloat X, GLfloat Y, GLfloat Z);


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

	// This strafes the camera left or right depending on the speed (+/-) 
	GLvoid StrafeCamera(GLfloat speed);

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


	// This will move the camera forward or backward depending on the speed
	GLvoid MoveCamera(GLfloat speed);

	// This checks for keyboard movement
	GLvoid CheckForMovement();


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

	// This updates the camera's view and other data (Should be called each frame)
	GLvoid Update();

	// This uses gluLookAt() to tell OpenGL where to look
	GLvoid Look();



	// The camera's position
	CVector3 m_vPosition;					

	// The camera's view
	CVector3 m_vView;						

	// The camera's up vector
	CVector3 m_vUpVector;		
	
	// The camera's strafe vector
	CVector3 m_vStrafe;						

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

};
extern CCamera g_Camera;

#endif
