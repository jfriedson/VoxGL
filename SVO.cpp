#include "SVO.h"

#include <iostream>
#include <fstream>


SVO::SVO(const uint32_t levels) {
	this->levels = levels;
	if (this->levels < 1)
		this->levels = 1;

	dimension = glm::pow(2, this->levels);

	//std::cout << "levels: " << level << std::endl;
	//std::cout << "dimensions: " << dimension << std::endl;

	for (uint32_t level = 0; level < this->levels; level++)
		octantGraph.emplace_back();

	octantGraph[0].emplace_back();
	for (unsigned int child = 0; child < 8; child++)
		octantGraph[0][0].children[child] = leafBit;

	// first leaf is blank
	leafVector.emplace_back();
	leafMap.emplace(0, leafBit);
}

SVO::SVO(const std::string filename) {
	std::ifstream fin("./assets/" + filename, std::ios::in | std::ios::binary);

	if (fin.fail()) {
		levels = 1;
		dimension = glm::pow(2, levels);

		//std::cout << "levels: " << level << std::endl;
		//std::cout << "dimensions: " << dimension << std::endl;

		for (uint32_t level = 0; level < levels; level++)
			octantGraph.emplace_back();

		octantGraph[0].emplace_back();
		for (unsigned int child = 0; child < 8; child++)
			octantGraph[0][0].children[child] = leafBit;

		// first leaf is blank
		leafVector.emplace_back();
		leafVector[0].color = glm::packUnorm4x8(glm::vec4(8.f, 0.2f, 0.3f, 1.f));
		leafMap.emplace(leafVector.back().color, leafBit);

		return;
	}
	// number of inner octant levels
	fin.read(reinterpret_cast<char*>(&levels), sizeof(levels));

	dimension = glm::pow(2, levels);


	for (uint32_t level = 0; level < levels; level++) {
		octantGraph.emplace_back();

		// number of inner octants on level
		unsigned int levelOctants;
		fin.read(reinterpret_cast<char*>(&levelOctants), sizeof(levelOctants));

		// octants on level
		for (unsigned int octant = 0; octant < levelOctants; octant++) {
			InnerOctant oct;
			fin.read(reinterpret_cast<char*>(&oct), sizeof(oct));
			octantGraph.back().push_back(oct);
		}
	}

	// number of leaves
	unsigned int leavesCount;
	fin.read(reinterpret_cast<char*>(&leavesCount), sizeof(leavesCount));

	// leaf data
	for (unsigned int leaf = 0; leaf < leavesCount; leaf++) {
		Leaf l;
		fin.read(reinterpret_cast<char*>(&l), sizeof(l));

		leafMap.emplace(l.color, leafBit | leafVector.size());
		leafVector.push_back(l);
	}

	fin.close();
}



const SVO::InnerOctant* SVO::getInnerOctantData() {
	octantArray.clear();

	uint32_t childrenIdx = 0;

	for (unsigned int level = 0; level < octantGraph.size(); level++) {
		// inner octant children offset by current level's size
		childrenIdx += octantGraph[level].size();

		for (unsigned int octant = 0; octant < octantGraph[level].size(); octant++) {
			octantArray.emplace_back(octantGraph[level][octant]);

			for (unsigned int child = 0; child < 8; child++)
				if (octantArray.back().children[child] < leafBit)
					octantArray.back().children[child] += childrenIdx;
		}
	}

	return octantArray.data();
}

uint32_t SVO::getInnerOctantsSize() const {
	uint32_t size = 0;

	for (unsigned int level = 0; level < octantGraph.size(); level++) {
		size += octantGraph[level].size();

		// print error and terminate if flattened svo is too large for gpu
		if (size >= leafBit || size < octantGraph[level].size()) {
			std::cout << "SVO is too large to be flattened." << std::endl;
			exit(1);
		}
	}

	return size * sizeof(InnerOctant);
}

const SVO::Leaf* SVO::getLeafData() const {
	return leafVector.data();
}

uint32_t SVO::getLeavesSize() const {
	return leafVector.size() * sizeof(Leaf);
}

void SVO::addElement(int x, int y, int z, uint32_t color) {
	// if new point is outside of predefined bounds just dismiss
	if (x >= dimension || y >= dimension || z >= dimension)
		return;

	glm::uvec3 pos = glm::uvec3(x, y, z);

	glm::uint extent = dimension;

	uint32_t index = 0;
	glm::uvec3 offset = glm::uvec3(0);

	for (unsigned int level = 0; level < levels; level++) {
		extent /= 2;
		glm::bvec3 mask = glm::greaterThanEqual(pos, offset + extent);

		int child = int(mask.x) + int(mask.y) * 2 + int(mask.z) * 4;

		offset += glm::vec3(mask) * glm::vec3(extent);

		// upon reaching lowest level, set leaf and return
		if (level == levels - 1) {
			auto leafExist = leafMap.try_emplace(color, leafBit | leafVector.size());

			if (leafExist.second) {
				// create new leaf
				octantGraph[level][index].children[child] = (*leafExist.first).second;
				leafVector.emplace_back();
				leafVector.back().color = color;
			}
			else {
				// reuse existing leaf
				octantGraph[level][index].children[child] = (*leafExist.first).second;
			}

			return;
		}

		// if child node is a leaf
		if (octantGraph[level][index].children[child] >= leafBit) {
			// create new inner octant
			octantGraph[level][index].children[child] = octantGraph[level + 1].size();
			octantGraph[level + 1].emplace_back();

			// assign the new inner octant blank leaves
			for (int j = 0; j < 8; j++)
				octantGraph[level + 1].back().children[j] = leafBit;
		}

		index = octantGraph[level][index].children[child];
	}
}

void SVO::printTree(bool verbose) {
	std::cout << "inner octants size " << getInnerOctantsSize() << " bytes" << std::endl;
	std::cout << "leaves size " << getLeavesSize() << " bytes" << std::endl;
	std::cout << "total size " << getInnerOctantsSize() + getLeavesSize() << " bytes\n\n" << std::endl;

	std::cout << "inner octants" << std::endl;
	for (unsigned int level = 0; level < levels; level++)
		std::cout << "level: " << level << " octants: " << octantGraph[level].size() << std::endl;

	if (verbose) {
		for (unsigned int level = 0; level < levels; level++) {
			std::cout << "level " << level << std::endl;
			for (int i = 0; i < octantGraph[level].size(); i++) {
				std::cout << i << std::endl;
				for (int j = 0; j < 8; j++)
					std::cout << ((octantGraph[level][i].children[j] & leafBit) > 0) << "  " << (octantGraph[level][i].children[j] & leafIdx) << std::endl;

				std::cout << std::endl;
			}
		}
	}

	std::cout << leafVector.size() << " leaves" << std::endl;
	if (verbose) {
		for (int i = 0; i < leafVector.size(); i++)
			std::cout << leafVector[i].color << std::endl;
	}

	std::cout << std::endl;
}

void SVO::save(const std::string filename) {
	std::ofstream fout("./assets/" + filename, std::ios::out | std::ios::binary);

	// number of inner octant levels
	fout.write(reinterpret_cast<const char*>(&levels), sizeof(levels));

	for (uint32_t level = 0; level < levels; level++) {
		// number of inner octants on level
		unsigned int levelOctants = octantGraph[level].size();
		fout.write(reinterpret_cast<const char*>(&levelOctants), sizeof(levelOctants));
		// octants on level
		fout.write(reinterpret_cast<const char*>(&octantGraph[level][0]), levelOctants * sizeof(InnerOctant));
	}

	// number of leaves
	unsigned int leavesCount = leafVector.size();
	fout.write(reinterpret_cast<const char*>(&leavesCount), sizeof(leavesCount));

	// leaf data
	fout.write(reinterpret_cast<const char*>(&leafVector[0]), leavesCount * sizeof(Leaf));

	fout.close();
}
