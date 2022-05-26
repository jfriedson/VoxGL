#pragma once

#include "robin_hood.h"

#include <array>
#include <string>

#include <glm/glm.hpp>


class HashGrid
{
public:
	const uint32_t dimension;
	const uint32_t static material_count = 2U;

	struct Material {
		uint32_t color;
	};

	enum materials {
		rock,
		water
	};

	struct Particle {
		uint8_t mat;

		Particle(uint8_t mat) : mat(mat) {}
	};

	robin_hood::unordered_flat_map<uint32_t, Particle> voxels;
	std::array<Material, material_count> materials;

public:
	HashGrid(const uint32_t dimension);
	~HashGrid();
};
