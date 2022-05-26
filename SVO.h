#pragma once

#include "robin_hood.h"

#include <string>
#include <vector>

#include <glm/glm.hpp>


// A sparse octree with merged leaves
class SVO
{
public:
	struct InnerOctant {
		uint32_t children[8];    // children indices
		//uint32_t color;    // RGBA format - 8 bits per channel
	};

	struct Leaf {
		uint32_t color;    // RGBA format - 8 bits per channel
		// add normal, metallic, roughness, specular
	};

private:
	std::vector<std::vector<InnerOctant>> octantGraph;
	std::vector<InnerOctant> octantArray;

	std::vector<Leaf> leafVector;
	robin_hood::unordered_flat_map<uint32_t, uint32_t> leafMap;  // leaf color and vector index

	const uint32_t leafBit = 0x80000000U,
				   leafIdx = 0x7FFFFFFFU;

	uint32_t dimension;
	uint32_t levels;

public:
	SVO(const uint32_t levels);
	SVO(const std::string filename);

	const InnerOctant* getInnerOctantData();
	uint32_t getInnerOctantsSize() const;

	const Leaf* getLeafData() const;
	uint32_t getLeavesSize() const;

	void addElement(int x, int y, int z, uint32_t color);

	void printTree(bool verbose);

	void save(const std::string filename);
};
