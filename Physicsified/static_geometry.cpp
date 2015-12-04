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

GLint GenerateCapsule(CVertexBufferObject &vboDest, GLfloat fRadius, GLfloat fHeight, GLint iSubDivAround, GLfloat fMapU, GLfloat fMapV)
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

	GLfloat fSigns[] = {1.0f, -1.0f};

	FOR(i, 2)
	{
		fCurAngleAround = 0.0f;
		iStepsAround = 1;
		GLfloat fs = fSigns[i];

		while(iStepsAround <= iSubDivAround)
		{
			GLfloat fSineAround = sin(fCurAngleAround/180.0f*PI);
			GLfloat fCosineAround = cos(fCurAngleAround/180.0f*PI);
			glm::vec3 vDir1(fCosineAround, 0.0f, fSineAround);
			GLfloat fNextAngleAround = fCurAngleAround+fAddAngleAround;
			GLfloat fNextSineAround = sin(fNextAngleAround/180.0f*PI);
			GLfloat fNextCosineAround = cos(fNextAngleAround/180.0f*PI);
			glm::vec3 vDir2(fNextCosineAround, 0.0f, fNextSineAround);


			GLint iStepsHalfSphere = 1;
			GLfloat fAngleHalfSphere = 0.0f;
			GLfloat fAngleHalfStep = 90.0f/10.0f;
			while(iStepsHalfSphere <= 10)
			{
				GLfloat fNextAngleHalfSphere = fAngleHalfSphere + fAngleHalfStep;

				GLfloat fSineExtend = sin(fAngleHalfSphere/180.0f*PI);
				GLfloat fNextSineExtend = sin(fNextAngleHalfSphere/180.0f*PI);
				GLfloat fCosineExtend = cos(fAngleHalfSphere/180.0f*PI);
				GLfloat fNextCosineExtend = cos(fNextAngleHalfSphere/180.0f*PI);

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

				GLint iIndices[] = {0, 1, 2, 2, 3, 0};

				FOR(i, 6)
				{
					GLint index = iIndices[i];
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

GLint iCapsuleFaces;


GLvoid LoadAllStaticObjects(CVertexBufferObject& dstVBO, GLuint& dstVAO)
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