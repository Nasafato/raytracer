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
    Material material_;
    virtual Intersection intersect(Ray) = 0;
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

class Plane: public Surface {

private:
    Vec3 normal_;
    double d_;

public:
    Intersection intersect(Ray);
    Plane(Vec3, double d, Material *);
};

class Triangle: public Surface {

private:
    Point p1_, p2_, p3_;
    Vec3 normal_;

public:
    Intersection intersect(Ray);
    Triangle(Point, Point, Point, Vec3, Material *);
};



#endif