#pragma once

#include "vertex.h"

#include <array>
#include <glm/glm.hpp>

namespace te
{

class polygon
{
public:
    polygon();
    polygon(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const color& c);
    polygon(const glm::vec3& p0, const color& c0, const glm::vec3& p1, const color& c1, const glm::vec3& p2, const color& c2);

public:
    std::array<vertex, 3> vertexes;
};

}
