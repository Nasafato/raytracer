#ifndef RAY_H
#define RAY_H

#include "point.h"
#include "vec3.h"

class Ray {
public:
    Ray(Point, Vec3);
    Point origin;
    Vec3 direction;
};

#endif