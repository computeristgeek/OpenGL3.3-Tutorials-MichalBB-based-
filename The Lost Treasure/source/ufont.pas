unit UFont;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, GL, GLU, UTexture;


var uiBase : Cardinal;
var tFont : CTexture;

procedure buildFont;
procedure print(x, y, pxsize : integer; sText : string);
procedure beginOrthoProjection;
procedure endOrthoProjection;

implementation


procedure beginOrthoProjection;
begin;
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 1024, 0, 768, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
end;

//====================

procedure endOrthoProjection;
begin;
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
end;

procedure buildFont;
var cx, cy : single;
var i : integer;
begin;
uiBase := glGenLists(256);
tFont.loadBMP('data\interface\font.bmp', 1);
tFont.bindTexture();
for i := 0 to 255 do
begin;
	cx:=single(i mod 16)/16.0;
	cy:=single(i div 16)/16.0;
	glNewList(uiBase+i,GL_COMPILE);
   	glBegin(GL_QUADS);
      glTexCoord2f(cx,1-cy-0.0625); glVertex2i(0,0);
		glTexCoord2f(cx+0.0625,1-cy-0.0625); glVertex2i(16,0);
		glTexCoord2f(cx+0.0625,1-cy); glVertex2i(16,16);
		glTexCoord2f(cx,1-cy); glVertex2i(0,16);
		glEnd();
		glTranslated(10,0,0);
	glEndList();
end;


end;



procedure print(x, y, pxsize : integer; sText : string);
var i : integer;
begin;
glPushMatrix();
glTranslated(x, y, 0);
glScalef(single(pxsize)/16.0, single(pxsize)/16.0, single(pxsize)/16.0);
tFont.bindTexture();
for i:=1 to Length(sText) do
glCallList(uiBase+ord(sText[i])-32);

glPopMatrix();
end;

end.

