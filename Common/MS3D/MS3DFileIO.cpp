

#include <iostream>
#include <cstdio>
#include <cstring>
#include <set>
#include <vector>

#include "MS3DFile.h"
#include "MS3DFileI.h"

#include "utils.h"
#include "image.h"
//#include "Textures.h"



#define MAKEDWORD(a, b)      ((unsigned int)(((word)(a)) | ((word)((word)(b))) << 16))

bool CMS3DFile::LoadFromFile(const char* lpszFileName)
{
	FILE *fp = fopen(lpszFileName, "rb");
	if (!fp)
		return false;

	size_t i;
	ms3d_header_t header;
	fread(&header, 1, sizeof(ms3d_header_t), fp);

	if (strncmp(header.id, "MS3D000000", 10) != 0)
		return false;

	if (header.version != 4)
		return false;

	// vertices
	word nNumVertices;
	fread(&nNumVertices, 1, sizeof(word), fp);
	m_pImpl->m_vVertices.resize(nNumVertices);
	fread(&m_pImpl->m_vVertices[0], nNumVertices, sizeof(ms3d_vertex_t), fp);

	// triangles
	word nNumTriangles;
	fread(&nNumTriangles, 1, sizeof(word), fp);
	m_pImpl->m_vTriangles.resize(nNumTriangles);
	fread(&m_pImpl->m_vTriangles[0], nNumTriangles, sizeof(ms3d_triangle_t), fp);

	// edges
	std::set<unsigned int> setEdgePair;
	for (i = 0; i < m_pImpl->m_vTriangles.size(); i++)
	{
		word a, b;
		a = m_pImpl->m_vTriangles[i].vertexIndices[0];
		b = m_pImpl->m_vTriangles[i].vertexIndices[1];
		if (a > b)
			std::swap(a, b);
		if (setEdgePair.find(MAKEDWORD(a, b)) == setEdgePair.end())
			setEdgePair.insert(MAKEDWORD(a, b));

		a = m_pImpl->m_vTriangles[i].vertexIndices[1];
		b = m_pImpl->m_vTriangles[i].vertexIndices[2];
		if (a > b)
			std::swap(a, b);
		if (setEdgePair.find(MAKEDWORD(a, b)) == setEdgePair.end())
			setEdgePair.insert(MAKEDWORD(a, b));

		a = m_pImpl->m_vTriangles[i].vertexIndices[2];
		b = m_pImpl->m_vTriangles[i].vertexIndices[0];
		if (a > b)
			std::swap(a, b);
		if (setEdgePair.find(MAKEDWORD(a, b)) == setEdgePair.end())
			setEdgePair.insert(MAKEDWORD(a, b));
	}

	for(std::set<unsigned int>::iterator it = setEdgePair.begin(); it != setEdgePair.end(); ++it)
	{
		unsigned int EdgePair = *it;
		ms3d_edge_t Edge;
		Edge.edgeIndices[0] = (word) EdgePair;
		Edge.edgeIndices[1] = (word) ((EdgePair >> 16) & 0xFFFF);
		m_pImpl->m_vEdges.push_back(Edge);
	}

	// groups
	word nNumGroups;
	fread(&nNumGroups, 1, sizeof(word), fp);
	m_pImpl->m_vGroups.resize(nNumGroups);
	for (i = 0; i < nNumGroups; i++)
	{
		fread(&m_pImpl->m_vGroups[i].m_Header.m_flags, 1, sizeof(byte), fp);
		fread(&m_pImpl->m_vGroups[i].m_Header.m_name, 32, sizeof(char), fp);
		fread(&m_pImpl->m_vGroups[i].m_Header.m_numtriangles, 1, sizeof(word), fp);
		m_pImpl->m_vGroups[i].triangleIndices.resize(m_pImpl->m_vGroups[i].m_Header.m_numtriangles);// = new word[m_pImpl->m_vGroups[i].m_Header.m_numtriangles];
		fread(&m_pImpl->m_vGroups[i].triangleIndices[0], m_pImpl->m_vGroups[i].m_Header.m_numtriangles, sizeof(word), fp);
		fread(&m_pImpl->m_vGroups[i].materialIndex, 1, sizeof(char), fp);
	}

	// materials
	word nNumMaterials;
	fread(&nNumMaterials, 1, sizeof(word), fp);
	m_pImpl->m_vMaterials.resize(nNumMaterials);
	fread(&m_pImpl->m_vMaterials[0], nNumMaterials, sizeof(ms3d_material_t), fp);

	fread(&m_pImpl->m_fAnimationFPS, 1, sizeof(float), fp);
	fread(&m_pImpl->m_fCurrentTime, 1, sizeof(float), fp);
	fread(&m_pImpl->m_iTotalFrames, 1, sizeof(int), fp);

	// joints
	word nNumJoints;
	fread(&nNumJoints, 1, sizeof(word), fp);
	m_pImpl->m_vJoints.resize(nNumJoints);
	for (i = 0; i < nNumJoints; i++)
	{
		fread(&m_pImpl->m_vJoints[i].m_Header.m_flags, 1, sizeof(byte), fp);
		fread(&m_pImpl->m_vJoints[i].m_Header.m_name, 32, sizeof(char), fp);
		fread(&m_pImpl->m_vJoints[i].m_Header.m_parentName, 32, sizeof(char), fp);
		fread(&m_pImpl->m_vJoints[i].m_Header.m_rotation, 3, sizeof(float), fp);
		fread(&m_pImpl->m_vJoints[i].m_Header.m_position, 3, sizeof(float), fp);
		fread(&m_pImpl->m_vJoints[i].m_Header.m_numKeyFramesRot, 1, sizeof(word), fp);
		fread(&m_pImpl->m_vJoints[i].m_Header.m_numKeyFramesTrans, 1, sizeof(word), fp);
		m_pImpl->m_vJoints[i].keyFramesRot.resize(m_pImpl->m_vJoints[i].m_Header.m_numKeyFramesRot);// = new ms3d_keyframe_rot_t[m_pImpl->m_vJoints[i].m_Header.m_numKeyFramesRot];
		m_pImpl->m_vJoints[i].keyFramesTrans.resize(m_pImpl->m_vJoints[i].m_Header.m_numKeyFramesTrans);// = new ms3d_keyframe_pos_t[m_pImpl->m_vJoints[i].m_Header.m_numKeyFramesTrans];
		if( !m_pImpl->m_vJoints[i].keyFramesRot.empty() )
			fread(&m_pImpl->m_vJoints[i].keyFramesRot[0], m_pImpl->m_vJoints[i].m_Header.m_numKeyFramesRot, sizeof(ms3d_keyframe_rot_t), fp);
		if (!m_pImpl->m_vJoints[i].keyFramesTrans.empty())
			fread(&m_pImpl->m_vJoints[i].keyFramesTrans[0], m_pImpl->m_vJoints[i].m_Header.m_numKeyFramesTrans, sizeof(ms3d_keyframe_pos_t), fp);
	}

	fclose(fp);


	//Finally we must read all the textures and save their opengl ids
	m_pImpl->m_vTextures.resize(nNumMaterials);//One texture per material (if no texture is used id=0)
	std::string folderPath(lpszFileName);
    std::string texturePath("./");
	size_t pos = max( folderPath.find_last_of("/") + 1, folderPath.find_last_of("\\") + 1 );
    folderPath = folderPath.substr(0, pos);


	for(int i=0; i<nNumMaterials; i++){
		if(!strlen(m_pImpl->m_vMaterials[i].texture)){
			m_pImpl->m_vTextures[i] = 0;
		}else{
			texturePath.assign(folderPath);
			texturePath.append(m_pImpl->m_vMaterials[i].texture);
			GLuint Texture;
			int W = 0, H = 0;
			GLenum Format;
			CreateTextureFromFileGL(texturePath.c_str(), Texture, W, H, Format, GL_LINEAR, GL_LINEAR, false);
			m_pImpl->m_vTextures[i] = Texture;// LoadGLTexture(texturePath.c_str());
		}
	}
	
	return true;
}

bool CMS3DFile::LoadFromFileFast(const char* lpszFileName)
{
	char* data = nullptr;
	if ( !OpenAndReadFile(lpszFileName, &data)  ){
		IO_TRACE("Error open file %s\n", lpszFileName);
		return false;
	}
	char* pos = data;
	/*FILE *fp = fopen(lpszFileName, "rb");
	if (!fp)
		return false;*/
	size_t i;
	ms3d_header_t* header = (ms3d_header_t*)pos;
	pos += sizeof(ms3d_header_t);
	//fread(&header, 1, sizeof(ms3d_header_t), fp);

	if (strncmp(header->id, "MS3D000000", 10) != 0)
		return false;

	if (header->version != 4)
		return false;

	// vertices
	word* nNumVertices = (word*)pos;
	pos += sizeof(word);
	//fread(&nNumVertices, 1, sizeof(word), fp);
	ms3d_vertex_t* vertexes = (ms3d_vertex_t*)pos;
	m_pImpl->m_vVertices.assign(vertexes, vertexes + *nNumVertices);
	pos += sizeof(ms3d_vertex_t)*(*nNumVertices);
	//m_pImpl->m_vVertices.resize(*nNumVertices);
	//fread(&m_pImpl->m_vVertices[0], nNumVertices, sizeof(ms3d_vertex_t), fp);

	// triangles
	word* nNumTriangles = (word*)pos;
	pos += sizeof(word);
	ms3d_triangle_t* triangles = (ms3d_triangle_t*)pos;
	m_pImpl->m_vTriangles.assign(triangles, triangles + *nNumTriangles);
	pos += sizeof(ms3d_triangle_t)*(*nNumTriangles);
	//fread(&nNumTriangles, 1, sizeof(word), fp);
	//m_pImpl->m_vTriangles.resize(nNumTriangles);
	//fread(&m_pImpl->m_vTriangles[0], nNumTriangles, sizeof(ms3d_triangle_t), fp);

	// edges
	std::set<unsigned int> setEdgePair;
	for (i = 0; i < m_pImpl->m_vTriangles.size(); i++)
	{
		word a, b;
		a = m_pImpl->m_vTriangles[i].vertexIndices[0];
		b = m_pImpl->m_vTriangles[i].vertexIndices[1];
		if (a > b)
			std::swap(a, b);
		if (setEdgePair.find(MAKEDWORD(a, b)) == setEdgePair.end())
			setEdgePair.insert(MAKEDWORD(a, b));

		a = m_pImpl->m_vTriangles[i].vertexIndices[1];
		b = m_pImpl->m_vTriangles[i].vertexIndices[2];
		if (a > b)
			std::swap(a, b);
		if (setEdgePair.find(MAKEDWORD(a, b)) == setEdgePair.end())
			setEdgePair.insert(MAKEDWORD(a, b));

		a = m_pImpl->m_vTriangles[i].vertexIndices[2];
		b = m_pImpl->m_vTriangles[i].vertexIndices[0];
		if (a > b)
			std::swap(a, b);
		if (setEdgePair.find(MAKEDWORD(a, b)) == setEdgePair.end())
			setEdgePair.insert(MAKEDWORD(a, b));
	}

	for (std::set<unsigned int>::iterator it = setEdgePair.begin(); it != setEdgePair.end(); ++it)
	{
		unsigned int EdgePair = *it;
		ms3d_edge_t Edge;
		Edge.edgeIndices[0] = (word)EdgePair;
		Edge.edgeIndices[1] = (word)((EdgePair >> 16) & 0xFFFF);
		m_pImpl->m_vEdges.push_back(Edge);
	}

	// groups
	word* nNumGroups = (word*)pos;
	pos += sizeof(word);
	//fread(&nNumGroups, 1, sizeof(word), fp);
	m_pImpl->m_vGroups.resize(*nNumGroups);
	for( auto& group : m_pImpl->m_vGroups)
//	for (i = 0; i < *nNumGroups; i++)
	{
		SGroupHeader* header  = (SGroupHeader*)pos;
		pos += sizeof(SGroupHeader);
		group.m_Header = *header;
		/*fread(&m_pImpl->m_vGroups[i].flags, 1, sizeof(byte), fp);
		fread(&m_pImpl->m_vGroups[i].name, 32, sizeof(char), fp);
		fread(&m_pImpl->m_vGroups[i].numtriangles, 1, sizeof(word), fp);*/
		word* ind = (word*)pos;
		pos += sizeof(word)*header->m_numtriangles;
		group.triangleIndices.assign( ind, ind + header->m_numtriangles );
		//m_pImpl->m_vGroups[i].triangleIndices = new word[m_pImpl->m_vGroups[i].numtriangles];
		//fread(m_pImpl->m_vGroups[i].triangleIndices, m_pImpl->m_vGroups[i].numtriangles, sizeof(word), fp);
		group.materialIndex = *pos; pos++;
		//fread(&m_pImpl->m_vGroups[i].materialIndex, 1, sizeof(char), fp);
	}

	// materials
	word* nNumMaterials = (word*)pos;
	pos += sizeof(word);
	//fread(&nNumMaterials, 1, sizeof(word), fp);
	//m_pImpl->m_vMaterials.resize(*nNumMaterials);
	ms3d_material_t* mat = (ms3d_material_t*)pos;
	m_pImpl->m_vMaterials.assign( mat, mat + *nNumMaterials);
	pos += sizeof(ms3d_material_t)*(*nNumMaterials);
	//fread(&m_pImpl->m_vMaterials[0], nNumMaterials, sizeof(ms3d_material_t), fp);
	m_pImpl->m_fAnimationFPS = *(float*)pos; pos += sizeof(float);
	m_pImpl->m_fCurrentTime = *(float*)pos; pos += sizeof(float);
	m_pImpl->m_iTotalFrames = *(int*)pos; pos += sizeof(int);
	/*fread(&m_pImpl->m_fAnimationFPS, 1, sizeof(float), fp);
	fread(&m_pImpl->m_fCurrentTime, 1, sizeof(float), fp);
	fread(&m_pImpl->m_iTotalFrames, 1, sizeof(int), fp);*/

	// joints
	word* nNumJoints = (word*)pos;
	pos += sizeof(word);
	//fread(&nNumJoints, 1, sizeof(word), fp);
	m_pImpl->m_vJoints.resize(*nNumJoints);
	for( auto& joint : m_pImpl->m_vJoints)
	//for (i = 0; i < *nNumJoints; i++)
	{
		joint.m_Header = *(SJointHeader*)pos;
		pos += sizeof(SJointHeader);
		/*fread(&m_pImpl->m_vJoints[i].flags, 1, sizeof(byte), fp);
		fread(&m_pImpl->m_vJoints[i].name, 32, sizeof(char), fp);
		fread(&m_pImpl->m_vJoints[i].parentName, 32, sizeof(char), fp);
		fread(&m_pImpl->m_vJoints[i].rotation, 3, sizeof(float), fp);
		fread(&m_pImpl->m_vJoints[i].position, 3, sizeof(float), fp);
		fread(&m_pImpl->m_vJoints[i].numKeyFramesRot, 1, sizeof(word), fp);
		fread(&m_pImpl->m_vJoints[i].numKeyFramesTrans, 1, sizeof(word), fp);*/
		ms3d_keyframe_rot_t* rotStart = (ms3d_keyframe_rot_t*)pos;
		joint.keyFramesRot.assign( rotStart, rotStart + joint.m_Header.m_numKeyFramesRot);// = new ms3d_keyframe_rot_t[m_pImpl->m_vJoints[i].numKeyFramesRot];
		pos += sizeof(ms3d_keyframe_rot_t)*joint.m_Header.m_numKeyFramesRot;
		ms3d_keyframe_pos_t* posStart = (ms3d_keyframe_pos_t*)pos;
		joint.keyFramesTrans.assign(posStart, posStart + joint.m_Header.m_numKeyFramesTrans);// = new ms3d_keyframe_pos_t[m_pImpl->m_vJoints[i].numKeyFramesTrans];
		pos += sizeof(ms3d_keyframe_pos_t)*joint.m_Header.m_numKeyFramesTrans;
		//fread(m_pImpl->m_vJoints[i].keyFramesRot, m_pImpl->m_vJoints[i].numKeyFramesRot, sizeof(ms3d_keyframe_rot_t), fp);
		//fread(m_pImpl->m_vJoints[i].keyFramesTrans, m_pImpl->m_vJoints[i].numKeyFramesTrans, sizeof(ms3d_keyframe_pos_t), fp);
	}

	//Finally we must read all the textures and save their opengl ids
	m_pImpl->m_vTextures.resize(*nNumMaterials);//One texture per material (if no texture is used id=0)
	std::string folderPath(lpszFileName);
	std::string texturePath("./");
	size_t strpos = max(folderPath.find_last_of("/") + 1, folderPath.find_last_of("\\") + 1);
	folderPath = folderPath.substr(0, strpos);


	for (int i = 0; i < *nNumMaterials ; i++) {
		if (!strlen(m_pImpl->m_vMaterials[i].texture)) {
			m_pImpl->m_vTextures[i] = 0;
		}
		else {
			texturePath.assign(folderPath);
			texturePath.append(m_pImpl->m_vMaterials[i].texture);
			GLuint Texture;
			int W = 0, H = 0;
			GLenum Format;
			CreateTextureFromFileGL(texturePath.c_str(), Texture, W, H, Format, GL_LINEAR, GL_LINEAR, false);
			m_pImpl->m_vTextures[i] = Texture;// LoadGLTexture(texturePath.c_str());
		}
	}

	return true;
}

bool CMS3DFile::SaveToFile(const char* lpszFileName)
{
	FILE *fp = fopen(lpszFileName, "w");
	if (!fp)
		return false;

	//header
	ms3d_header_t header;
	header.id[0] = 'M';
	header.id[1] = 'S';
	header.id[2] = '3';
	header.id[3] = 'D';
	header.id[4] = '0';
	header.id[5] = '0';
	header.id[6] = '0';
	header.id[7] = '0';
	header.id[8] = '0';
	header.id[9] = '0';
	header.version = 4;
	fwrite(&header, sizeof(ms3d_header_t), 1, fp);

	// vertices
	word nNumVertices = m_pImpl->m_vVertices.size();
	fwrite(&nNumVertices, sizeof(word), 1, fp);
	fwrite(&m_pImpl->m_vVertices[0], sizeof(ms3d_vertex_t), nNumVertices, fp);

	
	// triangles
	word nNumTriangles = m_pImpl->m_vTriangles.size();
	fwrite(&nNumTriangles, sizeof(word), 1, fp);
	fwrite(&m_pImpl->m_vTriangles[0], sizeof(ms3d_triangle_t), nNumTriangles, fp);	
	
	
	// groups
	word nNumGroups = m_pImpl->m_vGroups.size();
	fwrite(&nNumGroups, sizeof(word), 1, fp);

	word i;
	for (i = 0; i < nNumGroups; i++)
	{
		fwrite(&m_pImpl->m_vGroups[i].m_Header.m_flags, sizeof(byte), 1, fp);
		fwrite(&m_pImpl->m_vGroups[i].m_Header.m_name, sizeof(char), 32, fp);
		fwrite(&m_pImpl->m_vGroups[i].m_Header.m_numtriangles, sizeof(word), 1, fp);
		fwrite(&m_pImpl->m_vGroups[i].triangleIndices[0], sizeof(word), m_pImpl->m_vGroups[i].m_Header.m_numtriangles, fp);
		fwrite(&m_pImpl->m_vGroups[i].materialIndex, sizeof(char), 1, fp);
	}

	
	// materials
	word nNumMaterials = m_pImpl->m_vMaterials.size();
	fwrite(&nNumMaterials, sizeof(word), 1, fp);
	fwrite(&m_pImpl->m_vMaterials[0], sizeof(ms3d_material_t), nNumMaterials, fp);

	
	fwrite(&m_pImpl->m_fAnimationFPS, sizeof(float), 1, fp);
	fwrite(&m_pImpl->m_fCurrentTime, sizeof(float), 1, fp);
	fwrite(&m_pImpl->m_iTotalFrames, sizeof(int), 1, fp);
	

	// joints
	word nNumJoints = m_pImpl->m_vJoints.size();
	fwrite(&nNumJoints, sizeof(word), 1, fp);
	for (i = 0; i < nNumJoints; i++)
	{
		fwrite(&m_pImpl->m_vJoints[i].m_Header.m_flags, sizeof(byte), 1, fp);
		fwrite(&m_pImpl->m_vJoints[i].m_Header.m_name, sizeof(char), 32, fp);
		fwrite(&m_pImpl->m_vJoints[i].m_Header.m_parentName, sizeof(char), 32, fp);
		fwrite(&m_pImpl->m_vJoints[i].m_Header.m_rotation, sizeof(float), 3, fp);
		fwrite(&m_pImpl->m_vJoints[i].m_Header.m_position, sizeof(float), 3, fp);
		fwrite(&m_pImpl->m_vJoints[i].m_Header.m_numKeyFramesRot, sizeof(word), 1, fp);
		fwrite(&m_pImpl->m_vJoints[i].m_Header.m_numKeyFramesTrans, sizeof(word), 1, fp);
		fwrite(&m_pImpl->m_vJoints[i].keyFramesRot[0], sizeof(ms3d_keyframe_rot_t), m_pImpl->m_vJoints[i].m_Header.m_numKeyFramesRot, fp);
		fwrite(&m_pImpl->m_vJoints[i].keyFramesTrans[0], sizeof(ms3d_keyframe_pos_t), m_pImpl->m_vJoints[i].m_Header.m_numKeyFramesTrans, fp);
	}

	fclose(fp);
	return true;
}
