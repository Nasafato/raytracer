#ifndef SURFACES_H
#define SURFACES_H

#include "vec3.h"
#include "point.h"
#include "ray.h"
#include "material.h"
#include "light.h"

class Surface {

public:
    Material material;
    virtual Vec3 getSurfaceNormal(Ray, float) = 0;
    virtual Vec3 getPointLightVector(Ray, float, PointLight) = 0;
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
    Vec3 getSurfaceNormal(Ray, float);
    Vec3 getPointLightVector(Ray, float, PointLight);

    Sphere(Vec3, float, Material *);
};

#endif