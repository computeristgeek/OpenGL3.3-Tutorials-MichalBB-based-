class Ammo
{
public:
	CVector2 mvPos,mvInc;
	float life;
	virtual void Draw()=0;
};

class Bullet : public Ammo
{
public:
	void Draw();
	Bullet(){mvPos=mvInc=CVector2();  life=3;}
	Bullet(CVector2 setmvPos,CVector2 setmvInc){mvPos=setmvPos; mvInc=setmvInc; life=3;}

};

class Rocket : public Ammo
{
public:
	void Draw();
	float angle;
	Rocket(){mvPos=mvInc=CVector2(); life=3;}
	Rocket(CVector2 setmvPos,CVector2 setmvInc,float setangle){angle=setangle; mvPos=setmvPos; mvInc=setmvInc; life=3;}

};

class Bolt : public Ammo
{
public:
	void Draw();
	float angle;
	Bolt(){mvPos=mvInc=CVector2(); life=3;}
	Bolt(CVector2 setmvPos,CVector2 setmvInc,float setangle){angle=setangle; mvPos=setmvPos; mvInc=setmvInc; life=3;}

};


class Soldier
{
public:
	CVector2 mvPos,mvShoot; // Soldier's position
	int weapon; // Weapon held
	int life;
	int ammo[3];
	float shtime; // Time to shoot
	vector<Bullet> bullets;
	vector<Rocket> rockets;
	vector<Bolt> bolts;
	void Draw();
	void DisplayInfo();
	Soldier(){mvPos=mvShoot=CVector2(); weapon=0; shtime=0; life=100; ammo[0]=100; ammo[1]=10; ammo[2]=25;}
};

class Enemy
{
public:
	CVector2 mvPos;
	int life;
	float shtime;
	virtual void Draw()=0;
};

class Machiner : public Enemy
{
public:
	vector<Bullet> bullets;
	void Draw();
	Machiner(){mvPos=CVector2(); life=10; shtime=0;}
	Machiner(CVector2 setmvPos){life=10; mvPos=setmvPos; shtime=0;}
};

class Robot : public Enemy
{
public:
	vector<Bolt> bolts;
	void Draw();
	Robot(){mvPos=CVector2(); life=25; shtime=0;}
	Robot(CVector2 setmvPos){life=25; mvPos=setmvPos; shtime=0;}
};

class Can : public Enemy
{
public:
	vector<Rocket> rockets;
	void Draw();
	Can(){mvPos=CVector2(); life=90; shtime=0;}
	Can(CVector2 setmvPos){life=90; mvPos=setmvPos; shtime=0;}
};

template <class item>
void DeleteFromVector(vector<item> &vec,int index)
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
extern bool win;

extern vector<CVector2> health;
extern int enleft;

void AddEnemy();
void DrawMachiners();