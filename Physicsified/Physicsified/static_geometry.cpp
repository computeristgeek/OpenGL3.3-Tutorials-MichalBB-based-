#include "common_header.h"

#include "static_geometry.h"



glm::vec3 vCubeVertices[36] = 
{
	// Front face
	glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, 1.0f, 1.0f),
	// Back face
	glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, -1.0f),
	// Left face
	glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, 1.0f, -1.0f),
	// Right face
	glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
	// Top face
	glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 1.0f, -1.0f),
	// Bottom face
	glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, 1.0f),
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
	glm::vec3(-500, 1, -500), glm::vec3(-500, 1, 500), glm::vec3(500, 1, 500), glm::vec3(500, 1, 500), glm::vec3(500, 1, -500), glm::vec3(-500, 1, -500)
};

glm::vec2 vGroundTC[6] = 
{
	glm::vec2(0, 20), glm::vec2(0, 0), glm::vec2(20, 0), glm::vec2(20, 0), glm::vec2(20, 20), glm::vec2(0, 20)
};

int GenerateCapsule(CVertexBufferObject &vboDest, float fRadius, float fHeight, int iSubDivAround, float fMapU, float fMapV)
{
	float fAddAngleAround = 360.0f/(float)(iSubDivAround-1);

	float fCurAngleAround = 0.0f;
	int iStepsAround = 1;
	const float PI = float(atan(1.0)*4.0);

	int iFacesAdded = 0;

	while(iStepsAround <= iSubDivAround)
	{
		float fSineAround = sin(fCurAngleAround/180.0f*PI);
		float fCosineAround = cos(fCurAngleAround/180.0f*PI);
		glm::vec3 vDir1(fCosineAround, 0.0f, fSineAround);
		float fNextAngleAround = fCurAngleAround+fAddAngleAround;
		float fNextSineAround = sin(fNextAngleAround/180.0f*PI);
		float fNextCosineAround = cos(fNextAngleAround/180.0f*PI);
		glm::vec3 vDir2(fNextCosineAround, 0.0f, fNextSineAround);

		glm::vec3 vQuadPoints[] = 
		{
			glm::vec3(0.0f, fHeight/2.0f, 0.0f) + vDir1*fRadius,
			vDir1*fRadius,
			vDir2*fRadius,
			glm::vec3(0.0f, fHeight/2.0f, 0.0f) + vDir2*fRadius
		};

		vQuadPoints[1].y -= fHeight/2.0f;
		vQuadPoints[2].y -= fHeight/2.0f;

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

		int iIndices[] = {0, 1, 2, 2, 3, 0};

		FOR(i, 6)
		{
			int index = iIndices[i];
			vboDest.AddData(&vQuadPoints[index], sizeof(glm::vec3));
			vboDest.AddData(&vTexCoords[index], sizeof(glm::vec2));
			vboDest.AddData(&vNormals[index], sizeof(glm::vec3));
		}
		iFacesAdded += 2; // Keep count of added faces

		fCurAngleAround += fAddAngleAround;
		iStepsAround++;
	}

	float fSigns[] = {1.0f, -1.0f};

	FOR(i, 2)
	{
		fCurAngleAround = 0.0f;
		iStepsAround = 1;
		float fs = fSigns[i];

		while(iStepsAround <= iSubDivAround)
		{
			float fSineAround = sin(fCurAngleAround/180.0f*PI);
			float fCosineAround = cos(fCurAngleAround/180.0f*PI);
			glm::vec3 vDir1(fCosineAround, 0.0f, fSineAround);
			float fNextAngleAround = fCurAngleAround+fAddAngleAround;
			float fNextSineAround = sin(fNextAngleAround/180.0f*PI);
			float fNextCosineAround = cos(fNextAngleAround/180.0f*PI);
			glm::vec3 vDir2(fNextCosineAround, 0.0f, fNextSineAround);


			int iStepsHalfSphere = 1;
			float fAngleHalfSphere = 0.0f;
			float fAngleHalfStep = 90.0f/10.0f;
			while(iStepsHalfSphere <= 10)
			{
				float fNextAngleHalfSphere = fAngleHalfSphere + fAngleHalfStep;

				float fSineExtend = sin(fAngleHalfSphere/180.0f*PI);
				float fNextSineExtend = sin(fNextAngleHalfSphere/180.0f*PI);
				float fCosineExtend = cos(fAngleHalfSphere/180.0f*PI);
				float fNextCosineExtend = cos(fNextAngleHalfSphere/180.0f*PI);

				glm::vec3 vOrigin = glm::vec3(0.0f, fs*fHeight/2.0f, 0.0f);

				glm::vec3 vQuadPoints[] = 
				{
					vOrigin + vDir1*fSineExtend*fRadius,
					vOrigin + vDir1*fNextSineExtend*fRadius,
					vOrigin + vDir2*fNextSineExtend*fRadius,
					vOrigin + vDir2*fSineExtend*fRadius
				};

				vQuadPoints[0].y += fs*fRadius*fCosineExtend;
				vQuadPoints[1].y += fs*fRadius*fNextCosineExtend;
				vQuadPoints[2].y += fs*fRadius*fNextCosineExtend;
				vQuadPoints[3].y += fs*fRadius*fCosineExtend;

				glm::vec2 vTexCoords[] = 
				{
					glm::vec2(fMapU*fCurAngleAround/360.0f, fMapV*fCosineExtend),
					glm::vec2(fMapU*fCurAngleAround/360.0f, fMapV*fNextCosineExtend),
					glm::vec2(fMapU*fNextAngleAround/360.0f, fMapV*fNextCosineExtend),
					glm::vec2(fMapU*fNextAngleAround/360.0f, fMapV*fCosineExtend)
				};

				glm::vec3 vNormals[4];
				FOR(k, 4)
				{
					vNormals[k] = vQuadPoints[k]-vOrigin;
					vNormals[k] = glm::normalize(vNormals[k]);
				}

				int iIndices[] = {0, 1, 2, 2, 3, 0};

				FOR(i, 6)
				{
					int index = iIndices[i];
					vboDest.AddData(&vQuadPoints[index], sizeof(glm::vec3));
					vboDest.AddData(&vTexCoords[index], sizeof(glm::vec2));
					vboDest.AddData(&vNormals[index], sizeof(glm::vec3));
				}
				iFacesAdded += 2; // Keep count of added faces

				iStepsHalfSphere++;
				fAngleHalfSphere += fAngleHalfStep;
			}

			fCurAngleAround += fAddAngleAround;
			iStepsAround++;
		}
	}

	return iFacesAdded;
}

int iCapsuleFaces;


void LoadAllStaticObjects(CVertexBufferObject& dstVBO, UINT& dstVAO)
{
	dstVBO.CreateVBO();
	glGenVertexArrays(1, &dstVAO); // Create one VAO
	glBindVertexArray(dstVAO);

	dstVBO.BindVBO();

	// Add cube to VBO

	FOR(i, 36)
	{
		dstVBO.AddData(&vCubeVertices[i], sizeof(glm::vec3));
		dstVBO.AddData(&vCubeTexCoords[i%6], sizeof(glm::vec2));
		dstVBO.AddData(&vCubeNormals[i/6], sizeof(glm::vec3));
	}

	FOR(i, 6)
	{
		dstVBO.AddData(&vGround[i], sizeof(glm::vec3));
		dstVBO.AddData(&vGroundTC[i], sizeof(glm::vec2));
		dstVBO.AddData(&vCubeNormals[5], sizeof(glm::vec3));
	}

	iCapsuleFaces = GenerateCapsule(dstVBO, 1.0f, 1.0f, 10, 1.0f, 1.0f);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)));

	dstVBO.UploadDataToGPU(GL_STATIC_DRAW);
}