#include "common_header.h"

#include "static_geometry.h"

GLint generateTorus(CVertexBufferObject &vboDest, GLfloat fRadius, GLfloat fTubeRadius, GLint iSubDivAround, GLint iSubDivTube)
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
				vboDest.addData(&vQuadPoints[index], sizeof(glm::vec3));
				vboDest.addData(&vTexCoords[index], sizeof(glm::vec2));
				vboDest.addData(&vNormals[index], sizeof(glm::vec3));
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
	glm::vec3(-1000, 0, -1000), glm::vec3(50, 0, -1000), glm::vec3(1000, 0, 1000), glm::vec3(1000, 0, 1000), glm::vec3(-1000, 0, 1000), glm::vec3(-1000, 0, -1000)
};