#include "point.h"
#include "vec3.h"

Point::Point() {
    x = 0.0;
    y = 0.0;
    z = 0.0;
}

Point::Point(double xn, double yn, double zn) {
    x = xn;
    y = yn;
    z = zn;
}

Vec3 Point::operator- (const Point &p) const {
    return Vec3(x - p.x, y - p.y, z - p.z);
}

Point Point::operator+ (const Vec3 &v) const {
    return Point(x + v.x, y + v.y, z + v.z);
}

void Point::setX(double xn) {
    x = xn;
}

void Point::setY(double yn) {
    y = yn;
}

void Point::setZ(double zn) {
    z = zn;
}

Vec3 Point::toVec3() {
    return Vec3(x, y, z);
}