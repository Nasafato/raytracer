#ifndef VEC3_H
#define VEC3_H

#include <iostream>

class Vec3 {

public:
    Vec3();
    Vec3(double, double, double);

    Vec3 operator * (const double&) const;
    Vec3 operator * (const Vec3&) const;
    Vec3 operator - (const Vec3&) const;
    Vec3 operator + (const Vec3&) const;
    Vec3 operator / (const double&) const;
    Vec3& operator += (const Vec3&);
    Vec3& operator *= (const double& m);
    Vec3& operator *= (const Vec3& v);

    bool operator == (const Vec3&) const;

    double magnitude() const;
    double dot(const Vec3&) const;

    Vec3 cross(const Vec3&) const;
    Vec3& normalize();
    void print() const;
    Vec3 reverse() const;

    friend std::ostream &operator<<(std::ostream &os, Vec3 &v) {
        return os<<"<"<<v.x<<","<<v.y<<","<<v.z<<">";
    }

    double x, y, z;
};

#endif




