#include "common_header.h"

#include "win_OpenGLApp.h"

#include "assimp_model.h"

#pragma comment(lib, "assimp.lib")

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

CVertexBufferObject CAssimpModel::vboModelData;
CVertexBufferObject CAssimpModel::vboBumpMapData;
GLuint CAssimpModel::uiVAO;
vector<CTexture> CAssimpModel::tTextures;

/*-----------------------------------------------

Name:	GetDirectoryPath

Params:	sFilePath - guess ^^

Result: Returns directory name only from filepath.

/*---------------------------------------------*/

string GetDirectoryPath(string sFilePath)
{
	// Get directory path
	string sDirectory = "";
	RFOR(i, ESZ(sFilePath)-1)if(sFilePath[i] == '\\' || sFilePath[i] == '/')
	{
		sDirectory = sFilePath.substr(0, i+1);
		break;
	}
	return sDirectory;
}

CAssimpModel::CAssimpModel()
{
	bLoaded = false;
}

/*-----------------------------------------------

Name:	LoadModelFromFile

Params:	sFilePath - guess ^^

Result: Loads model using Assimp library.

/*---------------------------------------------*/

inline glm::vec3 aiToGlm(aiVector3D v)
{
	return glm::vec3(v.x, v.y, v.z);
}

GLboolean CAssimpModel::LoadModelFromFile(char* sFilePath, char* sTexturePath, char* sNormalTexturePath)
{
	if(vboModelData.GetBufferID() == 0)
	{
		vboModelData.CreateVBO();
		vboBumpMapData.CreateVBO();
		tTextures.reserve(50);
	}
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile( sFilePath, 
		aiProcess_CalcTangentSpace       | 
		aiProcess_Triangulate            |
		aiProcess_JoinIdenticalVertices  |
		aiProcess_SortByPType);

	if(!scene)
	{
		MessageBox(appMain.hWnd, "Couldn't load model ", "Error Importing Asset", MB_ICONERROR);
		return false;
	}

	const GLint iVertexTotalSize = sizeof(aiVector3D)*2+sizeof(aiVector2D);
	
	GLint iTotalVertices = 0;

	FOR(i, scene->mNumMeshes)
	{
		aiMesh* mesh = scene->mMeshes[i];
		GLint iMeshFaces = mesh->mNumFaces;
		iMaterialIndices.push_back(mesh->mMaterialIndex);
		GLint iSizeBefore = vboModelData.GetCurrentSize();
		iMeshStartIndices.push_back(iSizeBefore/iVertexTotalSize);
		FOR(j, iMeshFaces)
		{
			const aiFace& face = mesh->mFaces[j];

			glm::vec3 tangent;
			glm::vec3 bitangent;

			if(sNormalTexturePath)
			{
				glm::vec3 & v0 = aiToGlm(mesh->mVertices[face.mIndices[0]]);
				glm::vec3 & v1 = aiToGlm(mesh->mVertices[face.mIndices[1]]);
				glm::vec3 & v2 = aiToGlm(mesh->mVertices[face.mIndices[2]]);

				// Shortcuts for UVs
				glm::vec3 & uv0 = aiToGlm(mesh->mTextureCoords[0][face.mIndices[0]]);
				glm::vec3 & uv1 = aiToGlm(mesh->mTextureCoords[0][face.mIndices[1]]);
				glm::vec3 & uv2 = aiToGlm(mesh->mTextureCoords[0][face.mIndices[2]]);

				// Edges of the triangle : position delta
				glm::vec3 vPosDif1 = v1 - v0;
				glm::vec3 vPosDif2 = v2 - v0;

				// UV delta
				glm::vec3 vTexCoordDif1 = uv1 - uv0;
				glm::vec3 vTexCoordDif2 = uv2 - uv0;

				GLfloat r = 1.0f / (vTexCoordDif1.x * vTexCoordDif2.y - vTexCoordDif1.y * vTexCoordDif2.x);
				tangent = glm::normalize((vPosDif1 * vTexCoordDif2.y  - vPosDif2 * vTexCoordDif1.y)*r);
				bitangent = glm::normalize((vPosDif2 * vTexCoordDif1.x  - vPosDif1 * vTexCoordDif2.x)*r);
			}

			FOR(k, 3)
			{
				aiVector3D pos = mesh->mVertices[face.mIndices[k]];
				aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[k]];
				aiVector3D normal = mesh->HasNormals() ? mesh->mNormals[face.mIndices[k]] : aiVector3D(1.0f, 1.0f, 1.0f);
				vboModelData.AddData(&pos, sizeof(aiVector3D));
				vboModelData.AddData(&uv, sizeof(aiVector2D));
				vboModelData.AddData(&normal, sizeof(aiVector3D));
				if(sNormalTexturePath)
				{
					vboBumpMapData.AddData(&tangent, sizeof(glm::vec3));
					vboBumpMapData.AddData(&bitangent, sizeof(glm::vec3));
				}
			}
		}
		GLint iMeshVertices = mesh->mNumVertices;
		iTotalVertices += iMeshVertices;
		iMeshSizes.push_back((vboModelData.GetCurrentSize()-iSizeBefore)/iVertexTotalSize);
	}
	iNumMaterials = scene->mNumMaterials;

	vector<int> materialRemap(iNumMaterials);

	FOR(i, iNumMaterials)
	{
		const aiMaterial* material = scene->mMaterials[i];
		GLint a = 5;
		GLint texIndex = 0;
		aiString path;  // filename

		string sDir = GetDirectoryPath(sFilePath);
		string sFullPath;
		if(sTexturePath == NULL)
		{
			GLboolean ok = material->GetTexture(aiTextureType_DIFFUSE, texIndex, &path) == AI_SUCCESS;
			if(!ok)ok = material->GetTexture(aiTextureType_AMBIENT, texIndex, &path) == AI_SUCCESS;
			if(!ok)ok = material->GetTexture(aiTextureType_UNKNOWN, texIndex, &path) == AI_SUCCESS;
			if(!ok)ok = material->GetTexture(aiTextureType_EMISSIVE, texIndex, &path) == AI_SUCCESS;
			if(!ok)
			{
				string sPutToPath = sFilePath;
				sPutToPath = sPutToPath.substr(sPutToPath.find_last_of("/\\")+1);
				sPutToPath = sPutToPath.substr(0, sPutToPath.find_last_of("."));
				sPutToPath += ".jpg";
				path = sPutToPath.c_str();
			}

			string sTextureName = path.data;
			sFullPath = sDir+sTextureName;
		}
		else sFullPath = sDir + string(sTexturePath);
		GLint iTexFound = -1;
		FOR(j, ESZ(tTextures))if(sFullPath == tTextures[j].GetPath())
		{
			iTexFound = j;
			break;
		}
		if(iTexFound != -1)materialRemap[i] = iTexFound;
		else
		{
			CTexture tNew;
			tNew.LoadTexture2D(sFullPath, true);
			materialRemap[i] = ESZ(tTextures);
			tNew.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
			tTextures.push_back(tNew);
		}
	}

	if(sNormalTexturePath)
	{
		bHasBumpMap = true;
		string sDir = GetDirectoryPath(sFilePath);
		tNormalMap.LoadTexture2D(sDir + string(sNormalTexturePath));
		tNormalMap.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_NEAREST);
	}

	FOR(i, ESZ(iMeshSizes))
	{
		GLint iOldIndex = iMaterialIndices[i];
		iMaterialIndices[i] = materialRemap[iOldIndex];
	}

	return bLoaded = true;
}



/*-----------------------------------------------

Name:	FinalizeVBO

Params: none

Result: Uploads all loaded model data in one global
		models' VBO.

/*---------------------------------------------*/

GLvoid CAssimpModel::FinalizeVBO()
{
	glGenVertexArrays(1, &uiVAO);
	glBindVertexArray(uiVAO);
	vboModelData.BindVBO();
	vboModelData.UploadDataToGPU(GL_STATIC_DRAW);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2*sizeof(aiVector3D)+sizeof(aiVector2D), 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(aiVector3D)+sizeof(aiVector2D), (void*)sizeof(aiVector3D));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2*sizeof(aiVector3D)+sizeof(aiVector2D), (void*)(sizeof(aiVector3D)+sizeof(aiVector2D)));

	vboBumpMapData.BindVBO();
	vboBumpMapData.UploadDataToGPU(GL_STATIC_DRAW);

	// Tangent vector
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 2*sizeof(aiVector3D), 0);
	// Bitangent vector
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 2*sizeof(aiVector3D), (void*)(sizeof(aiVector3D)));
}

/*-----------------------------------------------

Name:	BindModelsVAO

Params: none

Result: Binds VAO of models with their VBO.

/*---------------------------------------------*/

GLvoid CAssimpModel::BindModelsVAO()
{
	glBindVertexArray(uiVAO);
}

/*-----------------------------------------------

Name:	RenderModel

Params: none

Result: Guess what it does ^^.

/*---------------------------------------------*/

GLvoid CAssimpModel::RenderModel(GLenum RenderMode)
{
	if(!bLoaded)return;
	GLint iNumMeshes = ESZ(iMeshSizes);
	FOR(i, iNumMeshes)
	{
		GLint iMatIndex = iMaterialIndices[i];
		tTextures[iMatIndex].BindTexture(0);
		if(bHasBumpMap)
			tNormalMap.BindTexture(1);
		glDrawArrays(RenderMode, iMeshStartIndices[i], iMeshSizes[i]);
	}
}