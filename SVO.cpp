#include "SVO.h"

#include <array>
#include <iostream>
#include <fstream>


SVO::SVO(const uint32_t levels) {
	this->levels = levels;
	if (this->levels < 1)
		this->levels = 1;

	dimension = glm::pow(2, this->levels);

	octantVector.emplace_back(leafBit);

	leafVector.emplace_back();
	//leafMap.emplace(0, leafBit);
}

SVO::SVO(const std::string filename) {
	std::ifstream fin("./assets/" + filename, std::ios::in | std::ios::binary);

	if (fin.fail()) {
		levels = 1;
		dimension = glm::pow(2, levels);

		octantVector.emplace_back(leafBit);

		leafVector.emplace_back();

		return;
	}

	// number of inner octant levels
	fin.read(reinterpret_cast<char*>(&levels), sizeof(levels));

	dimension = glm::pow(2, levels);


	// number of inner octants
	unsigned int levelOctants;
	fin.read(reinterpret_cast<char*>(&levelOctants), sizeof(levelOctants));

	this->reserveInnerOctants(levelOctants);

	// fill octants
	for (unsigned int octant = 0; octant < levelOctants; octant++) {
		InnerOctant oct(0);
		fin.read(reinterpret_cast<char*>(&oct), sizeof(oct));

		octantVector.push_back(oct);
	}

	// number of leaves
	unsigned int leavesCount;
	fin.read(reinterpret_cast<char*>(&leavesCount), sizeof(leavesCount));

	this->reserveLeaves(leavesCount);

	// leaf data
	for (unsigned int leaf = 0; leaf < leavesCount; leaf++) {
		Leaf l(0, 0);
		fin.read(reinterpret_cast<char*>(&l), sizeof(l));

		//leafMap.emplace(l.color, leafBit | leafVector.size());
		leafVector.push_back(l);
	}

	fin.close();
}


const SVO::InnerOctant* const SVO::getInnerOctantData() const {
	return octantVector.data();
}

const uint32_t SVO::getInnerOctantsSize() const {
	uint32_t size = octantVector.size();

	//for (unsigned int level = 0; level < octantGraph.size(); level++) {
		//size += octantVector.size();

		if (size >= leafBit) {
			std::cout << "SVO is too large for array" << std::endl;
			exit(1);
		}
	//}

	return size * sizeof(InnerOctant);
}

void SVO::setInnerOctantsSize(const uint32_t size) {
	try {
		octantVector.resize(size, InnerOctant(leafBit));
		octantVector.shrink_to_fit();
	}
	catch (const std::length_error& le) {
		std::cerr << "inner octant resize, alloc length error: " << le.what() << '\n';
		exit(1);
	}
	catch (const std::bad_alloc& le) {
		std::cerr << "inner octant resize, bad alloc error: " << le.what() << '\n';
		exit(1);
	}
	catch (...) {
		std::cerr << "inner octant resize, unknown error\n";
		exit(1);
	}
}

void SVO::reserveInnerOctants(const uint32_t size) {
	try {
		octantVector.reserve(size);
	}
	catch (const std::length_error& le) {
		std::cerr << "inner octant reserve, alloc length error: " << le.what() << '\n';
		exit(1);
	}
	catch (const std::bad_alloc& le) {
		std::cerr << "inner octant reserve, bad alloc error: " << le.what() << '\n';
		exit(1);
	}
	catch (...) {
		std::cerr << "inner octant reserve, unknown error\n";
		exit(1);
	}
}


const SVO::Leaf* const SVO::getLeafData() const {
	return leafVector.data();
}

const uint32_t SVO::getLeavesSize() const {
	return leafVector.size() * sizeof(Leaf);
}

void SVO::setLeavesSize(const uint32_t size) {
	try {
		leafVector.resize(size);
		leafVector.shrink_to_fit();
	}
	catch (const std::length_error& le) {
		std::cerr << "leaf resize, alloc length error: " << le.what() << '\n';
		exit(1);
	}
	catch (const std::bad_alloc& le) {
		std::cerr << "leaf resize, bad alloc error: " << le.what() << '\n';
		exit(1);
	}
	catch (...) {
		std::cerr << "leaf resize, unknown error\n";
		exit(1);
	}
}

void SVO::reserveLeaves(const uint32_t size) {
	try {
		leafVector.reserve(size);
	}
	catch (const std::length_error& le) {
		std::cerr << "leaf reserve, alloc length error: " << le.what() << '\n';
		exit(1);
	}
	catch (const std::bad_alloc& le) {
		std::cerr << "leaf reserve, bad alloc error: " << le.what() << '\n';
		exit(1);
	}
	catch (...) {
		std::cerr << "leaf reserve, unknown error\n";
		exit(1);
	}
}


void SVO::addElement(const glm::ivec3 coord, const uint32_t color, const uint32_t normal) {
	// if new point is outside of predefined bounds just dismiss
	if ( glm::any(glm::lessThan(coord, glm::ivec3(0)))  ||  glm::any(glm::greaterThanEqual(coord, glm::ivec3(dimension))) )
		return;

	glm::uvec3 pos = coord;

	glm::uint extent = dimension;

	uint32_t index = 0;
	glm::uvec3 offset = glm::uvec3(0);

	for (unsigned int level = 1; level < levels; level++) {
		extent /= 2;
		glm::bvec3 mask = glm::greaterThanEqual(pos, offset + extent);

		int child = int(mask.x) + int(mask.y) * 2 + int(mask.z) * 4;

		offset += glm::vec3(mask) * glm::vec3(extent);

		// if child node is a leaf
		if (octantVector[index].children[child] >= leafBit) {
			// create new inner octant with blank leaves
			octantVector[index].children[child] = octantVector.size();
			octantVector.emplace_back(leafBit);
		}

		index = octantVector[index].children[child];
	}

	extent /= 2;
	glm::bvec3 mask = glm::greaterThanEqual(pos, offset + extent);

	int child = int(mask.x) + int(mask.y) * 2 + int(mask.z) * 4;

	// don't overwrite leaves
	if (octantVector[index].children[child] == leafBit) {
		//auto leafExist = leafMap.try_emplace((uint64_t(normal) << 32) | color, leafBit | leafVector.size());

		//if (leafExist.second) {
			// create new leaf
		octantVector[index].children[child] = leafBit | leafVector.size();
		leafVector.emplace_back(color, normal);
		//}
		/*else {
				reuse existing leaf
			octantVector[index].children[child] = (*leafExist.first).second;
		}*/
	}
}

void SVO::printTree(bool verbose) {
	std::cout << "inner octants size " << getInnerOctantsSize() << " bytes" << std::endl;
	std::cout << "leaves size " << getLeavesSize() << " bytes" << std::endl;
	std::cout << "total size " << getInnerOctantsSize() + getLeavesSize() << " bytes\n\n" << std::endl;

	std::cout << "inner octants" << std::endl;
	//for (unsigned int level = 0; level < levels; level++)
		std::cout << " octants: " << octantVector.size() << std::endl;

	if (verbose) {
		for (unsigned int level = 0; level < levels; level++) {
			std::cout << "level " << level << std::endl;
			//for (int i = 0; i < octantGraph[level].size(); i++) {
				//std::cout << i << std::endl;
				//for (int j = 0; j < 8; j++)
				//	std::cout << ((octantGraph[level][i].children[j] & leafBit) > 0) << "  " << (octantGraph[level][i].children[j] & leafIdx) << std::endl;

				//std::cout << std::endl;
			//}
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

	// number of inner octants
	unsigned int levelOctants = octantVector.size();
	fout.write(reinterpret_cast<const char*>(&levelOctants), sizeof(levelOctants));
	// octants
	fout.write(reinterpret_cast<const char*>(&octantVector[0]), levelOctants * sizeof(InnerOctant));


	// number of leaves
	unsigned int leavesCount = leafVector.size();
	fout.write(reinterpret_cast<const char*>(&leavesCount), sizeof(leavesCount));

	// leaf data
	fout.write(reinterpret_cast<const char*>(&leafVector[0]), leavesCount * sizeof(Leaf));


	fout.close();
}
