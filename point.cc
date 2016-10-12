#include "point.h"
#include "vec3.h"

Point::Point() {
    x = 0.0;
    y = 0.0;
    z = 0.0;
}

Point::Point(float xn, float yn, float zn) {
    x = xn;
    y = yn;
    z = zn;
}

Vec3 Point::operator- (const Point &p) const {
    return Vec3(x - p.x, y - p.y, z - p.z);
}


void Point::setX(float xn) {
    x = xn;
}

void Point::setY(float yn) {
    y = yn;
}

void Point::setZ(float zn) {
    z = zn;
}

Vec3 Point::toVec3() {
    return Vec3(x, y, z);
}