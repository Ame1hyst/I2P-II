#include "function.h"
using namespace oj;

CuboidNut::CuboidNut(double l, double w, double h)
{
    if (l < 0 || w < 0 || h < 0) setVolume(0);
    else setVolume(l * w * h);
}

CubeNut::CubeNut(double s) : CuboidNut(s, s, s) {}

SphereNut::SphereNut(double r)
{
    if (r < 0) setVolume(0);
    else setVolume((4.0/3.0) * PI * r * r * r);
}

ConeNut::ConeNut(double r, double h)
{
    if (r < 0 || h < 0) setVolume(0);
    else setVolume((1.0/3.0) * PI * r * r * h);
}

CylinderNut::CylinderNut(double r, double h)
{
    if (r < 0 || h < 0) setVolume(0);
    else setVolume(PI * r * r * h);
}