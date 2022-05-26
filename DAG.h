#pragma once

//#include "ESVO.h"

#include <vector>

#include <glm/glm.hpp>



class DAG
{
public:
	struct InnerOctant {
		uint32_t children[8];    // children indices
		//uint32_t color;    // RGBA format - 8 bits per channel
	};

	struct Leaf {
		uint32_t color;    // RGBA format - 8 bits per channel
	};

private:
	std::vector<std::vector<InnerOctant>> innerOctants;
	std::vector<Leaf> leaves;

	std::vector<std::vector<uint16_t>> references;

	std::vector<InnerOctant> newOctants;

	const uint32_t leafBit = 0x80000000U,
				   leafIdx = 0x7FFFFFFFU;

	//glm::uvec3 dimensions;
	uint32_t dimension;
	uint32_t levels;

public:
	DAG(const uint32_t levels);
	//DAG(ESVO esvo);

	const InnerOctant* getInnerOctantData();
	uint32_t getInnerOctantsSize() const;

	const Leaf* getLeafData() const;
	uint32_t getLeavesSize() const;

	void addElement(int x, int y, int z, uint32_t color);

	void printTree(bool verbose = false);
	void printReferences();
};
