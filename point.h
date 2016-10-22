#ifndef POINT_H
#define POINT_H

#include "vec3.h"

class Point {

public:
    double x, y, z;
    Point();
    Point(double, double, double);
    void setX(double);
    void setY(double);
    void setZ(double) ;
    Vec3 operator- (const Point&) const;
    Point operator+ (const Vec3 &v) const;

    Vec3 toVec3();
    friend std::ostream &operator<<(std::ostream &os, Point &p) {
        return os<<"<"<<p.x<<","<<p.y<<","<<p.z<<">";
    }
};

#endif