#pragma once

namespace te
{

class color
{
public:
    color();
    color(float aR, float aG, float aB);
    color(float aR, float aG, float aB, float aA);

    float r = 0.f;
    float g = 0.f;
    float b = 0.f;
    float a = 0.f;
};

}
