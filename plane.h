#ifndef PLANE_H
#define PLANE_H

#include "surfaces.h"
#include "vec3.h"
#include "intersection.h"
#include "point.h"
#include "material.h"
#include "ray.h"

class Plane: public Surface {

private:
    Vec3 normal_;
    double d_;
    Material material_;

public:
    Intersection intersect(Ray);
    Plane(Vec3, double d, Material *);
};

#endif