#ifndef LIGHT_H
#define LIGHT_H

#include "point.h"

class Light {

public:
    double r, g, b;
};

class PointLight: public Light {
public:
    Point position;
    PointLight();
    PointLight(Point, double, double, double);
};

#endif
