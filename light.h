#ifndef LIGHT_H
#define LIGHT_H

#include "point.h"

class Light {

public:
    double r, g, b;
    Point position_;
};

class PointLight: public Light {
public:
    PointLight();
    PointLight(Point, double, double, double);
};

class AmbientLight {
public:
    double r_, g_, b_;
    AmbientLight();
    AmbientLight(double, double, double);
};



#endif
