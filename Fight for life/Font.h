class Rgb
{
public:
	float red,green,blue;
	Rgb(){red=green=blue=0;}
	Rgb(float RED, float GREEN, float BLUE){red=RED;green=GREEN;blue=BLUE;}
};

#define CENTER 0
#define NOTHING 1

void BuildFont();
void PrintText(CVector2 textpos,float size,Rgb color, int type,UINT flags, const char*fmt,...);
void CreateTexture(UINT *storage, LPSTR strFileName);

void BuildFont();
void Start2D(),End2D();