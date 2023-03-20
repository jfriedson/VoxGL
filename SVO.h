#pragma once

#include "robin_hood.h"

#include <string>
#include <vector>

#include <glm/glm.hpp>


// A sparse octree with merged leaves
class SVO
{
public:
	static const uint32_t leafBit{ 0x80000000U },
						  leafIdx{ 0x7FFFFFFFU };

	struct InnerOctant {
		uint32_t children[8];    // children indices
		//uint32_t color;    // RGBA format - 8 bits per channel

		InnerOctant() {
			for (unsigned int c = 0; c < 8; ++c)
				children[c] = 0;
		}

		InnerOctant(const uint32_t val) {
			for (unsigned int c = 0; c < 8; ++c)
				children[c] = val;
		}
	};

	struct Leaf {
		uint32_t color;    // RGBA format - 8 bits per channel
		uint32_t normal;

		// add metallic, roughness, specular

		Leaf() : color{ 0 }, normal{ 0 }
		{}

		Leaf(const uint32_t color, const uint32_t normal) :
			color{ color },
			normal{ normal }
		{}
	};

private:
	std::vector<InnerOctant> octantVector;

	std::vector<Leaf> leafVector;

	//robin_hood::unordered_flat_map<uint64_t, uint32_t> leafMap;  // leaf color and vector index

	uint32_t dimension;
	uint32_t levels;

public:
	SVO(const uint32_t levels);
	SVO(const std::string filename);

	const InnerOctant* const getInnerOctantData() const;
	const uint32_t getInnerOctantsSize() const;
	void setInnerOctantsSize(const uint32_t);
	void reserveInnerOctants(const uint32_t);

	const Leaf* const getLeafData() const;
	const uint32_t getLeavesSize() const;
	void setLeavesSize(const uint32_t);
	void reserveLeaves(const uint32_t);

	void addElement(const glm::ivec3 coord, const uint32_t color, const uint32_t normal);

	void printTree(bool verbose);

	void save(const std::string filename);
};
