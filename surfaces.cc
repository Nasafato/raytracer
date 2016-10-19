#include <iostream>
#include <cmath>
#include "surfaces.h"

Sphere::Sphere(Point ncenter, double nradius, Material* nm) {
    center = ncenter;
    radius = nradius;
    material_ = Material(nm->dr, nm->dg, nm->db, nm->sr, nm->sg, nm->sb, nm->ir, nm->ig, nm->ib, nm->r);
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
        Point closestPoint = ray.origin + (ray.direction * minT);

        intersection.closestPoint_ = closestPoint;
        intersection.t_ = minT;
        intersection.intersected_ = true;
        intersection.discriminant_ = discriminant;
        intersection.surfaceNormal_ = (closestPoint - center) / radius;
        if (intersection.surfaceNormal_.magnitude() > 1.0) {
            intersection.surfaceNormal_.normalize();
        }
        // std::cout << "Closest point is " << intersection.closestPoint_ << std::endl;
    }

    return intersection;
        // std::cout << "A is " << a << ", B is " << b << ", C is " << cCoeff << std::endl;
}

// Vec3 Sphere::getPointLightVector(Ray ray, double t, PointLight light) {
//     Vec3 lightVector = light.position.toVec3() - (ray.origin.toVec3() + (ray.direction * t));
//     return lightVector;
// }

void Sphere::print() {
    std::cout  << ", " << radius << std::endl;
}


Plane::Plane(Vec3 normal, double d, Material *nm) {
    normal_ = normal;
    d_ = d;
    material_ = Material(nm->dr, nm->dg, nm->db, nm->sr, nm->sg, nm->sb, nm->ir, nm->ig, nm->ib, nm->r);
}

Intersection Plane::intersect(Ray ray) {
    Intersection intersection;

    double numerator = -(ray.origin.toVec3().dot(normal_) - d_);
    double denominator = ray.direction.dot(normal_);
    if (denominator == 0.0) {
        return intersection;
    }

    double t = numerator / denominator;
    if (t >= 0.0) {
        Point p = ray.origin + (ray.direction * t);
        Vec3 normal = normal_;
        normal.normalize();

        intersection.intersected_ = true;
        intersection.t_ = t;
        intersection.closestPoint_ = p;
        intersection.surfaceNormal_ = normal;
    }

    return intersection;


}