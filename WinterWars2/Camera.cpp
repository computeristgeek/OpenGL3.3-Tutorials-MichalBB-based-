#include "main.h"
#include "Camera.h"
						
CVector3 Cross(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vNormal;	

	// Calculate the cross product with the non communitive equation
	vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
	vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
	vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	// Return the cross product
	return vNormal;										 
}


float Magnitude(CVector3 vNormal)
{
	// Here is the equation:  magnitude = sqrt(V.x^2 + V.y^2 + V.z^2) : Where V is the vector
	return (float)sqrt( (vNormal.x * vNormal.x) + 
						(vNormal.y * vNormal.y) + 
						(vNormal.z * vNormal.z) );
}

CVector3 Normalize(CVector3 vVector)
{
	// Get the magnitude of our normal
	float magnitude = Magnitude(vVector);				

	vVector = vVector / magnitude;		
	
	// Finally, return our normalized vector
	return vVector;										
}



CCamera::CCamera()
{
	CVector3 vZero = CVector3(0.0, 0.0, 0.0);		// Init a vVector to 0 0 0 for our position
	CVector3 vView = CVector3(0.0, 1.0, 0.5);		// Init a starting view vVector (looking up and out the screen) 
	CVector3 vUp   = CVector3(0.0, 0.0, 1.0);		// Init a standard up vVector (Rarely ever changes)

	m_vPosition	= vZero;					// Init the position to zero
	m_vView		= vView;					// Init the view to a std starting view
	m_vUpVector	= vUp;						// Init the UpVector
}


void CCamera::PositionCamera(float positionX, float positionY, float positionZ,
				  		     float viewX,     float viewY,     float viewZ,
							 float upVectorX, float upVectorY, float upVectorZ)
{
	CVector3 vPosition	= CVector3(positionX, positionY, positionZ);
	CVector3 vView		= CVector3(viewX, viewY, viewZ);
	CVector3 vUpVector	= CVector3(upVectorX, upVectorY, upVectorZ);

	// The code above just makes it cleaner to set the variables.
	// Otherwise we would have to set each variable x y and z.

	m_vPosition = vPosition;					// Assign the position
	m_vView     = vView;						// Assign the view
	m_vUpVector = vUpVector;					// Assign the up vector
}


	float angleY;						
	float angleZ;				
void CCamera::SetViewByMouse()
{
	POINT mousePos;									// This is a window structure that holds an X and Y
	int middleX = SCREEN_WIDTH >> 1;				// This is a binary shift to get half the width
	int middleY = SCREEN_HEIGHT >> 1;				// This is a binary shift to get half the height

	static float currentRotX = 0.0f;
	GetCursorPos(&mousePos);						
	if( (mousePos.x == middleX) && (mousePos.y == middleY) )return;
	SetCursorPos(middleX, middleY);							
	angleY = (float)( (middleX - mousePos.x) ) / 1000.0f;		
	angleZ = (float)( (middleY - mousePos.y) ) / 1000.0f;		

	currentRotX -= angleZ;  

	if(currentRotX > 1.5f)
		currentRotX = 1.5f;

	else if(currentRotX < -1.5f)
		currentRotX = -1.5f;
	// Otherwise, we can rotate the view around our position
	else
	{

		CVector3 vAxis = Cross(m_vView - m_vPosition, m_vUpVector);
		vAxis = Normalize(vAxis);

		// Rotate around our perpendicular axis and along the y-axis
		RotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);
		RotateView(angleY, 0, 1, 0);
	}
}

void CCamera::RotateView(float angle, float x, float y, float z)
{
	CVector3 vNewView;

	// Get the view vector (The direction we are facing)
	CVector3 vView = m_vView - m_vPosition;		

	// Calculate the sine and cosine of the angle once
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	// Find the new x position for the new rotated point
	vNewView.x  = (cosTheta + (1 - cosTheta) * x * x)		* vView.x;
	vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* vView.y;
	vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* vView.z;

	// Find the new y position for the new rotated point
	vNewView.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* vView.x;
	vNewView.y += (cosTheta + (1 - cosTheta) * y * y)		* vView.y;
	vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* vView.z;

	// Find the new z position for the new rotated point
	vNewView.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* vView.x;
	vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* vView.y;
	vNewView.z += (cosTheta + (1 - cosTheta) * z * z)		* vView.z;


	m_vView = m_vPosition + vNewView;
}



void CCamera::StrafeCamera(float speed)
{

	m_vPosition.x += m_vStrafe.x * speed;
	m_vPosition.z += m_vStrafe.z * speed;

	// Add the strafe vector to our view
	m_vView.x += m_vStrafe.x * speed;
	m_vView.z += m_vStrafe.z * speed;
}


void CCamera::MoveCamera(float speed)
{
	// Get the current view vector (the direction we are looking)
	CVector3 vVector = m_vView - m_vPosition;

		vVector = Normalize(vVector);

	m_vPosition.x += vVector.x * speed;		// Add our acceleration to our position's X
	m_vPosition.z += vVector.z * speed;		// Add our acceleration to our position's Z
	m_vView.x += vVector.x * speed;			// Add our acceleration to our view's X
	m_vView.z += vVector.z * speed;			// Add our acceleration to our view's Z
}

void CCamera::CheckForMovement()
{

	if(GetKeyState(VK_UP) & 0x80) {	

		MoveCamera(kSpeed);	
		
	}

	// Check if we hit the Down arrow or the 's' key
	if(GetKeyState(VK_DOWN) & 0x80) {			

		// Move our camera backward by a negative SPEED
		MoveCamera(-kSpeed);				
	}

	// Check if we hit the Left arrow or the 'a' key
	if(GetKeyState(VK_LEFT) & 0x80) {			

		// Strafe the camera left
		StrafeCamera(-kSpeed);
	}

	// Check if we hit the Right arrow or the 'd' key
	if(GetKeyState(VK_RIGHT) & 0x80) {			

		// Strafe the camera right
		StrafeCamera(kSpeed);
	}	
}

//////////////////////////// UPDATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::Update() 
{

	CVector3 vCross = Cross(m_vView - m_vPosition, m_vUpVector);

	// Normalize the strafe vector
	m_vStrafe = Normalize(vCross);

	// Move the camera's view by the mouse
	SetViewByMouse();

	// This checks to see if the keyboard was pressed
	CheckForMovement();
}



void CCamera::Look()
{
	// Give openGL our camera position, then camera view, then camera up vector
	gluLookAt(m_vPosition.x, m_vPosition.y, m_vPosition.z,	
			  m_vView.x,	 m_vView.y,     m_vView.z,	
			  m_vUpVector.x, m_vUpVector.y, m_vUpVector.z);
}


