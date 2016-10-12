#include "intersection.h"

Intersection::Intersection() {
    intersected_ = false;
    discriminant_ = 0.0;
    t_ = 0.0;
    closestPoint_ = Point();

}


Intersection::Intersection(bool intersected, double discriminant, double t, Point p1) {
    intersected_ = intersected;
    discriminant_ = discriminant;
    t_ = t;
    closestPoint_ = p1;
}