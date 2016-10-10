#ifndef SURFACES_H
#define SURFACES_H

#include "vec3.h"
#include "point.h"
#include "ray.h"
#include "material.h"

class Surface {

public:
    Material material;
    virtual float intersect(Ray) = 0;
    virtual void print() = 0;
};

class Sphere: public Surface {

protected:
    Vec3 center;
    float radius;

public:
    float intersect(Ray);
    void print();

    Sphere(Vec3, float, Material *);
};

#endif