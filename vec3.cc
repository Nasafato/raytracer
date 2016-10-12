#include <cmath>
#include "vec3.h"


Vec3::Vec3() {
    x = 0;
    y = 0;
    z = 0;
}

Vec3::Vec3(float xn, float yn, float zn) {
    x = xn;
    y = yn;
    z = zn;
}

Vec3 Vec3::operator * (const float& xn) const {
    return Vec3(x * xn, y * xn, z * xn);
}

Vec3 Vec3::operator * (const Vec3& v) const {
    return Vec3(x * v.x, y * v.y, z * v.z);
}

Vec3 Vec3::operator - (const Vec3& v) const {
    return Vec3(x - v.x, y - v.y, z - v.z);
}

Vec3 Vec3::operator + (const Vec3& v) const {
    return Vec3(x + v.x, y + v.y, z + v.z);
}

Vec3 Vec3::reverse() const {
    return Vec3(x * -1.0, y * -1.0, z * -1.0);
}

float Vec3::magnitude() const {
    return sqrt(x * x + y * y + z * z);
}

float Vec3::dot(const Vec3& v) const {
    return x * v.x + y * v.y + z * v.z;
}

Vec3 Vec3::cross(const Vec3& v) const {
    float c1 = y * v.z - z * v.y;
    float c2 = -(x * v.z - z * v.x);
    float c3 = x * v.y - y * v.x;

    return Vec3(c1, c2, c3);
}

Vec3& Vec3::normalize() {
    float mag = magnitude();
    x /= mag;
    y /= mag;
    z /= mag;
    return *this;
}

void Vec3::print() const {
    std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
}


