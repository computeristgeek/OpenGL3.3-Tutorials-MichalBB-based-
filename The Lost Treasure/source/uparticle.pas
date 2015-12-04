unit UParticle;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, UTexture, Math3D, GL;

type CParticle = object
	vPos, vDir : CVector3;
	fLife : real;
	r, g, b : byte;
	active : boolean;
end;

type CQuad = object
vDirX, vDirY : CVector3;
end;

var Particles : array[0..1499] of CParticle;
var FIP : integer; // Index of first inactive particle

function frand(min, max:single):single;
procedure AddParticle(Pos, Dir : CVector3; Life : Real; rr, gg, bb : byte);
procedure renderParticles(fTime : single);
implementation

function frand(min, max:single):single;
var r:real;
begin;
r := random(1000001);
frand := min+(max-min)*(r/1000000.0);
end;

procedure AddParticle(Pos, Dir : CVector3; Life : Real; rr, gg, bb : byte);
var Particle : CParticle;
var i:integer;
begin;
Particle.vPos := Pos; Particle.vDir := Dir; Particle.fLife := Life; Particle.r := rr; Particle.g := gg; Particle.b := bb;
Particle.active := true;
Particles[FIP] := Particle;
for i:=FIP+1 to 1499 do if not Particles[i].active then begin; FIP := i; break; end;
end;

function getNormQuad(vNorm : CVector3) : CQuad;
begin;
if(fabsf(vNorm.y-1.0) < 0.0001)then getNormQuad.vDirX := cross(vNorm, Vector3(-1, 0, 0))
else getNormQuad.vDirX := cross(vNorm, Vector3(0, 1, 0));
normalize(getNormQuad.vDirX);
getNormQuad.vDirY := cross(vNorm, getNormQuad.vDirX);
normalize(getNormQuad.vDirY);
end;

procedure renderParticles(fTime : single);
var i : integer;
var quad : CQuad;
begin;
quad := getNormQuad(Vector3(-tcMain.vDir.x, 20, -tcMain.vDir.z));
quad.vDirX := vectorScale(quad.vDirX, 1.4);quad.vDirY := vectorScale(quad.vDirY, 1.4);
glDepthMask(0);
tTextures[1].bindTexture;
glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE);
glBegin(GL_QUADS);
for i:=0 to 1499 do
begin;
	if not Particles[i].active then continue;
	with Particles[i] do
   begin;
   	glColor4f(single(r)/255.0, single(g)/255.0, single(b)/255.0, getmin(1.0, fLife));
		glTexCoord2f(0, 1); glVertex3f(vPos.x-quad.vDirX.x+quad.vDirY.x, vPos.y-quad.vDirX.y+quad.vDirY.y, vPos.z-quad.vDirX.z+quad.vDirY.z);
		glTexCoord2f(1, 1); glVertex3f(vPos.x+quad.vDirX.x+quad.vDirY.x, vPos.y+quad.vDirX.y+quad.vDirY.y, vPos.z+quad.vDirX.z+quad.vDirY.z);
		glTexCoord2f(1, 0); glVertex3f(vPos.x+quad.vDirX.x-quad.vDirY.x, vPos.y+quad.vDirX.y-quad.vDirY.y, vPos.z+quad.vDirX.z-quad.vDirY.z);
      glTexCoord2f(0, 0); glVertex3f(vPos.x-quad.vDirX.x-quad.vDirY.x, vPos.y-quad.vDirX.y-quad.vDirY.y, vPos.z-quad.vDirX.z-quad.vDirY.z);
      vPos := vectorPlus(vPos, vectorScale(vDir, fTime));
      fLife -= fTime;
      if(fLife < 0)then begin; particles[i].active := false; if(i < FIP)then FIP := i; end;
	end;
end;
glEnd();
glDisable(GL_BLEND);
glDepthMask(1);
end;

end.

