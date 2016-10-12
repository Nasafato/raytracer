#ifndef POINT_H
#define POINT_H

#include "vec3.h"

class Point {
protected:
    float x, y, z;

public:
    Point();
    Point(float, float, float);
    void setX(float);
    void setY(float);
    void setZ(float) ;
    Vec3 operator- (const Point&) const;

    Vec3 toVec3();
    friend std::ostream &operator<<(std::ostream &os, Point &p) {
        return os<<"<"<<p.x<<","<<p.y<<","<<p.z<<">";
    }
};

#endif