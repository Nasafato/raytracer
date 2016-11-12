#ifndef SURFACES_H
#define SURFACES_H

#include "vec3.h"
#include "point.h"
#include "ray.h"
#include "material.h"
#include "light.h"
#include "intersection.h"
#include <vector>

class BoundingBox {

public:
    Point minPoint_;
    Point maxPoint_;
    BoundingBox();
    BoundingBox(Point, Point);
    Intersection intersect(Ray, double, double, int);
};

class Surface {

public:
    Material* material_;
    Point center_;
    BoundingBox boundingBox_;
    virtual Intersection intersect(Ray, double, double, int) = 0;
    virtual void getType() = 0;
};

class Sphere: public Surface {

protected:
    double radius;

public:
    Intersection intersect(Ray, double, double, int);
    Sphere(Point, double, Material *);
    void getType();
};

class Plane: public Surface {

private:
    Vec3 normal_;
    double d_;

public:
    Intersection intersect(Ray, double, double, int);
    Plane(Vec3, double d, Material *);
    void getType();
};

class Triangle: public Surface {

private:
    double a;
    double b;
    double c;

    double d;
    double e;
    double f;
    Point p1_, p2_, p3_;
    Vec3 normal_;

public:
    Intersection intersect(Ray, double, double, int);
    Triangle(Point, Point, Point, Vec3, Material *);
    void getType();
};

// class BvhNode: public Surface {

// public:
//     Intersection intersect(Ray, double, double);
//     BvhNode();
//     Surface *left_;
//     Surface *right_;
//     BoundingBox thisBoundingBox_;
//     void createNode(std::vector<Surface *>, int);
//     void getType();

// };


#endif