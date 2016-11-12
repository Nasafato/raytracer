#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "point.h"
#include "material.h"

class Intersection {

public:
    bool intersected_;
    double discriminant_;
    double t_;
    Point closestPoint_;
    Vec3 surfaceNormal_;
    Material* material_;
    Intersection();
    Intersection(bool, double, double, Point);
};

#endif