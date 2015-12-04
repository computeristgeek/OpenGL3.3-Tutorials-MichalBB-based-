/*
	Model.h

		Abstract base class for a model. The specific extended class will render the given model. 

	Author:	Brett Porter
	Email: brettporter@yahoo.com
	Website: http://www.geocities.com/brettporter/
	Copyright (C)2000, Brett Porter. All Rights Reserved.

	This file may be used only as long as this copyright notice remains intact.
*/

#ifndef MODEL_H
#define MODEL_H

class Model
{
	public:
	char*texture;
		struct Mesh
		{
			GLint m_materialIndex;
			GLint m_numTriangles;
			GLint *m_pTriangleIndices;
		};

		//	Material properties
		struct Material
		{
			GLfloat m_ambient[4], m_diffuse[4], m_specular[4], m_emissive[4];
			GLfloat m_shininess;
			GLuGLint m_texture;
			GLchar *m_pTextureFilename;
		};

		//	Triangle structure
		struct Triangle
		{
			GLfloat m_vertexNormals[3][3];
			GLfloat m_s[3], m_t[3];
			GLint m_vertexIndices[3];
		};

		//	Vertex structure
		struct Vertex
		{
			GLchar m_boneID;	// for skeletal animation
			GLfloat m_location[3];
		};

	public:
		/*	Constructor. */
		Model();

		/*	Destructor. */
		virtual ~Model();

		/*	
			Load the model data into the private variables. 
				filename			Model filename
		*/
		virtual GLboolean loadModelData( const GLchar *filename ) = 0;

		/*
			Draw the model.
		*/
		GLvoid draw();

		/*
			Called if OpenGL context was lost and we need to reload textures, display lists, etc.
		*/
		GLvoid reloadTextures(char*name);

	protected:
		//	Meshes used
		GLint m_numMeshes;
		Mesh *m_pMeshes;

		//	Materials used
		GLint m_numMaterials;
		Material *m_pMaterials;

		//	Triangles used
		GLint m_numTriangles;
		Triangle *m_pTriangles;

		//	Vertices Used
		GLint m_numVertices;
		Vertex *m_pVertices;
};

#endif // ndef MODEL_H
