#include "vertex.h"

using namespace te;

vertex::vertex()
    : position(0.f, 0.f, 0.f)
    , vColor()
{
}

vertex::vertex(const glm::vec3& v, const color& c)
    : position(v)
    , vColor(c)
{
}
