

//#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <list>

#include "MS3DFile.h"
#include "MS3DFileI.h"

void CMS3DFile::mergeGroups(){
	std::vector<ms3d_group_t>* groups = &m_pImpl->m_vGroups;

	ms3d_group_t* curGroup; 
	ms3d_group_t* otherGroup;
	char curMaterial = -1;
	char otherMaterial = -1;
	for(unsigned int i = 0; i < groups->size(); i++){
		curGroup = &(*groups)[i];
		curMaterial = curGroup->materialIndex;
		for(unsigned int j=i+1; j<groups->size(); j++){
			otherGroup = &(*groups)[j];
			otherMaterial = otherGroup->materialIndex;
			if(curMaterial == otherMaterial){
				/*Merge*/
				int oldNumTriangles = curGroup->m_Header.m_numtriangles;
				//word* oldIndices = curGroup->triangleIndices;
				
				curGroup->m_Header.m_numtriangles += otherGroup->m_Header.m_numtriangles;
				curGroup->triangleIndices.resize(curGroup->m_Header.m_numtriangles); //= new word[curGroup->m_Header.m_numtriangles];
				curGroup->triangleIndices.insert(curGroup->triangleIndices.end(), otherGroup->triangleIndices.begin(), otherGroup->triangleIndices.end());
				//std::copy(oldIndices, oldIndices+oldNumTriangles, curGroup->triangleIndices);
				//std::copy(otherGroup->triangleIndices, (otherGroup->triangleIndices)+(otherGroup->m_Header.m_numtriangles), curGroup->triangleIndices+oldNumTriangles);

				//delete otherGroup->triangleIndices;
				groups->erase(groups->begin()+j);

				/*After merging there is one less group (this one) so we decrement j*/
				j--;
			}
		}
	}
}

void CMS3DFile::removeUnusedMaterials(){

	/*Not actually removing it, just erasing the texture associated with it so we dont load the image next time*/

	int numOfMaterials = m_pImpl->m_vMaterials.size();
	std::vector< bool > used(numOfMaterials, false );
	//for(int i=0; i<numOfMaterials; i++)
	//	used[i] = false;

	for(unsigned int i=0; i<m_pImpl->m_vGroups.size(); i++)
		used[(int)m_pImpl->m_vGroups[i].materialIndex] = true;

	for(int i=0; i<numOfMaterials; i++)
		if(!used[i]){
			m_pImpl->m_vMaterials[i].texture[0] = '\0';
			std::cout << "Cleared out material: " << m_pImpl->m_vMaterials[i].name << "\n";
		}
}
