#include <iostream>
#include <cmath>
#include "surfaces.h"

Sphere::Sphere(Vec3 ncenter, float nradius, Material* nm) {
    center = ncenter;
    radius = nradius;
    material = Material(nm->dr, nm->dg, nm->db, nm->sr, nm->sg, nm->sb, nm->ir, nm->ig, nm->ib, nm->r);
}

float Sphere::intersect(Ray ray) {

    Vec3 d = ray.direction;
    Vec3 e = ray.origin.toVec3();
    Vec3 c = center;

/*
    std::cout << "ray direction is ";
    d.print();
    std::cout << "ray origin is ";
    e.print();
    std::cout << "circle center is ";
    c.print();
    */

    float a = (d.dot(d));
    float b = (d * 2.0).dot((e - c));
    float cCoeff = (e - c).dot((e - c)) - radius * radius;


    float discriminant = b * b - 4 * a * cCoeff;
        // std::cout << "Discriminant is " << discriminant << std::endl;
    if (discriminant >= 0.0) {
        float t = (-b + discriminant) / (2 * a);
        float t_2 = (-b - discriminant) / (2 * a);
        // std::cout << "t is " << t << ", t_2 is " << t_2 << std::endl;
        if (t < t_2) {
            return t;
        } else {
            return t_2;
        }
    } else {
        return -1.0;
    }

    return discriminant;
        // std::cout << "A is " << a << ", B is " << b << ", C is " << cCoeff << std::endl;
}

Vec3 Sphere::getSurfaceNormal(Ray ray, float t) {
    Vec3 unitNormal = (ray.origin.toVec3() + (ray.direction * t)) - center;
    return unitNormal;
}

Vec3 Sphere::getPointLightVector(Ray ray, float t, PointLight light) {
    Vec3 lightVector = light.position.toVec3() - (ray.origin.toVec3() + (ray.direction * t));
    return lightVector;
}

void Sphere::print() {
    std::cout  << ", " << radius << std::endl;
}