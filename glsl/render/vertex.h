#pragma once

#include "color.h"

#include <glm/glm.hpp>

namespace te
{

class vertex
{
public:
    vertex();
    vertex(const glm::vec3& v, const color& c);
public:
    glm::vec3 position;
    color vColor;
};

}
