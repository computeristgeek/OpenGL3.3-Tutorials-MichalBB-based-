unit Math3D;

{$mode objfpc}{$H+}

interface

uses
  Classes, GL, GLU, SysUtils, Dialogs, Math;

var TooGlobalString : string;

type CVector2 = object
x, y: single;
end;

type CVector3 = object
x, y, z: single;
end;

type CThirdPersonCamera = object
vDir : CVector3;

procedure look(vCenter : CVector3);
procedure rotate(fAngle : single);
end;

var tcMain: CThirdPersonCamera;

function Vector3(x,y,z:single):CVector3;
function VectorPlus(v1, v2 : CVector3):CVector3;
function VectorMinus(v1, v2 : CVector3):CVector3;
function VectorScale(v:CVector3;s:single):CVector3;

procedure Normalize(var vNormal : CVector3);
function Dot(vVector1, vVector2 : CVector3) : single;
function Cross(vVector1, vVector2 : CVector3) : CVector3;

function ClassifySphere(var vCenter, vNormal, vPoint : CVector3; radius : single; var distance2 : single) : integer;
function GetCollisionOffset(var vNormal : CVector3; radius, distance2 : single) : CVector3;
function InsidePolygon(vIntersection : CVector3; Poly : array of CVector3; vc : integer) : boolean;
function EdgeSphereCollision(var vCenter : CVector3; vPolygon : array of CVector3; vertexCount : integer; radius : single) : boolean;
function IntersectedPolygon(vPoly : array of CVector3; vLine : array of CVector3; vc : integer) : boolean;
function IntersectedPlane(vPoly : array of CVector3; vLine : array of CVector3; var vNormal : CVector3; var originDistance : single):boolean;
function AngleBetweenVectors(Vector1, Vector2 : CVector3) : single;
function fabsf(fNum : single) : single;
function Distance(vPoint1, vPoint2 : CVector3) : single;
function getMin(a,b:single):single;
function getMax(a,b:single):single;

implementation

//////// VECTORS /////////*
//
// Some vectors functions.
//
//////// VECTORS /////////*

function Vector3(x,y,z:single):CVector3;
begin;
Vector3.x := x; Vector3.y := y; Vector3.z := z;
end;

function VectorPlus(v1, v2 : CVector3):CVector3;
begin;
VectorPlus.x := v1.x+v2.x; VectorPlus.y := v1.y+v2.y; VectorPlus.z := v1.z+v2.z;
end;

function VectorMinus(v1, v2 : CVector3):CVector3;
begin;
VectorMinus.x := v1.x-v2.x; VectorMinus.y := v1.y-v2.y; VectorMinus.z := v1.z-v2.z;
end;

function VectorScale(v:CVector3;s:single):CVector3;
begin;
VectorScale.x := v.x*s; VectorScale.y := v.y*s; VectorScale.z := v.z*s;
end;


function fabsf(fNum : single) : single;
begin;
	if(fNum < 0) then fabsf := 0-fNum else fabsf := fNum;
end;


function Cross(vVector1, vVector2 : CVector3) : CVector3;
begin;
	Cross.x := ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
	Cross.y := ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
	Cross.z := ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));
end;

function Magnitude(vNormal : CVector3) :single;
begin;
	Magnitude := sqrt((vNormal.x*vNormal.x)+(vNormal.y*vNormal.y)+(vNormal.z*vNormal.z));
end;


procedure Normalize(var vNormal : CVector3);
var fMagn : single;
begin;
	fMagn := Magnitude(vNormal);
   if(fabsf(fMagn) < 0.001)then exit;
   vNormal := VectorScale(vNormal, 1.0/fMagn);
end;

function Distance(vPoint1, vPoint2 : CVector3) : single;
begin;
Distance := sqrt( (vPoint2.x - vPoint1.x) * (vPoint2.x - vPoint1.x) +
						    (vPoint2.y - vPoint1.y) * (vPoint2.y - vPoint1.y) +
						    (vPoint2.z - vPoint1.z) * (vPoint2.z - vPoint1.z) );
end;

function ClosestPointOnLine(vA, vB, vPoint : CVector3) : CVector3;
var vVector1, vVector2, vVector3 : CVector3;
var d, t : single;
begin;
	vVector1 := vectorMinus(vPoint, vA);
   vVector2 := vectorMinus(vB, vA); Normalize(vVector2);

	d := Distance(vA, vB);
   t := Dot(vVector2, vVector1);

    if (t <= 0)then ClosestPointOnLine := vA
    else if (t >= d)then ClosestPointOnLine := vB
    else begin;
    	vVector3 := vectorScale(vVector2, t);
    	ClosestPointOnLine := vectorPlus(vA, vVector3);
	end;
end;

function PlaneDistance(Normal, Point : CVector3) : single;
begin;
	PlaneDistance := - ((Normal.x * Point.x) + (Normal.y * Point.y) + (Normal.z * Point.z));
end;

function IntersectedPlane(vPoly : array of CVector3; vLine : array of CVector3; var vNormal : CVector3; var originDistance : single):boolean;
var distance1, distance2 : single;
begin;
	vNormal := Cross(vectorMinus(vPoly[0], vPoly[1]), vectorMinus(vPoly[2], vPoly[1]));
   Normalize(vNormal);
	originDistance := PlaneDistance(vNormal, vPoly[0]);

	distance1 := ((vNormal.x*vLine[0].x)  +
		         (vNormal.y*vLine[0].y)  +
				 (vNormal.z*vLine[0].z)) + originDistance;

	distance2 := ((vNormal.x * vLine[1].x)  +
		         (vNormal.y * vLine[1].y)  +
				 (vNormal.z * vLine[1].z)) + originDistance;

	if(distance1 * distance2 >= 0)then IntersectedPlane := False else IntersectedPlane := true;
end;

function Dot(vVector1, vVector2 : CVector3) : single;
begin;
	Dot := (vVector1.x * vVector2.x) + (vVector1.y * vVector2.y) + (vVector1.z * vVector2.z);
end;

function AngleBetweenVectors(Vector1, Vector2 : CVector3) : single;
var dotProduct, vectorsMagn: single;
begin;
	dotProduct := Dot(Vector1, Vector2);
   AngleBetweenVectors := 0.0;
	vectorsMagn := Magnitude(Vector1) * Magnitude(Vector2);
   try AngleBetweenVectors := ArcCos( (dotProduct / vectorsMagn) );
   except on EInvalidArgument do AngleBetweenVectors := 0.0; end;
end;

function IntersectionPoint(var vNormal : CVector3; vLine : array of CVector3; distance2 : single) : CVector3;
var vPoint, vLineDir : CVector3;
Numerator, Denominator, dist : single;
begin;
	vLineDir := vectorMinus(vLine[1], vLine[0]);
	Normalize(vLineDir);

	Numerator := - (vNormal.x * vLine[0].x +
				   vNormal.y * vLine[0].y +
				   vNormal.z * vLine[0].z + distance2);

	Denominator := Dot(vNormal, vLineDir);

	if( fabsf(Denominator) < 0.001)then begin;InterSectionPoint := vLine[0]; exit; end;
	dist := Numerator / Denominator;
	IntersectionPoint.x := (vLine[0].x + (vLineDir.x * dist));
	IntersectionPoint.y := (vLine[0].y + (vLineDir.y * dist));
	IntersectionPoint.z := (vLine[0].z + (vLineDir.z * dist));

end;

function InsidePolygon(vIntersection : CVector3; Poly : array of CVector3; vc : integer) : boolean;
var Angle : single;
var vA, vB : CVector3;
var i:integer;
begin;
	Angle := 0;
	for i:= 0 to vc-1 do
   begin;
		vA := vectorMinus(Poly[i], vIntersection);
		vB := vectorMinus(Poly[(i+1) mod vc],vIntersection);
		Angle += AngleBetweenVectors(vA, vB);
	end;

	if(Angle >= 0.99*(2*PI))then InsidePolygon := True else InsidePolygon := False;
end;

function IntersectedPolygon(vPoly : array of CVector3; vLine : array of CVector3; vc : integer) : boolean;
var vNormal, vIntersection : CVector3;
var originDistance : single;
begin;
	if(not IntersectedPlane(vPoly, vLine, vNormal, originDistance))then begin; InterSectedPolygon := False; exit; end;

	vIntersection := IntersectionPoint(vNormal, vLine, originDistance);

	IntersectedPolygon := InsidePolygon(vIntersection, vPoly, vc);
end;

function ClassifySphere(var vCenter, vNormal, vPoint : CVector3; radius : single; var distance2 : single) : integer;

var d : single;
begin;
	d := PlaneDistance(vNormal, vPoint);

	distance2 := (vNormal.x * vCenter.x + vNormal.y * vCenter.y + vNormal.z * vCenter.z + d);

	if(fabsf(distance2) < radius)then
		ClassifySphere := 0
	else if(distance2 >= radius)then
		ClassifySphere := 1
	else ClassifySphere := -1;
end;

function EdgeSphereCollision(var vCenter : CVector3; vPolygon : array of CVector3; vertexCount : integer; radius : single) : boolean;
var vPoint : CVector3;
var i:integer;
var distance2 : single;
begin;
	for i := 0 to VertexCount-1 do begin;
		vPoint := ClosestPointOnLine(vPolygon[i], vPolygon[(i+1) mod vertexCount], vCenter);
		distance2 := Distance(vPoint, vCenter);
		if(distance2 < radius)then begin; EdgeSphereCollision := true; exit; end;
	end;

	EdgeSphereCollision := false;
end;

function SpherePolygonCollision(vPolygon : array of CVector3; var vCenter : CVector3; vertexCount : integer; radius : single) : boolean;
var vNormal, vOffset, vPosition : CVector3;
var distance2 : single;
var classif : integer;
begin;
	vNormal := Cross(vectorMinus(vPolygon[0], vPolygon[1]), vectorMinus(vPolygon[2], vPolygon[1]));
   Normalize(vNormal);

	classif := ClassifySphere(vCenter, vNormal, vPolygon[0], radius, distance2);
	if(classif = 0)then
   begin;
		vOffset := vectorScale(vNormal, distance2);
		vPosition := vectorMinus(vCenter, vOffset);
		if(InsidePolygon(vPosition, vPolygon, 3))then begin; SpherePolygonCollision := true; exit; end;
		if(EdgeSphereCollision(vCenter, vPolygon, vertexCount, radius))then begin; SpherePolygonCollision := true; exit; end;
	end;

	SpherePolygonCollision := false;
end;

function GetCollisionOffset(var vNormal : CVector3; radius, distance2 : single) : CVector3;
var vOffset : CVector3;
var distanceOver : single;
begin;
	vOffset := Vector3(0, 0, 0);
	if(distance2 > 0)then begin;
		distanceOver := radius - distance2;
		vOffset := vectorScale(vNormal, distanceOver);
	end
	else begin;
		distanceOver := radius + distance2;
		vOffset := vectorScale(vNormal, -distanceOver);
	end;

	GetCollisionOffset := vOffset;
end;


//////// CTHIRDPERSONCAMERA /////////*
//
// Camera functions.
//
//////// CTHIRDPERSONCAMERA /////////*

procedure CThirdPersonCamera.look(vCenter : CVector3);
var vHelp : CVector3;
begin;
vHelp := vectorPlus(vCenter, vDir);
gluLookAt(vHelp.x, vHelp.y, vHelp.z, vCenter.x, vCenter.y, vCenter.z, 0, 1, 0);
end;

procedure CThirdPersonCamera.rotate(fAngle : single);
var sine, cosine : Single;
var vOld : CVector3;
begin;
vOld := vDir;
sine := sin(fAngle*PI/180.0);
cosine := cos(fAngle*PI/180.0);
vDir.x := cosine*vOld.x-sine*vOld.z;
vDir.z := sine*vOld.x+cosine*vOld.z;

end;

function getMin(a,b:single):single;begin;if a<b then getMin:=a else getMin:=b;end;
function getMax(a,b:single):single;begin;if a>b then getMax:=a else getMax:=b;end;
end.

