#include "DAG.h"

#include <iostream>
#include <string>


DAG::DAG(const uint32_t levels) {
	this->levels = levels;
	if (this->levels < 1)
		this->levels = 1;

	dimension = glm::pow(2, this->levels);

	//std::cout << "levels: " << level << std::endl;
	//std::cout << "dimensions: " << dimension << std::endl;


	// create octant levels
	for (unsigned int level = 0; level < this->levels; level++) {
		innerOctants.emplace_back();
		references.emplace_back();
	}
	// root node doesn't have a reference count. leaves use root level instead
	innerOctants[0].emplace_back();

	for (unsigned int child = 0; child < 8; child++)
		innerOctants[0][0].children[child] = leafBit;

	// blank leaf
	leaves.emplace_back();
	references[0].emplace_back(1);
}


const DAG::InnerOctant* DAG::getInnerOctantData() {
	newOctants.clear();

	uint32_t childrenIdx = 0;

	for (unsigned int level = 0; level < innerOctants.size(); level++) {
		// inner octant children offset by current level's size
		childrenIdx += innerOctants[level].size();

		for (unsigned int octant = 0; octant < innerOctants[level].size(); octant++) {
			InnerOctant *newOctant = &newOctants.emplace_back(innerOctants[level][octant]);

			for (unsigned int child = 0; child < 8; child++)
				if (newOctant->children[child] < leafBit)
					newOctant->children[child] += childrenIdx;
		}
	}

	return newOctants.data();
}

uint32_t DAG::getInnerOctantsSize() const {
	uint32_t size = 0;

	for (unsigned int level = 0; level < innerOctants.size(); level++)
		size += innerOctants[level].size();

	return size * sizeof(InnerOctant);
}

const DAG::Leaf* DAG::getLeafData() const {
	return leaves.data();
}

uint32_t DAG::getLeavesSize() const {
	return leaves.size() * sizeof(Leaf);
}


void DAG::addElement(int x, int y, int z, uint32_t color) {
	std::cerr << "DAG::addElement is not functional yet" << std::endl;
	return;

	// if new point is outside of predefined bounds just dismiss
	if (x >= dimension || y >= dimension || z >= dimension)
		return;

	// find or create leaf
	bool newLeaf = true;
	unsigned int newLeafIdx = 0;

	for (; newLeafIdx < leaves.size(); newLeafIdx++) {
		if (leaves[newLeafIdx].color == color) {
			references[0][newLeafIdx]++;
			newLeaf = false;
			break;
		}
	}

	if (newLeaf) {
		leaves.emplace_back();
		leaves[newLeafIdx].color = color;

		references[0].emplace_back(1);
	}

	newLeafIdx |= leafBit;


	// duplicate tree branch
	std::vector<InnerOctant> dupInnerOctants;
	std::vector<uint32_t> childrenIdxs;

	//std::vector<int> replaceableOctants;
	//replaceableOctants.emplace_back(-1);

	glm::uvec3 pos = glm::uvec3(x, y, z);

	glm::uint extent = dimension;

	unsigned int index = 0;
	glm::uvec3 offset = glm::uvec3(0);

	for (unsigned int level = 0; level < levels; level++) {
		// copy current octant to end of duplicate octants
		InnerOctant currentOctant = dupInnerOctants.emplace_back(innerOctants[level][index]);

		extent /= 2;
		glm::bvec3 mask = glm::greaterThanEqual(pos, offset + extent);

		unsigned int child = childrenIdxs.emplace_back(int(mask.x) + int(mask.y) * 2 + int(mask.z) * 4);
		offset += glm::vec3(mask) * glm::vec3(extent);

		/*
		if (level > 0)
		{
			--references[level][index];
			if (references[level][index] == 0)
				replaceableOctants.emplace_back(index);
			else
				replaceableOctants.emplace_back(-1);
		}*/

		// if reached deepest inner octant, break out
		if (level == levels - 1) {
			references[0][currentOctant.children[child] & leafIdx]--;
			break;
		}

		// if shallow octant child is a leaf
		if (currentOctant.children[child] >= leafBit) {
			// create new branches until reaching leaf level
			for (; level < levels - 1; level++) {
				extent /= 2;
				glm::bvec3 mask = glm::greaterThanEqual(pos, offset + extent);

				child = childrenIdxs.emplace_back(int(mask.x) + int(mask.y) * 2 + int(mask.z) * 4);
				offset += glm::vec3(mask) * glm::vec3(extent);

				InnerOctant *newOctant = &dupInnerOctants.emplace_back();

				for (unsigned int nchild = 0; nchild < 8; nchild++)
					newOctant->children[nchild] = leafBit;

				//replaceableOctants.emplace_back(-1);
			}
			break;
		}

		index = currentOctant.children[child];
	}

	// set leaf to appropriate child
	dupInnerOctants.back().children[childrenIdxs.back()] = newLeafIdx;


	// merge duplicate branch into original tree
	for (unsigned int level = levels - 1; level > 0; level--) {
		if (innerOctants[level].size() == 0) {
			innerOctants[level].emplace_back(dupInnerOctants[level]);
			references[level].emplace_back(1);
			continue;
		}

		bool newOctant = true;
		for (index = 0; index < innerOctants[level].size(); index++) {
			bool duplicate = true;
			for (unsigned int child = 0; child < 8; child++) {
				if (dupInnerOctants[level].children[child] != innerOctants[level][index].children[child]) {
					duplicate = false;
					break;
				}
			}

			if (duplicate) {
				dupInnerOctants[level - 1].children[childrenIdxs[level - 1]] = index;
				references[level][index]++;

				newOctant = false;
				break;
			}
		}

		if (newOctant) {
			/*if (replaceableOctants[level] >= 0)
			{
				dupInnerOctants[level - 1].children[childrenIdxs[level - 1]] = replaceableOctants[level];
				innerOctants[level][replaceableOctants[level]] = dupInnerOctants[level];
				references[level][replaceableOctants[level]] = 1;

				replaceableOctants[level] = -1;
			}
			else*/
			{
				dupInnerOctants[level - 1].children[childrenIdxs[level - 1]] = innerOctants[level].size();
				innerOctants[level].emplace_back(dupInnerOctants[level]);
				references[level].emplace_back(1);
			}
		}
	}

	// set root octant
	innerOctants[0][0] = dupInnerOctants[0];

	/*
	for (unsigned int level = levels - 1; level > 0; level--)
	{
		if (replaceableOctants[level] >= 0)
		{
			innerOctants[level].erase(innerOctants[level].begin() + replaceableOctants[level]);
			references[level].erase(references[level].begin() + replaceableOctants[level]);

			for (unsigned int octant = 0; octant < innerOctants[level - 1].size(); octant++)
				for (unsigned int child = 0; child < 8; child++)
					if (innerOctants[level - 1][octant].children[child] > replaceableOctants[level])
						innerOctants[level - 1][octant].children[child]--;
		}
	}
	*/
}


void DAG::printTree(bool verbose)
{
	std::cout << "inner octants size " << getInnerOctantsSize() << " bytes" << std::endl;
	std::cout << "leaves size " << getLeavesSize() << " bytes" << std::endl;
	std::cout << "total size " << getInnerOctantsSize() + getLeavesSize() << " bytes\n\n" << std::endl;

	std::cout << "inner octants" << std::endl;
	for (unsigned int level = 0; level < levels; level++)
		std::cout << "level: " << level << " octants: " << innerOctants[level].size() << std::endl;
	if(verbose)
		for (unsigned int level = 0; level < levels; level++) {
			std::cout << "level " << level << std::endl;
			for (int i = 0; i < innerOctants[level].size(); i++) {
				std::cout << i << std::endl;
				for (int j = 0; j < 8; j++)
					std::cout << ((innerOctants[level][i].children[j] & leafBit) > 0) << "  " << (innerOctants[level][i].children[j] & leafIdx) << std::endl;
				std::cout << std::endl;
			}
		}

	std::cout << leaves.size() << " leaves" << std::endl;
	if (verbose)
		for (int i = 0; i < leaves.size(); i++)
			std::cout << leaves[i].color << std::endl;

	std::cout << std::endl;
}

void DAG::printReferences() {
	unsigned int greaterThanZero = 0;

	std::cout << "inner octants" << std::endl;
	for (unsigned int level = 1; level < levels; level++) {
		std::cout << "level " << level << std::endl;

		greaterThanZero = 0;
		for (unsigned int index = 0; index < references[level].size(); index++)
			if (references[level][index] > 0)
				greaterThanZero++;

		std::cout << "0 " << references[level].size() - greaterThanZero << std::endl;
		std::cout << "1 " << greaterThanZero << std::endl;

	}

	greaterThanZero = 0;
	for (unsigned int index = 0; index < references[0].size(); index++)
		if (references[0][index] > 0)
			greaterThanZero++;

	std::cout << "leaves" << std::endl;
	std::cout << "0 " << references[0].size() - greaterThanZero << std::endl;
	std::cout << "1 " << greaterThanZero << std::endl;
}
