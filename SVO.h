#pragma once

#include "robin_hood.h"

#include <array>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <glm/glm.hpp>


// A sparse octree with merged leaves
template <typename InnerOctantType, typename LeafType, typename MaskType>
class SVO
{
private:
	const MaskType dataMasks;

	std::vector<InnerOctantType> octantVector;

	std::vector<LeafType> leafVector;

	//robin_hood::unordered_flat_map<uint64_t, uint32_t> leafMap;  // leaf color and vector index

	uint32_t dimension;
	uint32_t levels;

public:
	SVO(const uint32_t levels);
	SVO(const std::string& filename);

	const InnerOctantType* const getInnerOctantData() const;
	const uint32_t getInnerOctantsSize() const;
	void setInnerOctantsSize(const uint32_t);
	void reserveInnerOctants(const uint32_t);

	const LeafType* const getLeafData() const;
	const uint32_t getLeavesSize() const;
	void setLeavesSize(const uint32_t);
	void reserveLeaves(const uint32_t);

	void addElement(const glm::ivec3 coord, const uint32_t color, const uint32_t normal);

	void printTree(bool verbose);

	void save(const std::string filename);
};






template <typename InnerOctantType, typename LeafType, typename MaskType>
SVO<typename InnerOctantType, typename LeafType, typename MaskType>::SVO(const uint32_t levels) {
	this->levels = levels;
	if (this->levels < 1)
		this->levels = 1;

	dimension = glm::pow(2, this->levels);

	octantVector.emplace_back(dataMasks.leafBit);

	leafVector.emplace_back();
}

template <typename InnerOctantType, typename LeafType, typename MaskType>
SVO<typename InnerOctantType, typename LeafType, typename MaskType>::SVO(const std::string& filename) {
	std::ifstream fin("./assets/" + filename, std::ios::in | std::ios::binary);

	if (fin.fail()) {
		levels = 1;
		dimension = glm::pow(2, levels);

		octantVector.emplace_back(dataMasks.leafBit);

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
		InnerOctantType oct(0);
		fin.read(reinterpret_cast<char*>(&oct), sizeof(oct));

		octantVector.push_back(oct);
	}

	// number of leaves
	unsigned int leavesCount;
	fin.read(reinterpret_cast<char*>(&leavesCount), sizeof(leavesCount));

	this->reserveLeaves(leavesCount);

	// leaf data
	for (unsigned int leaf = 0; leaf < leavesCount; leaf++) {
		LeafType l(0, 0);
		fin.read(reinterpret_cast<char*>(&l), sizeof(l));

		//leafMap.emplace(l.color, leafBit | leafVector.size());
		leafVector.push_back(l);
	}

	fin.close();
}

template <typename InnerOctantType, typename LeafType, typename MaskType>
const InnerOctantType* const SVO<typename InnerOctantType, typename LeafType, typename MaskType>::getInnerOctantData() const {
	return octantVector.data();
}

template <typename InnerOctantType, typename LeafType, typename MaskType>
const uint32_t SVO<typename InnerOctantType, typename LeafType, typename MaskType>::getInnerOctantsSize() const {
	uint32_t size = octantVector.size();

	//for (unsigned int level = 0; level < octantGraph.size(); level++) {
		//size += octantVector.size();

	if (size >= dataMasks.leafBit) {
		std::cout << "SVO is too large for array" << std::endl;
		exit(1);
	}
	//}

	return size * sizeof(InnerOctantType);
}

template <typename InnerOctantType, typename LeafType, typename MaskType>
void SVO<typename InnerOctantType, typename LeafType, typename MaskType>::setInnerOctantsSize(const uint32_t size) {
	try {
		octantVector.resize(size, InnerOctantType(dataMasks.leafBit));
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

template <typename InnerOctantType, typename LeafType, typename MaskType>
void SVO<typename InnerOctantType, typename LeafType, typename MaskType>::reserveInnerOctants(const uint32_t size) {
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


template <typename InnerOctantType, typename LeafType, typename MaskType>
const LeafType* const SVO<typename InnerOctantType, typename LeafType, typename MaskType>::getLeafData() const {
	return leafVector.data();
}

template <typename InnerOctantType, typename LeafType, typename MaskType>
const uint32_t SVO<typename InnerOctantType, typename LeafType, typename MaskType>::getLeavesSize() const {
	return leafVector.size() * sizeof(LeafType);
}

template <typename InnerOctantType, typename LeafType, typename MaskType>
void SVO<typename InnerOctantType, typename LeafType, typename MaskType>::setLeavesSize(const uint32_t size) {
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

template <typename InnerOctantType, typename LeafType, typename MaskType>
void SVO<typename InnerOctantType, typename LeafType, typename MaskType>::reserveLeaves(const uint32_t size) {
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


template <typename InnerOctantType, typename LeafType, typename MaskType>
void SVO<typename InnerOctantType, typename LeafType, typename MaskType>::addElement(const glm::ivec3 coord, const uint32_t color, const uint32_t normal) {
	// if new point is outside of predefined bounds just dismiss
	if (glm::any(glm::lessThan(coord, glm::ivec3(0))) || glm::any(glm::greaterThanEqual(coord, glm::ivec3(dimension))))
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
		if (octantVector[index].children[child] >= dataMasks.leafBit) {
			// create new inner octant with blank leaves
			octantVector[index].children[child] = octantVector.size();
			octantVector.emplace_back(dataMasks.leafBit);
		}

		index = octantVector[index].children[child];
	}

	extent /= 2;
	glm::bvec3 mask = glm::greaterThanEqual(pos, offset + extent);

	int child = int(mask.x) + int(mask.y) * 2 + int(mask.z) * 4;

	// don't overwrite leaves
	if (octantVector[index].children[child] == dataMasks.leafBit) {
		//auto leafExist = leafMap.try_emplace((uint64_t(normal) << 32) | color, leafBit | leafVector.size());

		//if (leafExist.second) {
			// create new leaf
		octantVector[index].children[child] = dataMasks.leafBit | leafVector.size();
		leafVector.emplace_back(color, normal);
		//}
		/*else {
				reuse existing leaf
			octantVector[index].children[child] = (*leafExist.first).second;
		}*/
	}
}

template <typename InnerOctantType, typename LeafType, typename MaskType>
void SVO<typename InnerOctantType, typename LeafType, typename MaskType>::printTree(bool verbose) {
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

template <typename InnerOctantType, typename LeafType, typename MaskType>
void SVO<typename InnerOctantType, typename LeafType, typename MaskType>::save(const std::string filename) {
	std::ofstream fout("./assets/" + filename, std::ios::out | std::ios::binary);


	// number of inner octant levels
	fout.write(reinterpret_cast<const char*>(&levels), sizeof(levels));

	// number of inner octants
	unsigned int levelOctants = octantVector.size();
	fout.write(reinterpret_cast<const char*>(&levelOctants), sizeof(levelOctants));
	// octants
	fout.write(reinterpret_cast<const char*>(&octantVector[0]), levelOctants * sizeof(InnerOctantType));


	// number of leaves
	unsigned int leavesCount = leafVector.size();
	fout.write(reinterpret_cast<const char*>(&leavesCount), sizeof(leavesCount));

	// leaf data
	fout.write(reinterpret_cast<const char*>(&leafVector[0]), leavesCount * sizeof(LeafType));


	fout.close();
}
