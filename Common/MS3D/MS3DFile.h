/*
 *	LXSR Import ms3d
 * 	adapted from source at milkshape website by E 
 *	MS3DFile.h
 *
 *  Created on: Jun 3, 2012
 */


#ifndef _MS3DFILE_H_
#define _MS3DFILE_H_

//#include <GL/glew.h>

//#include <pshpack1.h>
#include <windows.h>
#include <gl\gl.h>			
//#include <gl\glu.h>	
//#define GL_GLEXT_PROTOTYPES
#include <GL/glext.h>

#ifndef RC_INVOKED
#pragma pack(push,1)
#endif


#ifndef byte
typedef unsigned char byte;
#endif // byte

#ifndef word
typedef unsigned short word;
#endif // word

typedef struct
{
    char    id[10];                                     // always "MS3D000000"
    int     version;                                    // 4
} ms3d_header_t;

typedef struct
{
    byte    flags;                                      // SELECTED | SELECTED2 | HIDDEN
    float   vertex[3];                                  //
    char    boneId;                                     // -1 = no bone
    byte    referenceCount;
} ms3d_vertex_t;

typedef struct
{
    word    flags;                                      // SELECTED | SELECTED2 | HIDDEN
    word    vertexIndices[3];                           //
    float   vertexNormals[3][3];                        //
    GLfloat   s[3];                                       //
    GLfloat   t[3];                                       //
    byte    smoothingGroup;                             // 1 - 32
    byte    groupIndex;                                 //
} ms3d_triangle_t;

typedef struct
{
	word edgeIndices[2];
} ms3d_edge_t;

struct SGroupHeader
{
   byte            m_flags;                              // SELECTED | HIDDEN
   char            m_name[32];                           //
   word            m_numtriangles;                       //
};

typedef struct
{
	SGroupHeader			m_Header;
    std::vector< word >	    triangleIndices;			// the groups group the triangles
    char					materialIndex;              // -1 = no material
} ms3d_group_t;

typedef struct
{
    char            name[32];                           //
    float           ambient[4];                         //
    float           diffuse[4];                         //
    float           specular[4];                        //
    float           emissive[4];                        //
    float           shininess;                          // 0.0f - 128.0f
    float           transparency;                       // 0.0f - 1.0f
    char            mode;                               // 0, 1, 2 is unused now
    char            texture[128];                        // texture.bmp
    char            alphamap[128];                       // alpha.bmp

} ms3d_material_t;

typedef struct
{
    float           time;                               // time in seconds
    float           rotation[3];                        // x, y, z angles
} ms3d_keyframe_rot_t;

typedef struct
{
    float           time;                               // time in seconds
    float           position[3];                        // local position
} ms3d_keyframe_pos_t;

struct SJointHeader
{
    byte            m_flags;                              // SELECTED | DIRTY
    char            m_name[32];                           //
    char            m_parentName[32];                     //
    float           m_rotation[3];                        // local reference matrix
    float           m_position[3];

    word            m_numKeyFramesRot;                    //
    word            m_numKeyFramesTrans;                  //
};

typedef struct
{
	SJointHeader			m_Header;

	std::vector<ms3d_keyframe_rot_t>	keyFramesRot;					// local animation matrices
    std::vector<ms3d_keyframe_pos_t>	keyFramesTrans;					// local animation matrices
} ms3d_joint_t;

//#include <poppack.h>
#ifndef RC_INVOKED
#pragma pack(pop)
#endif


class CMS3DFileI;
class CMS3DFile
{
private:
	CMS3DFileI* m_pImpl = nullptr;
	GLuint* _vao = nullptr;
	GLuint* _vbo = nullptr;
	GLuint* _eab = nullptr;
	GLuint _shader;
	bool		m_bOverrideAmbient = false;
	bool		m_bOverrideDiffuse = false;
	bool		m_bOverrideSpecular = false;
	bool		m_bOverrideEmissive = false;
	
	float* _white;
	float* _black;

public:
	CMS3DFile();
	virtual ~CMS3DFile();

	bool LoadFromFile(const char* lpszFileName);
	bool LoadFromFileFast(const char* lpszFileName);
	bool SaveToFile(const char* lpszFileName);
	void Clear();

	void createRectangle(float width, float height, GLuint texture);

	void optimize();

	void prepareModel(GLuint shader);
	void unloadModel();
	void drawGL3();

	int GetNumVertices();
	void GetVertexAt(int nIndex, ms3d_vertex_t **ppVertex);
	int GetNumTriangles();
	void GetTriangleAt(int nIndex, ms3d_triangle_t **ppTriangle);
	int GetNumEdges();
	void GetEdgeAt(int nIndex, ms3d_edge_t **ppEdge);
	int GetNumGroups();
	void GetGroupAt(int nIndex, ms3d_group_t **ppGroup);
	int FindGroupByName(const char* lpszName);
	int GetNumMaterials();
	void GetMaterialAt(int nIndex, ms3d_material_t **ppMaterial);
	int GetNumJoints();
	void GetJointAt(int nIndex, ms3d_joint_t **ppJoint);
	int FindJointByName(const char* lpszName);

	float GetAnimationFPS();
	float GetCurrentTime();
	int GetTotalFrames();

	void draw31();
	void draw();
	void setMaterial(ms3d_material_t* material, int textureIndex);
	void setMaterial(int texture, ms3d_group_t* group);
	void setMaterialGL3(ms3d_material_t* material, int textureIndex);
	void setTexture(unsigned int textureIndex, int texture);

	inline void setOverrideAmbient(bool overrideAmbient) {
		m_bOverrideAmbient = overrideAmbient;
	};
	inline void setOverrideDiffuse(bool overrideDiffuse) {
		m_bOverrideDiffuse = overrideDiffuse;
	};
	inline void setOverrideSpecular(bool overrideSpecular) {
		m_bOverrideSpecular = overrideSpecular;
	};
	inline void setOverrideEmissive(bool overrideEmissive) {
		m_bOverrideEmissive = overrideEmissive;
	};

	void setMaterialEmissive(char* matName, float red, float green, float blue);
	void setMaterialTransparency(char* matName, float alpha);

	void translateModel(float x, float y, float z);

	CMS3DFile(const CMS3DFile& rhs);
	CMS3DFile& operator=(const CMS3DFile& rhs);

private:
	void mergeGroups();
	void removeUnusedMaterials();
	void prepareGroup(ms3d_group_t* group, unsigned int groupIndex);
	void drawGroup(ms3d_group_t* group);
};

#endif // _MS3DFILE_H_
