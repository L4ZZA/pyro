#pragma once
#include <vector>
#include <glm/glm.hpp>

struct corridor
{
    std::vector<int> tiles_indexes;
    glm::ivec2 start_tile;
    glm::ivec2 end_tile;
};

