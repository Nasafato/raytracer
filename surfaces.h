#ifndef SURFACES_H
#define SURFACES_H

#include "vec3.h"
#include "point.h"
#include "ray.h"
#include "material.h"
#include "light.h"
#include "intersection.h"

class Surface {

public:
    Material material;
    virtual Intersection intersect(Ray) = 0;
    virtual void print() = 0;
};

class Sphere: public Surface {

protected:
    Point center;
    double radius;

public:
    Intersection intersect(Ray);
    void print();

    Sphere(Point, double, Material *);
};

#endif