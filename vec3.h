#ifndef VEC3_H
#define VEC3_H

#include <iostream>

class Vec3 {

public:
    Vec3();
    Vec3(float, float, float);

    Vec3 operator * (const float&) const;
    Vec3 operator * (const Vec3&) const;
    Vec3 operator - (const Vec3&) const;
    Vec3 operator + (const Vec3&) const;

    float magnitude() const;
    float dot(const Vec3&) const;

    Vec3 cross(const Vec3&) const;
    Vec3& normalize();
    void print() const;
    Vec3 reverse() const;

    friend std::ostream &operator<<(std::ostream &os, Vec3 &v) {
        return os<<"<"<<v.x<<","<<v.y<<","<<v.z<<">";
    }

    float x, y, z;
};

#endif




