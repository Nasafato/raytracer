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
    virtual Vec3 getSurfaceNormal(Ray, float) = 0;
    virtual Vec3 getPointLightVector(Ray, float, PointLight) = 0;
    virtual Intersection intersect(Ray) = 0;
    virtual void print() = 0;
};

class Sphere: public Surface {

protected:
    Point center;
    float radius;

public:
    Intersection intersect(Ray);
    void print();
    Vec3 getSurfaceNormal(Ray, float);
    Vec3 getPointLightVector(Ray, float, PointLight);

    Sphere(Point, float, Material *);
};

#endif