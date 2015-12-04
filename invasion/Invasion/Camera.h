//************************************ CCamera **********************************************//
class CCamera {

public:

	// Our camera constructor
	CCamera();	

	// This changes the position, view, and up vector of the camera.
	// This is primarily used for initialization
	void PositionCamera(float positionX, float positionY, float positionZ,
			 		    float viewX,     float viewY,     float viewZ,
						float upVectorX, float upVectorY, float upVectorZ);

	// This rotates the camera's view around the position depending on the values passed in.
	void RotateView(float angle, float X, float Y, float Z);

	// This moves the camera's view by the mouse movements (First person view)
	void SetViewByMouse(); 

	// This strafes the camera left or right depending on the speed (+/-) 
	void StrafeCamera(float speed);

	// This will move the camera forward or backward depending on the speed
	void MoveCamera(float speed);

	// This checks for keyboard movement
	void CheckForMovement();

	// This updates the camera's view and other data (Should be called each frame)
	void Update();

	// This uses gluLookAt() to tell OpenGL where to look
	void Look();

	CVector3 m_vPosition;	// The camera's position				
	CVector3 m_vView;		// The camera's view				
	CVector3 m_vUpVector;	// The camera's up vector	
	CVector3 m_vStrafe;		// The camera's strafe vector

	float anX,anY;          // Viewing angles
	

};

//************************************ CCamera **********************************************//
// This is our extern camera object
extern CCamera Main;
void CheckAmmoAndCamera(CCamera &camera, CAmmo &ammo);
