class Ammo
{
public:
	CVector2 mvPos,mvInc;
	GLfloat life;
	virtual GLvoid Draw()=0;
};

class Bullet : public Ammo
{
public:
	GLvoid Draw();
	Bullet(){mvPos=mvInc=CVector2();  life=3;}
	Bullet(CVector2 setmvPos,CVector2 setmvInc){mvPos=setmvPos; mvInc=setmvInc; life=3;}

};

class Rocket : public Ammo
{
public:
	GLvoid Draw();
	GLfloat angle;
	Rocket(){mvPos=mvInc=CVector2(); life=3;}
	Rocket(CVector2 setmvPos,CVector2 setmvInc,GLfloat setangle){angle=setangle; mvPos=setmvPos; mvInc=setmvInc; life=3;}

};

class Bolt : public Ammo
{
public:
	GLvoid Draw();
	GLfloat angle;
	Bolt(){mvPos=mvInc=CVector2(); life=3;}
	Bolt(CVector2 setmvPos,CVector2 setmvInc,GLfloat setangle){angle=setangle; mvPos=setmvPos; mvInc=setmvInc; life=3;}

};


class Soldier
{
public:
	CVector2 mvPos,mvShoot; // Soldier's position
	GLint weapon; // Weapon held
	GLint life;
	GLint ammo[3];
	GLfloat shtime; // Time to shoot
	vector<Bullet> bullets;
	vector<Rocket> rockets;
	vector<Bolt> bolts;
	GLvoid Draw();
	GLvoid DisplayInfo();
	Soldier(){mvPos=mvShoot=CVector2(); weapon=0; shtime=0; life=100; ammo[0]=100; ammo[1]=10; ammo[2]=25;}
};

class Enemy
{
public:
	CVector2 mvPos;
	GLint life;
	GLfloat shtime;
	virtual GLvoid Draw()=0;
};

class Machiner : public Enemy
{
public:
	vector<Bullet> bullets;
	GLvoid Draw();
	Machiner(){mvPos=CVector2(); life=10; shtime=0;}
	Machiner(CVector2 setmvPos){life=10; mvPos=setmvPos; shtime=0;}
};

class Robot : public Enemy
{
public:
	vector<Bolt> bolts;
	GLvoid Draw();
	Robot(){mvPos=CVector2(); life=25; shtime=0;}
	Robot(CVector2 setmvPos){life=25; mvPos=setmvPos; shtime=0;}
};

class Can : public Enemy
{
public:
	vector<Rocket> rockets;
	GLvoid Draw();
	Can(){mvPos=CVector2(); life=90; shtime=0;}
	Can(CVector2 setmvPos){life=90; mvPos=setmvPos; shtime=0;}
};

template <class item>
GLvoid DeleteFromVector(vector<item> &vec,GLint index)
{
	vec[index]=vec[vec.size()-1];
	vec.pop_back();
}

extern UINT textures[11];
extern FSOUND_SAMPLE* sounds[5];
extern Soldier g_soldier;
extern vector<Machiner> machiners;
extern vector<Robot> robots;
extern vector<Can> cans;
extern GLboolean win;

extern vector<CVector2> health;
extern GLint enleft;

GLvoid AddEnemy();
GLvoid DrawMachiners();