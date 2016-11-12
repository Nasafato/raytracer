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

    friend std::ostream &operator<<(std::ostream &os, BoundingBox &bbox) {
        return os<<"<"<<bbox.minPoint_<<","<<bbox.maxPoint_<<">";
    }

};

class Surface {

public:
    Material* material_;
    Point center_;
    BoundingBox bbox_;
    virtual Intersection intersect(Ray, double, double, int) = 0;
    virtual void getType() = 0;
    virtual BoundingBox getBoundingBox() = 0;
};

class Sphere: public Surface {

protected:
    double radius;

public:
    Intersection intersect(Ray, double, double, int);
    Sphere(Point, double, Material *);
    BoundingBox getBoundingBox();
    void getType();
};

class Plane: public Surface {

private:
    Vec3 normal_;
    double d_;

public:
    Intersection intersect(Ray, double, double, int);
    Plane(Vec3, double d, Material *);
    BoundingBox getBoundingBox();
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
    BoundingBox getBoundingBox();
    void getType();
};

class BvhNode: public Surface {

public:
    Intersection intersect(Ray, double, double, int);
    BvhNode();
    BvhNode(std::vector<Surface *>, int, std::vector<BvhNode *>);
    Surface *left_;
    Surface *right_;
    BoundingBox combine(BoundingBox, BoundingBox);
    BoundingBox getBoundingBox();
    // void createNode(std::vector<Surface *>, int);
    void getType();

};


#endif