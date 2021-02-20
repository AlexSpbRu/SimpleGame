#pragma once

#include <vector>

class CMS3DFileI    {
public:
	std::vector<ms3d_vertex_t>		m_vVertices;
	std::vector<ms3d_triangle_t>	m_vTriangles;
	std::vector<ms3d_edge_t>		m_vEdges;
	std::vector<ms3d_group_t>		m_vGroups;
	std::vector<ms3d_material_t>	m_vMaterials;
	std::vector<int>				m_vTextures; //Contains the ids of the OpenGL textures, Indexes match between this and arrMaterials
	float							m_fAnimationFPS = 24.0f;
	float							m_fCurrentTime = 0.0f;
	int								m_iTotalFrames = 0;
	std::vector<ms3d_joint_t>		m_vJoints;

public:
	CMS3DFileI()//:	fAnimationFPS(24.0f),fCurrentTime(0.0f),iTotalFrames(0)
	{
	}
};
