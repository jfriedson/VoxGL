#include "HashGrid.h"


HashGrid::HashGrid(const uint32_t dimension) : dimension(dimension) {
	materials[rock].color = glm::packUnorm4x8(glm::vec4(.6f, .6f, .6f, 1.f));
	materials[water].color = glm::packUnorm4x8(glm::vec4(.1f, .2f, .9f, 0.6f));
}


HashGrid::~HashGrid()
{
}
