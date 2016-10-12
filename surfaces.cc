#include <iostream>
#include <cmath>
#include "surfaces.h"

Sphere::Sphere(Point ncenter, float nradius, Material* nm) {
    center = ncenter;
    radius = nradius;
    material = Material(nm->dr, nm->dg, nm->db, nm->sr, nm->sg, nm->sb, nm->ir, nm->ig, nm->ib, nm->r);
}

Intersection Sphere::intersect(Ray ray) {

    Vec3 e_c = ray.origin - center;
    Vec3 d = ray.direction;
    double d_dot_d = d.dot(d);

/*
    std::cout << "ray direction is ";
    d.print();
    std::cout << "ray origin is ";
    e.print();
    std::cout << "circle center is ";
    c.print();
    */
    double discriminantTermOne = d.dot(e_c) * d.dot(e_c);
    double discriminantTermTwo = (e_c.dot(e_c) - (radius * radius)) * d_dot_d;

    double discriminant = discriminantTermOne - discriminantTermTwo;
    // std::cout << "Discriminant: " << discriminant << std::endl;


    Intersection intersection = Intersection(false, 0.0, 0.0, Point(0.0, 0.0, 0.0));

    if (discriminant >= 0.0) {
        double firstTerm = d.reverse().dot(e_c);
        double plusDiscriminant = (firstTerm + sqrt(discriminant)) / d_dot_d;
        double minusDiscriminant = (firstTerm - sqrt(discriminant)) / d_dot_d;
        double minT = std::min(std::max(plusDiscriminant, (double)0.0), std::max(minusDiscriminant, (double)0.0));

        intersection.closestPoint_ = ray.origin + (ray.direction * minT);
        intersection.t_ = minT;
        intersection.intersected_ = true;
        intersection.discriminant_ = discriminant;
        // std::cout << "Closest point is " << intersection.closestPoint_ << std::endl;
    }

    return intersection;
        // std::cout << "A is " << a << ", B is " << b << ", C is " << cCoeff << std::endl;
}

Vec3 Sphere::getSurfaceNormal(Ray ray, float t) {
    // Vec3 unitNormal = (ray.origin.toVec3() + (ray.direction * t)) - center;
    Vec3 unitNormal = Vec3(0.0, 0.0, 0.0);
    return unitNormal;
}

Vec3 Sphere::getPointLightVector(Ray ray, float t, PointLight light) {
    Vec3 lightVector = light.position.toVec3() - (ray.origin.toVec3() + (ray.direction * t));
    return lightVector;
}

void Sphere::print() {
    std::cout  << ", " << radius << std::endl;
}