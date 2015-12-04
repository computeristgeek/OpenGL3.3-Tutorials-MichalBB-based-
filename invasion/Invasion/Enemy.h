//********************************** Enemy **************************************************//

class Enemy
{
public:

	CVector3 pos,inc; // Position data and moving data
	int life; // Life
	Ammo* ammo; // Pointer to ammo to use
	void Shoot(float millisec, CAmmo &ammo,void(*DRAW)()); // Shooting function
	void DrawMe(); // Drawing function
	void(*Draw)(); // Pointer to a draw function
	float lastshoottime; // Time of last shot

	Enemy();
	Enemy(CVector3 POS,int LIFE, void(*draw)());

};

//********************************** Enemy **************************************************//

//********************************* Explosion ***********************************************//


class Explosion
{
	public:
	CVector3 pos;
	float size;
	void Simulate();
	Explosion(){pos=CVector3();size=0;}
	Explosion(CVector3 POS){pos=POS;size=0;}

};



typedef vector<Enemy> CEnemy; // Here we create new data type- enemy vector
typedef vector<Explosion> CExplo; // Here we create new data type- explosion vector
typedef vector<CVector3> CItem;

void CreateRandomObject(const Enemy &enemy);
void DrawAllObjects();

extern CEnemy martian;
extern CExplo mart;
extern float mt;
extern CItem health,missilepack,bazooka2;