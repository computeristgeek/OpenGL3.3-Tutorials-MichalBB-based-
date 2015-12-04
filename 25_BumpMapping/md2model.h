#pragma once

typedef GLfloat vec3_t[3];

// number of precalculated normals
#define NUMVERTEXNORMALS		162

// precalculated normal vectors
#define SHADEDOT_QUANT			16

// magic number "IDP2" or 844121161
#define MD2_IDENT				(('2'<<24) + ('P'<<16) + ('D'<<8) + 'I')

// model version
#define	MD2_VERSION				8

// maximum number of vertices for a MD2 model
#define MAX_MD2_VERTS			2048

// md2 header
typedef struct
{
	GLint     ident;              // magic number. must be equal to "IDP2"
	GLint     version;            // md2 version. must be equal to 8

	GLint     skinwidth;          // width of the texture
	GLint     skinheight;         // height of the texture
	GLint     framesize;          // size of one frame in bytes

	GLint     num_skins;          // number of textures
	GLint     num_xyz;            // number of vertices
	GLint     num_st;             // number of texture coordinates
	GLint     num_tris;           // number of triangles
	GLint     num_glcmds;         // number of opengl commands
	GLint     num_frames;         // total number of frames

	GLint     ofs_skins;          // offset to skin names (64 bytes each)
	GLint     ofs_st;             // offset to s-t texture coordinates
	GLint     ofs_tris;           // offset to triangles
	GLint     ofs_frames;         // offset to frame data
	GLint     ofs_glcmds;         // offset to opengl commands
	GLint     ofs_end;            // offset to end of file

} md2_t;

// md2 compressed vertex
typedef struct
{
	unsigned char   v[3];                // compressed vertex (x, y, z) coordinates
	unsigned char   lightnormalindex;    // index to a normal vector for the lighting

} vertex_t;

// md2 frame data (vertices are stored here)
typedef struct
{
	GLfloat       scale[3];       // scale values
	GLfloat       translate[3];   // translation vector
	char        name[16];       // frame name
	vertex_t    verts[1];       // first vertex of this frame

} frame_t;

// animation data - first and last frame and FPS to know how fast should it run
typedef struct
{
	int		first_frame;			// first frame of the animation
	int		last_frame;				// number of frames
	int		fps;					// number of frames per second

} anim_t;

// status animation
typedef struct
{
	int		startframe;				// first frame
	int		endframe;				// last frame
	int		fps;					// frame per second for this animation

	float	curr_time;				// current time
	float	old_time;				// old time
	float	interpol;				// percent of interpolation

	int		type;					// animation type

	int		curr_frame;				// current frame
	int		next_frame;				// next frame

} animState_t;

// animation list
typedef enum {
	STAND,
	RUN,
	ATTACK,
	PAIN_A,
	PAIN_B,
	PAIN_C,
	JUMP,
	FLIP,
	SALUTE,
	FALLBACK,
	WAVE,
	POINTING, // Had to rename this from POINT to POINTING because of POINT class in windows.h
	CROUCH_STAND,
	CROUCH_WALK,
	CROUCH_ATTACK,
	CROUCH_PAIN,
	CROUCH_DEATH, 
	DEATH_FALLBACK,
	DEATH_FALLFORWARD,
	DEATH_FALLBACKSLOW,
	BOOM,

	MAX_ANIMATIONS

} animType_t;

extern string sMD2AnimationNames[MAX_ANIMATIONS];

#include "vertexBufferObject.h"
#include "shaders.h"
#include "texture.h"

/*****************************************************

Class:		CMD2Model

Purpose:	Handles MD2 models - loading, rendering,
			animation etc.

*****************************************************/

class CMD2Model
{
public:
	GLvoid LoadModel(char* sFilename);

	GLvoid RenderModel(animState_t* animState);

	animState_t StartAnimation(animType_t type);
	GLvoid UpdateAnimation(animState_t* animState, GLfloat fTimePassed);
	GLvoid PauseAnimation();
	GLvoid StopAnimation();

	static anim_t	animlist[21];
private:
	GLuint uiModelVAO;
	
	vector<GLuint> uiFramesBuffer;

	md2_t header;
	
	vector< vector<glm::vec3> > vVertices; // Vertices extracted for every frame
	vector <vector< GLint > > vNormals; // Normal indices extracted for every frame
	vector<int> glCommands; // Rendering OpenGL commands

	vector<CVertexBufferObject> vboFrameVertices; // All frames (keyframes) of model
	CVertexBufferObject vboTextureCoords; // Texture coords are same for all frames

	vector<int> renderModes; // Rendering modes
	vector<int> numRenderVertices; // with number of vertices

	CTexture tSkin;

	GLuint uiVAO;
};

