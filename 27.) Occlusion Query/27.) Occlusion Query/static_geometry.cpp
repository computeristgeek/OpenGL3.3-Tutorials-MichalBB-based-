#include "common_header.h"

#include "static_geometry.h"

CVertexBufferObject vboSceneObjects;
CVertexBufferObject vboOccluders;
UINT uiVAOSceneObjects;
UINT uiVAOOccluders;
float fCubeHalfSize = 30.0f;
int iSphereFaces;

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
	glm::vec3(-300, 0, -300), glm::vec3(300, 0, -300), glm::vec3(300, 0, 300), glm::vec3(300, 0, 300), glm::vec3(-300, 0, 300), glm::vec3(-300, 0, -300)
};

glm::vec2 vGroundTexCoords[6] = 
{
	glm::vec2(0.0f, 10.0f), glm::vec2(10.0f, 10.0f), glm::vec2(10.0f, 0.0f), glm::vec2(10.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 10.0f)
};

/*-----------------------------------------------

Name:	GenerateSphere

Params:	vboDest - VBO where to store sphere
		fRadius - sphere radius

Result: Generates centered sphere.

/*---------------------------------------------*/

int GenerateSphere(CVertexBufferObject &vboDest, float fRadius, int iSubDivY, int iSubDivZ)
{
	float fAddAngleY = 360.0f/float(iSubDivY), fAddAngleZ = 180.0f/float(iSubDivZ);
	float fCurAngleY = 0.0f;
	float fTexU = 1.0f/float(iSubDivY), fTexV = 1.0f/float(iSubDivZ); 
	int iStepsY = 1;

	const float PI = float(atan(1.0)*4.0);

	int iFacesAdded = 0;

	while(iStepsY <= iSubDivY)
	{
		float fNextAngleY = fCurAngleY+fAddAngleY;
		float fSineY = sin(fCurAngleY/180.0f*PI), fCosY = cos(fCurAngleY/180.0f*PI);
		float fNextSineY = sin(fNextAngleY/180.0f*PI), fNextCosY = cos(fNextAngleY/180.0f*PI);
		glm::vec3 vDirY(fCosY, 0.0f, -fSineY), vNextDirY(fNextCosY, 0.0f, -fNextSineY);
		float fCurAngleZ = 0.0f;
		int iStepsZ = 1;
		while(iStepsZ <= iSubDivZ)
		{
			float fNextAngleZ = fCurAngleZ+fAddAngleZ;

			float fSineZ = sin(fCurAngleZ/180.0f*PI), fCosZ = cos(fCurAngleZ/180.0f*PI);
			float fNextSineZ = sin(fNextAngleZ/180.0f*PI), fNextCosZ = cos(fNextAngleZ/180.0f*PI);

			glm::vec3 vQuadPoints[] = 
			{
				glm::vec3(vDirY.x*fSineZ*fRadius, fCosZ*fRadius, vDirY.z*fSineZ*fRadius),
				glm::vec3(vDirY.x*fNextSineZ*fRadius, fNextCosZ*fRadius, vDirY.z*fNextSineZ*fRadius),
				glm::vec3(vNextDirY.x*fNextSineZ*fRadius, fNextCosZ*fRadius, vNextDirY.z*fNextSineZ*fRadius),
				glm::vec3(vNextDirY.x*fSineZ*fRadius, fCosZ*fRadius, vNextDirY.z*fSineZ*fRadius),
			};

			glm::vec3 vNormals[] = 
			{
				glm::normalize(vQuadPoints[0]),
				glm::normalize(vQuadPoints[1]),
				glm::normalize(vQuadPoints[2]),
				glm::normalize(vQuadPoints[3])
			};

			glm::vec2 vTexCoords[] = 
			{
				glm::vec2(asin(vNormals[0].x)/PI+0.5f , asin(vNormals[0].y)/PI+0.5f),
				glm::vec2(asin(vNormals[1].x)/PI+0.5f , asin(vNormals[1].y)/PI+0.5f),
				glm::vec2(asin(vNormals[2].x)/PI+0.5f , asin(vNormals[2].y)/PI+0.5f),
				glm::vec2(asin(vNormals[3].x)/PI+0.5f , asin(vNormals[3].y)/PI+0.5f),
			};

			int iIndices[] = {0, 1, 2, 2, 3, 0};

			FOR(i, 6)
			{
				int index = iIndices[i];
				vboDest.AddData(&vQuadPoints[index], sizeof(glm::vec3));
				vboDest.AddData(&vTexCoords[index], sizeof(glm::vec2));
				vboDest.AddData(&vNormals[index], sizeof(glm::vec3));
			}
			iFacesAdded += 2; // Keep count of added faces

			iStepsZ++;
			fCurAngleZ += fAddAngleZ;
		}
		iStepsY++;
		fCurAngleY += fAddAngleY;
	}

	return iFacesAdded;
}


/*-----------------------------------------------

Name:	PrepareStaticSceneObjects

Params:	none

Result: Initializes all static scene objects and
builds its VAOs and VBOs.

/*---------------------------------------------*/

void PrepareStaticSceneObjects()
{
	vboSceneObjects.CreateVBO();

	int iSubDivY, iSubDivZ;
	// Read how detailed the sphere should be from a file
	FILE* fp = fopen("sphere.ini", "r");
	fscanf(fp, "%d %d", &iSubDivY, &iSubDivZ);
	fclose(fp);
	iSphereFaces = GenerateSphere(vboSceneObjects, fCubeHalfSize/6.0f, iSubDivY, iSubDivZ);
	

	glm::vec2 vTexCoords[] = 
	{
		glm::vec2(0.0f, fCubeHalfSize*0.1f),
		glm::vec2(fCubeHalfSize*0.1f, fCubeHalfSize*0.1f),
		glm::vec2(fCubeHalfSize*0.1f, 0.0f),
		glm::vec2(0.0f, 0.0f)
	};

	int indices[] = {0, 3, 1, 1, 3, 2};

	FOR(i, 2)
	{
		float fSign = i ? -1.0f : 1.0f;
		glm::vec3 vNormal(0.0f, 1.0f, 0.0f);
		glm::vec3 vQuad[] = 
		{
			glm::vec3(-fCubeHalfSize, -fCubeHalfSize+fCubeHalfSize*(i+1)*2.0f/3.0f, -fCubeHalfSize),
			glm::vec3(fCubeHalfSize, -fCubeHalfSize+fCubeHalfSize*(i+1)*2.0f/3.0f, -fCubeHalfSize),
			glm::vec3(fCubeHalfSize, -fCubeHalfSize+fCubeHalfSize*(i+1)*2.0f/3.0f, fCubeHalfSize),
			glm::vec3(-fCubeHalfSize, -fCubeHalfSize+fCubeHalfSize*(i+1)*2.0f/3.0f, fCubeHalfSize),
		};

		FOR(j, 6)
		{
			int k = indices[j];
			vboSceneObjects.AddData(&vQuad[k], sizeof(glm::vec3));
			vboSceneObjects.AddData(&vTexCoords[k], sizeof(glm::vec2));
			vboSceneObjects.AddData(&vNormal, sizeof(glm::vec3));
		}
	}

	FOR(i, 2)
	{
		float fSign = i ? -1.0f : 1.0f;
		glm::vec3 vNormal(1.0f, 0.0f, 0.0f);
		glm::vec3 vQuad[] = 
		{
			glm::vec3(-fCubeHalfSize+fCubeHalfSize*(i+1)*2.0f/3.0f, fCubeHalfSize, -fCubeHalfSize),
			glm::vec3(-fCubeHalfSize+fCubeHalfSize*(i+1)*2.0f/3.0f, fCubeHalfSize, fCubeHalfSize),
			glm::vec3(-fCubeHalfSize+fCubeHalfSize*(i+1)*2.0f/3.0f, -fCubeHalfSize, fCubeHalfSize),
			glm::vec3(-fCubeHalfSize+fCubeHalfSize*(i+1)*2.0f/3.0f, -fCubeHalfSize, -fCubeHalfSize),
		};

		FOR(j, 6)
		{
			int k = indices[j];
			vboSceneObjects.AddData(&vQuad[k], sizeof(glm::vec3));
			vboSceneObjects.AddData(&vTexCoords[k], sizeof(glm::vec2));
			vboSceneObjects.AddData(&vNormal, sizeof(glm::vec3));
		}
	}


	FOR(i, 2)
	{
		float fSign = i ? -1.0f : 1.0f;
		glm::vec3 vNormal(0.0f, 0.0f, 1.0f);
		glm::vec3 vQuad[] = 
		{
			glm::vec3(-fCubeHalfSize, fCubeHalfSize, -fCubeHalfSize+fCubeHalfSize*(i+1)*2.0f/3.0f),
			glm::vec3(fCubeHalfSize, fCubeHalfSize, -fCubeHalfSize+fCubeHalfSize*(i+1)*2.0f/3.0f),
			glm::vec3(fCubeHalfSize, -fCubeHalfSize, -fCubeHalfSize+fCubeHalfSize*(i+1)*2.0f/3.0f),
			glm::vec3(-fCubeHalfSize, -fCubeHalfSize, -fCubeHalfSize+fCubeHalfSize*(i+1)*2.0f/3.0f),
		};

		FOR(j, 6)
		{
			int k = indices[j];
			vboSceneObjects.AddData(&vQuad[k], sizeof(glm::vec3));
			vboSceneObjects.AddData(&vTexCoords[k], sizeof(glm::vec2));
			vboSceneObjects.AddData(&vNormal, sizeof(glm::vec3));
		}
	}

	glm::vec3 vNormal(0.0f, 1.0f, 0.0f);
	FOR(i, 6)
	{
		vboSceneObjects.AddData(&vGround[i], sizeof(glm::vec3));
		vboSceneObjects.AddData(&vGroundTexCoords[i], sizeof(glm::vec2));
		vboSceneObjects.AddData(&vNormal, sizeof(glm::vec3));
	}


	glGenVertexArrays(1, &uiVAOSceneObjects);
	glBindVertexArray(uiVAOSceneObjects);
	vboSceneObjects.BindVBO();
	vboSceneObjects.UploadDataToGPU(GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)));


	vboOccluders.CreateVBO();
	vboOccluders.AddData(vCubeVertices, 36*sizeof(glm::vec3));
	glGenVertexArrays(1, &uiVAOOccluders);
	glBindVertexArray(uiVAOOccluders);
	vboOccluders.BindVBO();
	vboOccluders.UploadDataToGPU(GL_STATIC_DRAW);


	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
}