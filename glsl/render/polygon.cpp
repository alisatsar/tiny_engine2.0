#include "polygon.h"

using namespace te;

polygon::polygon()
{
    for(int i; i < vertexes.size(); ++i)
    {
        vertexes[i] = vertex();
    }
}

polygon::polygon(const glm::vec3& p0, const glm::vec3& p1,
                 const glm::vec3& p2, const color& c)
{
    vertexes[0] = vertex(p0, c);
    vertexes[1] = vertex(p1, c);
    vertexes[2] = vertex(p2, c);
}

polygon::polygon(const glm::vec3& p0, const color& c0,
                 const glm::vec3& p1, const color& c1,
                 const glm::vec3& p2, const color& c2)
{
    vertexes[0] = vertex(p0, c0);
    vertexes[1] = vertex(p1, c1);
    vertexes[2] = vertex(p2, c2);
}
