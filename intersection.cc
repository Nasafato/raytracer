#include "intersection.h"

Intersection::Intersection(bool intersected, double discriminant, double t, Point p1) {
    intersected_ = intersected;
    discriminant_ = discriminant;
    t_ = t;
    closestPoint_ = p1;
}