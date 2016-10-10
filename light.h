#ifndef LIGHT_H
#define LIGHT_H

#include "point.h"

class Light {

public:
    float r, g, b;
};

class PointLight: public Light {
public:
    Point position;
    PointLight();
    PointLight(Point, float, float, float);
};

#endif
