//*********************************** CVector3 **********************************************//

class CVector3
{
public:
	
	// A default constructor
	CVector3() {}
	// This is our constructor that allows us to initialize our data upon creating an instance
	CVector3(float X, float Y, float Z) 
	{ 
		x = X; y = Y; z = Z;
	}

	// Here we overload the + operator so we can add vectors together 
	CVector3 operator+(CVector3 vVector)
	{
		// Return the added vectors result.
		return CVector3(vVector.x + x, vVector.y + y, vVector.z + z);
	}
	// Here we overload the - operator so we can subtract vectors 
	CVector3 operator-(CVector3 vVector)
	{
		// Return the subtracted vectors result
		return CVector3(x - vVector.x, y - vVector.y, z - vVector.z);
	}	
	// Here we overload the * operator so we can multiply by scalars
	CVector3 operator*(float num)
	{
		// Return the scaled vector
		return CVector3(x * num, y * num, z * num);
	}
	// Here we overload the / operator so we can divide by a scalar
	CVector3 operator/(float num)
	{
		// Return the scale vector
		return CVector3(x / num, y / num, z / num);
	}

	CVector3& operator+=(CVector3 vVector)
	{
		x+=vVector.x;
		y+=vVector.y;
		z+=vVector.z;
		return *this;
	}

	float x, y, z;						
};

//*********************************** CVector3 **********************************************//

void Normalize(CVector3& vVector); // Function, that normalizes vector

//********************************** Ammo ***************************************************//

class Ammo
{
	public:

	CVector3 pos,inc; // Position and moving data
	int damage;		  // Damage that ammo causes
	float rotx,roty;  // Rotation data
	void(*Draw)();    // Pointer to our drawing function;
	void DrawMe();    // Main drawing function
	int follow;
	Ammo();		// Default constructor
	Ammo(CVector3 POS, CVector3 INC, int dam, void(*draw)(), float rotX,float rotY); // Another constructor



}; 
typedef vector<Ammo> CAmmo; // Here we create new data type- ammo vector
//********************************** Ammo ***************************************************//


class MissileBase
{
	public:
	CVector3 pos;
	int count;
	void DrawMe();
	void Shoot(CAmmo &ammo,float delay);

	MissileBase(){pos=CVector3();count=0;}
	MissileBase(CVector3 POS){pos=POS; count=0;}
};

typedef vector<MissileBase> CMb;

void DrawMB(CMb &mb);

struct Particle
{
	CVector3 pos,inc;
	float life;
	void DrawMe();

	Particle(){pos=inc=CVector3(); life=0.5f;}
	Particle(CVector3 POS, CVector3 INC){pos=POS;inc=INC;life=0.5f;}
};



typedef vector<Particle> CPart;

void DrawParticles(CPart &part);
void DeleteAmmo(CAmmo &ammo, int num);

extern CAmmo Mlasers,Glasers,missiles;

extern int weapon,missile,mbs,life;