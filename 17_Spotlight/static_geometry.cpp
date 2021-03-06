#include "common_header.h"

#include "static_geometry.h"

/*-----------------------------------------------

Name:	GenerateTorus

Params:	vboDest - VBO where to store torus
		fRadius - outer radius
		fTubeRadius - inner radius
		iSubDivAround - subdivisions around torus
		iSubDivTube - subdivisions of tube

Result: Generates centered torus with specified
		parameters and stores it in VBO.

/*---------------------------------------------*/

GLint GenerateTorus(CVertexBufferObject &vboDest, GLfloat fRadius, GLfloat fTubeRadius, GLint iSubDivAround, GLint iSubDivTube)
{
	GLfloat fAddAngleAround = 360.0f/(float)iSubDivAround;
	GLfloat fAddAngleTube = 360.0f/(float)iSubDivTube;

	GLfloat fCurAngleAround = 0.0f;
	GLint iStepsAround = 1;
	const GLfloat PI = float(atan(1.0)*4.0);

	GLint iFacesAdded = 0;

	while(iStepsAround <= iSubDivAround)
	{
		GLfloat fSineAround = sin(fCurAngleAround/180.0f*PI);
		GLfloat fCosineAround = cos(fCurAngleAround/180.0f*PI);
		glm::vec3 vDir1(fSineAround, fCosineAround, 0.0f);
		GLfloat fNextAngleAround = fCurAngleAround+fAddAngleAround;
		GLfloat fNextSineAround = sin(fNextAngleAround/180.0f*PI);
		GLfloat fNextCosineAround = cos(fNextAngleAround/180.0f*PI);
		glm::vec3 vDir2(fNextSineAround, fNextCosineAround, 0.0f);
		GLfloat fCurAngleTube = 0.0f;
		GLint iStepsTube = 1;
		while(iStepsTube <= iSubDivTube)
		{
			GLfloat fSineTube = sin(fCurAngleTube/180.0f*PI);
			GLfloat fCosineTube = cos(fCurAngleTube/180.0f*PI);
			GLfloat fNextAngleTube = fCurAngleTube+fAddAngleTube;
			GLfloat fNextSineTube = sin(fNextAngleTube/180.0f*PI);
			GLfloat fNextCosineTube = cos(fNextAngleTube/180.0f*PI);
			glm::vec3 vMid1 = vDir1*(fRadius-fTubeRadius/2), vMid2 = vDir2*(fRadius-fTubeRadius/2);
			glm::vec3 vQuadPoints[] = 
			{
				vMid1 + glm::vec3(0.0f, 0.0f, -fNextSineTube*fTubeRadius) + vDir1*fNextCosineTube*fTubeRadius,
				vMid1 + glm::vec3(0.0f, 0.0f, -fSineTube*fTubeRadius) + vDir1*fCosineTube*fTubeRadius,
				vMid2 + glm::vec3(0.0f, 0.0f, -fSineTube*fTubeRadius) + vDir2*fCosineTube*fTubeRadius,
				vMid2 + glm::vec3(0.0f, 0.0f, -fNextSineTube*fTubeRadius) + vDir2*fNextCosineTube*fTubeRadius
			};

			glm::vec3 vNormals[] = 
			{
				glm::vec3(0.0f, 0.0f, -fNextSineTube) + vDir1*fNextCosineTube,
				glm::vec3(0.0f, 0.0f, -fSineTube) + vDir1*fCosineTube,
				glm::vec3(0.0f, 0.0f, -fSineTube) + vDir2*fCosineTube,
				glm::vec3(0.0f, 0.0f, -fNextSineTube) + vDir2*fNextCosineTube
			};

			glm::vec2 vTexCoords[] = 
			{
				glm::vec2(fCurAngleAround/360.0f, fNextAngleTube/360.0f),
				glm::vec2(fCurAngleAround/360.0f, fCurAngleTube/360.0f),
				glm::vec2(fNextAngleAround/360.0f, fCurAngleTube/360.0f),
				glm::vec2(fNextAngleAround/360.0f, fNextAngleTube/360.0f)
			};

			GLint iIndices[] = {0, 1, 2, 2, 3, 0};

			FOR(i, 6)
			{
				GLint index = iIndices[i];
				vboDest.AddData(&vQuadPoints[index], sizeof(glm::vec3));
				vboDest.AddData(&vTexCoords[index], sizeof(glm::vec2));
				vboDest.AddData(&vNormals[index], sizeof(glm::vec3));
			}
			iFacesAdded += 2; // Keep count of added faces
			fCurAngleTube += fAddAngleTube;
			iStepsTube++;
		}
		fCurAngleAround += fAddAngleAround;
		iStepsAround++;
	}
	return iFacesAdded;
}

/*-----------------------------------------------

Name:	GenerateCylinder

Params:	vboDest - VBO where to store torus
		fRadius - outer radius
		fHeight - height of cylinder
		iSubDivAround - subdivisions around cylinder

Result: Generates centered opened cylinder and stores
		it in VBO.

/*---------------------------------------------*/

GLint GenerateCylinder(CVertexBufferObject &vboDest, GLfloat fRadius, GLfloat fHeight, GLint iSubDivAround, GLfloat fMapU, GLfloat fMapV)
{
	GLfloat fAddAngleAround = 360.0f/(float)(iSubDivAround-1);

	GLfloat fCurAngleAround = 0.0f;
	GLint iStepsAround = 1;
	const GLfloat PI = float(atan(1.0)*4.0);

	GLint iFacesAdded = 0;

	while(iStepsAround <= iSubDivAround)
	{
		GLfloat fSineAround = sin(fCurAngleAround/180.0f*PI);
		GLfloat fCosineAround = cos(fCurAngleAround/180.0f*PI);
		glm::vec3 vDir1(fCosineAround, 0.0f, fSineAround);
		GLfloat fNextAngleAround = fCurAngleAround+fAddAngleAround;
		GLfloat fNextSineAround = sin(fNextAngleAround/180.0f*PI);
		GLfloat fNextCosineAround = cos(fNextAngleAround/180.0f*PI);
		glm::vec3 vDir2(fNextCosineAround, 0.0f, fNextSineAround);

		glm::vec3 vQuadPoints[] = 
		{
			glm::vec3(0.0f, fHeight, 0.0f) + vDir1*fRadius,
			vDir1*fRadius,
			vDir2*fRadius,
			glm::vec3(0.0f, fHeight, 0.0f) + vDir2*fRadius
		};


		glm::vec2 vTexCoords[] = 
		{
			glm::vec2(fMapU*fCurAngleAround/360.0f, fMapV),
			glm::vec2(fMapU*fCurAngleAround/360.0f, 0.0f),
			glm::vec2(fMapU*fNextAngleAround/360.0f, 0.0f),
			glm::vec2(fMapU*fNextAngleAround/360.0f, fMapV)
		};

		glm::vec3 vNormals[] = 
		{
			vDir1,
			vDir1,
			vDir2,
			vDir2
		};

		GLint iIndices[] = {0, 1, 2, 2, 3, 0};

		FOR(i, 6)
		{
			GLint index = iIndices[i];
			vboDest.AddData(&vQuadPoints[index], sizeof(glm::vec3));
			vboDest.AddData(&vTexCoords[index], sizeof(glm::vec2));
			vboDest.AddData(&vNormals[index], sizeof(glm::vec3));
		}
		iFacesAdded += 2; // Keep count of added faces

		fCurAngleAround += fAddAngleAround;
		iStepsAround++;
	}
	return iFacesAdded;
}

glm::vec3 vCubeVertices[36] = 
{
	// Front face
	glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, 0.5f),
	// Back face
	glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, -0.5f),
	// Left face
	glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, -0.5f),
	// Right face
	glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f),
	// Top face
	glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, -0.5f),
	// Bottom face
	glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, 0.5f),
};
glm::vec2 vCubeTexCoords[6] = {glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f)};

glm::vec3 vCubeNormals[6] = 
{
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(0.0f, 0.0f, -1.0f),
	glm::vec3(-1.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, -1.0f, 0.0f)
};

glm::vec3 vGround[6] = 
{
	glm::vec3(-1000, 0, -1000), glm::vec3(-1000, 0, 1000), glm::vec3(1000, 0, 1000), glm::vec3(1000, 0, 1000), glm::vec3(1000, 0, -1000), glm::vec3(-1000, 0, -1000)
};

glm::vec3 vStair[] = 
{
	glm::vec3(-40.0f, 4.0f, -15.0f), glm::vec3(0.0f, 4.0f, -15.0f), glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(-40.0f, 4.0f, 0.0f), // Top
	glm::vec3(-40.0f, 4.0f, 0.0f), glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-40.0f, 0.0f, 0.0f), // Front
	glm::vec3(0.0f, 4.0f, -15.0f), glm::vec3(-40.0f, 4.0f, -15.0f), glm::vec3(-40.0f, 0.0f, -15.0f), glm::vec3(0.0f, 0.0f, -15.0f), // Back
	glm::vec3(-40.0f, 4.0f, -15.0f), glm::vec3(-40.0f, 4.0f, 0.0f), glm::vec3(-40.0f, 0.0f, 0.0f), glm::vec3(-40.0f, 0.0f, -15.0f), // Left
	glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(0.0f, 4.0f, -15.0f), glm::vec3(0.0f, 0.0f, -15.0f), glm::vec3(0.0f, 0.0f, 0.0f), // Right
};

glm::vec2 vStairCoords[] = 
{
	glm::vec2(0.0f, 0.9f), glm::vec2(2.0f, 0.9f), glm::vec2(2.0f, 0.0f), glm::vec2(0.0f, 0.0f),
	glm::vec2(0.0f, 0.2f), glm::vec2(2.0f, 0.2f), glm::vec2(2.0f, 0.0f), glm::vec2(0.0f, 0.0f),
	glm::vec2(0.0f, 0.2f), glm::vec2(2.0f, 0.2f), glm::vec2(2.0f, 0.0f), glm::vec2(0.0f, 0.0f),
	glm::vec2(0.0f, 0.2f), glm::vec2(0.9f, 0.2f), glm::vec2(0.9f, 0.0f), glm::vec2(0.0f, 0.0f),
	glm::vec2(0.0f, 0.2f), glm::vec2(0.9f, 0.2f), glm::vec2(0.9f, 0.0f), glm::vec2(0.0f, 0.0f)
};

glm::vec3 vStairNormals[] = 
{
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(0.0f, 0.0f, -1.0f),
	glm::vec3(-1.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.0f)
};

GLint iCylinderFaces;

/*-----------------------------------------------

Name:	AddSceneObjects

Params:	vboDest - VBO where to store objects

Result: Adds all static objects to scene.

/*---------------------------------------------*/

GLvoid AddSceneObjects(CVertexBufferObject& vboDest)
{
	// Add ground to VBO
	FOR(i, 6)
	{
		vboDest.AddData(&vGround[i], sizeof(glm::vec3));
		glm::vec2 vCoord = vCubeTexCoords[i]*50.0f;
		vboDest.AddData(&vCoord, sizeof(glm::vec2));
		glm::vec3 vGroundNormal(0.0f, 1.0f, 0.0f);
		vboDest.AddData(&vGroundNormal, sizeof(glm::vec3));
	}

	// Add stair to VBO
	FOR(i, 5) // i is stair's side, 5 sides are enough (all beside bottom side)
	{
		GLint iIndices[] = {0, 1, 2, 2, 3, 0};
		FOR(j, 6)
		{
			vboDest.AddData(&vStair[4*i+iIndices[j]], sizeof(glm::vec3));
			vboDest.AddData(&vStairCoords[4*i+iIndices[j]], sizeof(glm::vec2));
			vboDest.AddData(&vStairNormals[i], sizeof(glm::vec3));
		}
	}

	iCylinderFaces = GenerateCylinder(vboDest, 60.0f, 250.0f, 15, 8.0f, 15.0f);
}