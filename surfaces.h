#ifndef SURFACES_H
#define SURFACES_H

#include "vec3.h"
#include "point.h"
#include "ray.h"
#include "material.h"
#include "light.h"
#include "intersection.h"


class BoundingBox {

public:
    Point minPoint_;
    Point maxPoint_;
    BoundingBox();
    BoundingBox(Point, Point);
    bool intersect(Ray, double, double);
};

class Surface {

public:
    Material* material_;
    BoundingBox boundingBox_;
    virtual Intersection intersect(Ray, double, double) = 0;
    virtual void getType() = 0;
};

class Sphere: public Surface {

protected:
    Point center;
    double radius;

public:
    Intersection intersect(Ray, double, double);
    Sphere(Point, double, Material *);
    void getType();
};

class Plane: public Surface {

private:
    Vec3 normal_;
    double d_;

public:
    Intersection intersect(Ray, double, double);
    Plane(Vec3, double d, Material *);
    void getType();
};

class Triangle: public Surface {

private:
    Point p1_, p2_, p3_;
    Vec3 normal_;

public:
    Intersection intersect(Ray, double, double);
    Triangle(Point, Point, Point, Vec3, Material *);
    void getType();
};



#endif