//************************************* CVector2 ************************************************

class CVector2
{
public:
	GLfloat x,y; // Data used for position on axises
	//----------------- Constructors -----------------------

	CVector2(){x=y=0;}
	CVector2(GLfloat X, GLfloat Y){x=X;y=Y;}

	//-------------------- Operators -----------------------

	// Operator + for adding two vectors together
	CVector2 operator+(CVector2 vAdd){return CVector2(x+vAdd.x,y+vAdd.y);}
	// Operator - for subtracting two vectors
	CVector2 operator-(CVector2 vSub){return CVector2(x-vSub.x,y-vSub.y);}
	// Operator * for scaling vector
	CVector2 operator*(GLfloat value){return CVector2(x*value,y*value);}
	// Operator / for dividing vector
	CVector2 operator/(GLfloat value){return CVector2(x/value,y/value);}
	// Operator += for nice adding of two vectors
	GLvoid operator+=(CVector2 vAdd){x+=vAdd.x;y+=vAdd.y;}
	// Operator -= for nice subtracting of two vectors
	GLvoid operator-=(CVector2 vSub){x-=vSub.x;y-=vSub.y;}
	// Operator *= for nice vector scaling
	GLvoid operator*=(GLfloat value){x*=value;y*=value;}
	// Operator /= for nice vector dividing
	GLvoid operator/=(GLfloat value){x/=value;y/=value;}
};

//************************************* CVector2 ************************************************

GLfloat RotateAfter(CVector2 from, CVector2 after);
GLfloat Magnitude(CVector2 vVector);
CVector2 v(CVector2 vInc);
GLboolean CBCollide(CVector2 &b1, CVector2 &b2,CVector2 &pos1, CVector2 &pos2);
GLfloat Distance(CVector2 &vPoint1, CVector2 &vPoint2);