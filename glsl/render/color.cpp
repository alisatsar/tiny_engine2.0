#include "color.h"

using namespace te;

color::color(){}

color::color(float aR, float aG, float aB)
    : r(aR)
    , g(aG)
    , b(aB)
{}

color::color(float aR, float aG, float aB, float aA)
    : r(aR)
    , g(aG)
    , b(aB)
    , a(aA)
{}
