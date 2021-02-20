/*
 * 	adapted from source at milkshape website by E 
 *	MS3DFile.cpp
 *
 *  Created on: Jun 3, 2012
 */
//#pragma warning(disable : 4786)



#include <cstring>
#include <set>
#include <vector>
#include <math.h>
#include <iterator>

#define OPENGL
//#define OPENGLES20

//#ifdef OPENGL
//#include <gl/gl.h>	
//#include <gl/glext.h>
//#endif
//
//#ifdef OPENGLES20
//#include <EGL/egl.h>
//#include <GLES2/gl2.h>	
//#endif

#include "MS3DFile.h"
#include "MS3DFileI.h"
#include "utils.h"
//#include <GL/glut.h>


CMS3DFile::CMS3DFile()
{
	m_pImpl = new CMS3DFileI();

	_white = new float [4];
	_black = new float [4];

	for(int i=0;i<4;i++)
		_white[i] = 1.0;
	for(int i=0;i<3;i++)
		_black[i] = 0.0;
	_black[3] = 1.0;

}

CMS3DFile::~CMS3DFile()
{
	delete m_pImpl;
	delete _white;
	delete _black;
}


void CMS3DFile::Clear()
{
	m_pImpl->m_vVertices.clear();
	m_pImpl->m_vTriangles.clear();
	m_pImpl->m_vEdges.clear();
	m_pImpl->m_vGroups.clear();
	m_pImpl->m_vMaterials.clear();
	m_pImpl->m_vJoints.clear();
	m_pImpl->m_vTextures.clear();
}

int CMS3DFile::GetNumVertices()
{
	return (int) m_pImpl->m_vVertices.size();
}


void CMS3DFile::draw31()
{
	GLboolean texEnabled = glIsEnabled(GL_TEXTURE_2D);

	for( auto& group : m_pImpl->m_vGroups ) {
		int materialIndex = (int)group.materialIndex;
		if (materialIndex >= 0 && materialIndex <  m_pImpl->m_vMaterials.size() ) {
			glMaterialfv(GL_FRONT, GL_AMBIENT, m_pImpl->m_vMaterials[materialIndex].ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, m_pImpl->m_vMaterials[materialIndex].diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, m_pImpl->m_vMaterials[materialIndex].specular);
			glMaterialfv(GL_FRONT, GL_EMISSION, m_pImpl->m_vMaterials[materialIndex].emissive);
			glMaterialf(GL_FRONT, GL_SHININESS, m_pImpl->m_vMaterials[materialIndex].shininess);
			
			if (m_pImpl->m_vTextures[materialIndex] > 0)
			{
				glBindTexture(GL_TEXTURE_2D, m_pImpl->m_vTextures[materialIndex]);
				glEnable(GL_TEXTURE_2D);
			}
			else
				glDisable(GL_TEXTURE_2D);
		}
		else
			glDisable(GL_TEXTURE_2D);


		glBegin(GL_TRIANGLES);
		{
			for (int j = 0; j < group.m_Header.m_numtriangles ; j++)
			{
				auto triangleIndex = group.triangleIndices[j];
				auto& triangle = m_pImpl->m_vTriangles[triangleIndex];

				for (int k = 0; k < 3; k++)
				{
					int index = triangle.vertexIndices[k];

					glNormal3fv(triangle.vertexNormals[k]);
					glTexCoord2f(triangle.s[k], triangle.t[k]);
					glVertex3fv(m_pImpl->m_vVertices[index].vertex);
				}
			}
		}
		glEnd();
	}

	if (texEnabled)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);
}



void CMS3DFile::draw(){
	GLboolean texEnabled = glIsEnabled( GL_TEXTURE_2D );
	
	for(unsigned int i=0; i < m_pImpl->m_vGroups.size(); i++){
		int materialIndex = (int)m_pImpl->m_vGroups[i].materialIndex;
		if( materialIndex >= 0 ){
			setMaterial(&(m_pImpl->m_vMaterials[materialIndex]), materialIndex); 
		}else
			glDisable( GL_TEXTURE_2D );
		drawGroup(&(m_pImpl->m_vGroups[i]));
	}		

	if ( texEnabled )
		glEnable( GL_TEXTURE_2D );
	else
		glDisable( GL_TEXTURE_2D );
}


void CMS3DFile::GetVertexAt(int nIndex, ms3d_vertex_t **ppVertex)
{
	if (nIndex >= 0 && nIndex < (int)m_pImpl->m_vVertices.size())
		*ppVertex = &m_pImpl->m_vVertices[nIndex];
}

int CMS3DFile::GetNumTriangles()
{
	return (int)m_pImpl->m_vTriangles.size();
}

void CMS3DFile::GetTriangleAt(int nIndex, ms3d_triangle_t **ppTriangle)
{
	if (nIndex >= 0 && nIndex < (int)m_pImpl->m_vTriangles.size())
		*ppTriangle = &m_pImpl->m_vTriangles[nIndex];
}

int CMS3DFile::GetNumEdges()
{
	return (int)m_pImpl->m_vEdges.size();
}

void CMS3DFile::GetEdgeAt(int nIndex, ms3d_edge_t **ppEdge)
{
	if (nIndex >= 0 && nIndex < (int)m_pImpl->m_vEdges.size())
		*ppEdge = &m_pImpl->m_vEdges[nIndex];
}

int CMS3DFile::GetNumGroups()
{
	return (int)m_pImpl->m_vGroups.size();
}

void CMS3DFile::GetGroupAt(int nIndex, ms3d_group_t **ppGroup)
{
	if (nIndex >= 0 && nIndex < (int)m_pImpl->m_vGroups.size())
		*ppGroup = &m_pImpl->m_vGroups[nIndex];
}

int CMS3DFile::FindGroupByName(const char* lpszName)
{
	int i = 0;
	for (auto& group : m_pImpl->m_vGroups) {
		if (!strcmp(group.m_Header.m_name, lpszName))
			return i;
		++i;
	}
	//for (size_t i = 0; i < m_pImpl->m_vGroups.size(); i++)
		//if (!strcmp(m_pImpl->m_vGroups[i].name, lpszName))
			//return i;
	return -1;
}
int CMS3DFile::GetNumMaterials()
{
	return (int)m_pImpl->m_vMaterials.size();
}

void CMS3DFile::GetMaterialAt(int nIndex, ms3d_material_t **ppMaterial)
{
	if (nIndex >= 0 && nIndex < (int)m_pImpl->m_vMaterials.size())
		*ppMaterial = &m_pImpl->m_vMaterials[nIndex];
}

int CMS3DFile::GetNumJoints()
{
	return (int)m_pImpl->m_vJoints.size();
}

void CMS3DFile::GetJointAt(int nIndex, ms3d_joint_t **ppJoint)
{
	if (nIndex >= 0 && nIndex < (int)m_pImpl->m_vJoints.size())
		*ppJoint = &m_pImpl->m_vJoints[nIndex];
}

int CMS3DFile::FindJointByName(const char* lpszName)
{
	int i = 0;
	for (auto& joint : m_pImpl->m_vJoints) {
		if (!strcmp(joint.m_Header.m_name, lpszName))
			return i;
		++i;
	}
	/*for (size_t i = 0; i < m_pImpl->m_vJoints.size(); i++)
	{
		if (!strcmp(m_pImpl->m_vJoints[i].name, lpszName))
			return i;
	}*/

	return -1;
}

float CMS3DFile::GetAnimationFPS()
{
	return m_pImpl->m_fAnimationFPS;
}

float CMS3DFile::GetCurrentTime()
{
	return m_pImpl->m_fCurrentTime;
}

int CMS3DFile::GetTotalFrames()
{
	return m_pImpl->m_iTotalFrames;
}


void CMS3DFile::drawGL3(){
	GLboolean texEnabled = glIsEnabled( GL_TEXTURE_2D );
	
	for(unsigned int i=0; i < m_pImpl->m_vGroups.size(); i++){
		int materialIndex = (int)m_pImpl->m_vGroups[i].materialIndex;
		if( materialIndex >= 0 )
			setMaterialGL3(&(m_pImpl->m_vMaterials[materialIndex]),materialIndex); 
		else
			glDisable( GL_TEXTURE_2D );
		glBindVertexArray(_vao[i]);
		glDrawElements(GL_TRIANGLES, m_pImpl->m_vGroups[i].m_Header.m_numtriangles*3, GL_UNSIGNED_INT, 0);
	}		

	if ( texEnabled )
		glEnable( GL_TEXTURE_2D );
	else
		glDisable( GL_TEXTURE_2D );
}


void CMS3DFile::setMaterialGL3(ms3d_material_t* material, int textureIndex){
	GLint ambient = glGetUniformLocation(_shader, "ambient");
	GLint diffuse = glGetUniformLocation(_shader, "diffuse");
	GLint specular = glGetUniformLocation(_shader, "specular");
	GLint emissive = glGetUniformLocation(_shader, "emissive");
	GLint shininess = glGetUniformLocation(_shader, "shininess");
	GLint transparency = glGetUniformLocation(_shader, "transparency");
	
	glUniform4fv(ambient, 1, material->ambient);
	glUniform4fv(diffuse, 1, material->diffuse);
	glUniform4fv(specular, 1, material->specular);
	glUniform4fv(emissive, 1, material->emissive);

	glUniform1f(shininess, material->shininess);
	glUniform1f(transparency, 1-(material->transparency));
	
	if( m_pImpl->m_vTextures[textureIndex] > 0){
		glBindTexture( GL_TEXTURE_2D, m_pImpl->m_vTextures[textureIndex]);
		glEnable( GL_TEXTURE_2D );
	}else
		glDisable( GL_TEXTURE_2D );
}

void CMS3DFile::setMaterial(ms3d_material_t* material, int textureIndex){
	if(m_bOverrideAmbient)
		glMaterialfv( GL_FRONT, GL_AMBIENT, _white);
	else
		glMaterialfv( GL_FRONT, GL_AMBIENT, material->ambient);
	if(m_bOverrideDiffuse)
		glMaterialfv( GL_FRONT, GL_DIFFUSE, _white);
	else
		glMaterialfv( GL_FRONT, GL_DIFFUSE, material->diffuse );
	if(m_bOverrideSpecular)
		glMaterialfv( GL_FRONT, GL_SPECULAR, _white );
	else
		glMaterialfv( GL_FRONT, GL_SPECULAR, material->specular );
	if(m_bOverrideEmissive)
		glMaterialfv( GL_FRONT, GL_EMISSION, _black);
	else
		glMaterialfv( GL_FRONT, GL_EMISSION, material->emissive );

	glMaterialf( GL_FRONT, GL_SHININESS, material->shininess );
	if( m_pImpl->m_vTextures[textureIndex] > 0){
		glBindTexture( GL_TEXTURE_2D, m_pImpl->m_vTextures[textureIndex]);
		glEnable( GL_TEXTURE_2D );
	}else
		glDisable( GL_TEXTURE_2D );
}

//binds an opengl texture with the material information from a given group (usually to draw that same group)
void CMS3DFile::setMaterial(int texture, ms3d_group_t* group){
	ms3d_material_t* material = &(m_pImpl->m_vMaterials[group->materialIndex]);	
	if(m_bOverrideAmbient)
		glMaterialfv( GL_FRONT, GL_AMBIENT, _white);
	else
		glMaterialfv( GL_FRONT, GL_AMBIENT, material->ambient);
	if(m_bOverrideDiffuse)
		glMaterialfv( GL_FRONT, GL_DIFFUSE, _white);
	else
		glMaterialfv( GL_FRONT, GL_DIFFUSE, material->diffuse );
	if(m_bOverrideSpecular)
		glMaterialfv( GL_FRONT, GL_SPECULAR, _white );
	else
		glMaterialfv( GL_FRONT, GL_SPECULAR, material->specular );
	if(m_bOverrideEmissive)
		glMaterialfv( GL_FRONT, GL_EMISSION, _black);
	else
		glMaterialfv( GL_FRONT, GL_EMISSION, material->emissive );

	glMaterialf( GL_FRONT, GL_SHININESS, material->shininess );
	glBindTexture( GL_TEXTURE_2D, texture);
	glEnable( GL_TEXTURE_2D );
}

void CMS3DFile::setTexture(unsigned int textureIndex, int texture){
	if(textureIndex < m_pImpl->m_vTextures.size())
		m_pImpl->m_vTextures[textureIndex] = texture;
}


void CMS3DFile::drawGroup(ms3d_group_t* group){
	glBegin( GL_TRIANGLES );{
		int numTriangles = group->m_Header.m_numtriangles;
		for(int j=0; j<numTriangles; j++){
			int triangleIndex = (int)group->triangleIndices[j];
			ms3d_triangle_t* tri = &(m_pImpl->m_vTriangles[triangleIndex]);
			//Draw each vertex
			//for(int k=0;k<3;k++){
				//Aparently gcc would still do the for loop even with -O3 so we just expand it by hand
				glNormal3fv( tri->vertexNormals[0] );
				glTexCoord2f( tri->s[0], tri->t[0] );
				glVertex3fv( m_pImpl->m_vVertices[tri->vertexIndices[0]].vertex );
			
				glNormal3fv( tri->vertexNormals[1] );
				glTexCoord2f( tri->s[1], tri->t[1] );
				glVertex3fv( m_pImpl->m_vVertices[tri->vertexIndices[1]].vertex );
				
				glNormal3fv( tri->vertexNormals[2] );
				glTexCoord2f( tri->s[2], tri->t[2] );
				glVertex3fv( m_pImpl->m_vVertices[tri->vertexIndices[2]].vertex );
			//}
		}
	}glEnd();
}

void CMS3DFile::unloadModel(){
	GLsizei numOfGroups = m_pImpl->m_vGroups.size();
	glDeleteBuffers(numOfGroups, _eab);
	glDeleteBuffers(numOfGroups, _vbo);
	glDeleteVertexArrays(numOfGroups, _vao);
}

void CMS3DFile::prepareModel(GLuint shader){
	_shader = shader;

	//We have one vao and one vbo per group (is this the best approach?)
	_vao = new GLuint [m_pImpl->m_vGroups.size()];
	_vbo = new GLuint [m_pImpl->m_vGroups.size()];
	_eab = new GLuint [m_pImpl->m_vGroups.size()];

	glGenVertexArrays(m_pImpl->m_vGroups.size(), _vao);

	for(unsigned int i=0; i < m_pImpl->m_vGroups.size(); i++){
		prepareGroup(&(m_pImpl->m_vGroups[i]), i);
	}		

}
void CMS3DFile::prepareGroup(ms3d_group_t* group, unsigned int groupIndex){
	glBindVertexArray(_vao[groupIndex]);
		
	int numTriangles = group->m_Header.m_numtriangles;

	int totalVertices = m_pImpl->m_vVertices.size();
	int *indexes_table = new int[totalVertices]; //We allocate all temporary arrays on the heap because they can potentially be quite big!
	
	for(int i=0;i<totalVertices;i++)
		indexes_table[i] = -1;  // -1 means this vertex has not been used yet other value means its index


	GLfloat* vertices_position = new GLfloat[numTriangles*3*4];
	GLfloat* vertices_normals = new GLfloat[numTriangles*9];
	GLfloat* texture_coord = new GLfloat[numTriangles*6];

	int numVertices = numTriangles*3;
	GLuint* indices = new GLuint[numVertices];

	//Fill the arrays
	int vertex_coordinate_index = 0;
	int texture_coordinate_index = 0;
	int normal_coordinate_index = 0;
	int indices_index = 0; //Current index in the indexes table
	int index = 0; //Actual index of the vertex
	for(int j=0; j<numTriangles; j++){
		int triangleIndex = (int)group->triangleIndices[j];
		ms3d_triangle_t* tri = &(m_pImpl->m_vTriangles[triangleIndex]);
		for(int k = 0; k < 3; k++){
			int existing_tex_coord_index = indexes_table[tri->vertexIndices[k]]*2;
			int existing_normal_index = indexes_table[tri->vertexIndices[k]]*3;
			if( indexes_table[tri->vertexIndices[k]] == -1 || 					 //If we havent seen this vertex before
					(tri->s[k] != texture_coord[existing_tex_coord_index] 			 //Or if for some reason it has a different texture mapping
					 || tri->t[k] != texture_coord[existing_tex_coord_index+1]) || 		
					(tri->vertexNormals[k][0] != vertices_normals[existing_normal_index]     //Or if for some reason it has different normals...
					|| tri->vertexNormals[k][1] != vertices_normals[existing_normal_index+1] //create a new vertex (I know, comparing floats..., but what else can I do?)
					|| tri->vertexNormals[k][2] != vertices_normals[existing_normal_index+2]) ){

				vertices_position[vertex_coordinate_index++] = m_pImpl->m_vVertices[tri->vertexIndices[k]].vertex[0]; 
				vertices_position[vertex_coordinate_index++] = m_pImpl->m_vVertices[tri->vertexIndices[k]].vertex[1]; 
				vertices_position[vertex_coordinate_index++] = m_pImpl->m_vVertices[tri->vertexIndices[k]].vertex[2];
				vertices_position[vertex_coordinate_index++] = 1.0;

				vertices_normals[normal_coordinate_index++] = tri->vertexNormals[k][0];
				vertices_normals[normal_coordinate_index++] = tri->vertexNormals[k][1];
				vertices_normals[normal_coordinate_index++] = tri->vertexNormals[k][2];

				texture_coord[texture_coordinate_index++] = tri->s[k];
				texture_coord[texture_coordinate_index++] = tri->t[k];

				indexes_table[tri->vertexIndices[k]] = index; // change from -1 to the index of this vertex
				indices[indices_index++] = index;
				index++;
			}else{
				indices[indices_index++] = indexes_table[tri->vertexIndices[k]];
			}
		}
	}

	glGenBuffers(1, &_vbo[groupIndex]);

	size_t positionSize = sizeof(GLfloat)*vertex_coordinate_index;
	size_t normalsSize = sizeof(GLfloat)*normal_coordinate_index;
	size_t textureCoordSize = sizeof(GLfloat)*texture_coordinate_index;

	size_t totalSize = positionSize + normalsSize + textureCoordSize;

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[groupIndex]);
	glBufferData(GL_ARRAY_BUFFER, totalSize, NULL, GL_STATIC_DRAW);

	/*Copy the data to the buffer*/
	glBufferSubData(GL_ARRAY_BUFFER, 0, positionSize, vertices_position);
	glBufferSubData(GL_ARRAY_BUFFER, positionSize, textureCoordSize, texture_coord);
	glBufferSubData(GL_ARRAY_BUFFER, positionSize + textureCoordSize, normalsSize, vertices_normals);

	//Set up the indices
	glGenBuffers(1, &_eab[groupIndex]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _eab[groupIndex]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*numVertices , indices, GL_STATIC_DRAW);

	//Position Attribute
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, 0);
	glEnableVertexAttribArray(0);

	//Texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)positionSize);
	glEnableVertexAttribArray(1);

	//Normals attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(positionSize+textureCoordSize) );
	glEnableVertexAttribArray(2);


	//Free all the temporary memory
	delete[] indexes_table;
	delete[] vertices_position;
	delete[] vertices_normals;
	delete[] texture_coord;
	delete[] indices;
}


void CMS3DFile::optimize(){
	mergeGroups();
	removeUnusedMaterials();
}

void CMS3DFile::createRectangle(float width, float height, GLuint texture){

	float vertBuffer[6*3] = { 0,height,0, 0,0,0, width,0,0,  width,0,0, width,height,0, 0,height,0 };
	float normal[3] = { 0,0,1 };
	float texCoordBuffer[6*2] = { 0,0, 0,1, 1,1, 1,1, 1,0, 0,0 };

	int vertIndex = 0;

	word nNumVertices = 6;
	m_pImpl->m_vVertices.resize(nNumVertices);
	ms3d_vertex_t* vert;
	for(int i=0; i<nNumVertices; i++){
		vert = &m_pImpl->m_vVertices[i];
		vert->flags = 0;
		vert->boneId = -1;
		vert->vertex[0] = vertBuffer[vertIndex++];
		vert->vertex[1] = vertBuffer[vertIndex++];
		vert->vertex[2] = vertBuffer[vertIndex++];
	}
	
	vertIndex = 0;
	int texIndex = 0;
	
	word nNumTriangles = 2;
	m_pImpl->m_vTriangles.resize(nNumTriangles);
	ms3d_triangle_t* tri;
	for(int i=0; i<nNumTriangles; i++){
		tri = &m_pImpl->m_vTriangles[i];
		tri->flags = 0;
		tri->vertexIndices[0] = vertIndex++;
		tri->vertexIndices[1] = vertIndex++;
		tri->vertexIndices[2] = vertIndex++;

		for(int j=0; j<3; j++){
			tri->vertexNormals[j][0] = normal[0];
			tri->vertexNormals[j][1] = normal[1];
			tri->vertexNormals[j][2] = normal[2];
		}
		for(int j=0; j<3; j++){
			tri->s[j] = texCoordBuffer[texIndex++];
			tri->t[j] = texCoordBuffer[texIndex++];
		}
		tri->smoothingGroup = 1;
		tri->groupIndex = 0;
	}

	word nNumGroups = 1;
	m_pImpl->m_vGroups.resize(nNumGroups);
	ms3d_group_t* group = &m_pImpl->m_vGroups[0];
	group->m_Header.m_flags = 0;
	group->m_Header.m_name[0] = '1';
	group->m_Header.m_name[1] = '\0';
	group->m_Header.m_numtriangles = 2;
	group->triangleIndices.resize(2);// = new word[2];
	group->triangleIndices[0] = 0;
	group->triangleIndices[1] = 1;
	group->materialIndex = 0;

	word nNumMaterials = 1;
	m_pImpl->m_vMaterials.resize(nNumMaterials);
	ms3d_material_t* mat = &m_pImpl->m_vMaterials[0];
	mat->name[0] = '1';
	mat->name[1] = '\0';
	for(int i=0; i<3; i++){
		mat->ambient[i] = 25;
		mat->diffuse[i] = 200;
		mat->specular[i] = 0;
		mat->emissive[i] = 0;
	}
	mat->ambient[3] = 0;
	mat->diffuse[3] = 0;
	mat->specular[3] = 0;
	mat->emissive[3] = 0;
	mat->shininess = 0;
	mat->transparency = 1;
	mat->mode = 0;
	mat->texture[0] = '\0';
	mat->alphamap[0] = '\0';
	
	m_pImpl->m_vTextures.resize(1);
	m_pImpl->m_vTextures[0] = texture;

	m_pImpl->m_vJoints.resize(0);
}

void CMS3DFile::setMaterialEmissive(char* matName, float red, float green, float blue){
	int numMaterials = m_pImpl->m_vMaterials.size();
	ms3d_material_t* mat;
	for(int i =0; i<numMaterials; i++){
		mat = &m_pImpl->m_vMaterials[i];
		if(!strcmp(matName,mat->name)){
			mat->emissive[0] = red;
			mat->emissive[1] = green;
			mat->emissive[2] = blue;
		}
	}
}

void CMS3DFile::setMaterialTransparency(char* matName, float alpha){
	int numMaterials = m_pImpl->m_vMaterials.size();
	ms3d_material_t* mat;
	for(int i =0; i<numMaterials; i++){
		mat = &m_pImpl->m_vMaterials[i];
		if(!strcmp(matName,mat->name))
			mat->transparency = alpha;
	}
}


void CMS3DFile::translateModel(float x, float y, float z){
	for(unsigned int i = 0; i<m_pImpl->m_vVertices.size(); i++){
		m_pImpl->m_vVertices[i].vertex[0] += x;
		m_pImpl->m_vVertices[i].vertex[1] += y;
		m_pImpl->m_vVertices[i].vertex[2] += z;
	}
}
