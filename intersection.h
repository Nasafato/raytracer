#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "point.h"

class Intersection {

public:
    bool intersected_;
    double discriminant_;
    double t_;
    Point closestPoint_;
    Vec3 surfaceNormal_;
    Intersection();
    Intersection(bool, double, double, Point);
};

#endif