#include <iostream>
#include <cmath>
#include "surfaces.h"

Sphere::Sphere(Vec3 ncenter, float nradius, Material* nm) {
    center = ncenter;
    radius = nradius;
    material = Material(nm->dr, nm->dg, nm->db, nm->sr, nm->sg, nm->sb, nm->ir, nm->ig, nm->ib, nm->r);
}

bool Sphere::intersect(Ray ray) {

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

    if (discriminant < 0.0) {
        // std::cout << "A is " << a << ", B is " << b << ", C is " << cCoeff << std::endl;
        // std::cout << "Discriminant is " << discriminant << std::endl;
        return false;
    } else {
        return true;
    }

}

void Sphere::print() {
    std::cout  << ", " << radius << std::endl;
}