class Rgb
{
public:
	GLfloat red,green,blue;
	Rgb(){red=green=blue=0;}
	Rgb(GLfloat RED, GLfloat GREEN, GLfloat BLUE){red=RED;green=GREEN;blue=BLUE;}
};

#define CENTER 0
#define NOTHING 1

GLvoid BuildFont();
GLvoid PrintText(CVector2 textpos,GLfloat size,Rgb color, GLint type,UINT flags, const char*fmt,...);
GLvoid CreateTexture(UINT *storage, LPSTR strFileName);

GLvoid BuildFont();
GLvoid Start2D(),End2D();