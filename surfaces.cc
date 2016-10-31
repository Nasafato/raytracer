#include <iostream>
#include <cmath>
#include "surfaces.h"

Sphere::Sphere(Point ncenter, double nradius, Material* nm) {
    center = ncenter;
    radius = nradius;
    material_ = Material(nm->dr, nm->dg, nm->db, nm->sr, nm->sg, nm->sb, nm->ir, nm->ig, nm->ib, nm->r);
}


Intersection Sphere::intersect(Ray ray, double minT, double maxT) {

    Vec3 e_c = ray.origin - center;
    Vec3 d = ray.direction;
    double d_dot_d = d.dot(d);
    double discriminantTermOne = d.dot(e_c) * d.dot(e_c);
    double discriminantTermTwo = (e_c.dot(e_c) - (radius * radius)) * d_dot_d;
    double discriminant = discriminantTermOne - discriminantTermTwo;

    Intersection intersection = Intersection(false, 0.0, 0.0, Point(0.0, 0.0, 0.0));

    if (discriminant >= 0.0) {
        double firstTerm = d.reverse().dot(e_c);
        double plusDiscriminant = (firstTerm + sqrt(discriminant)) / d_dot_d;
        double minusDiscriminant = (firstTerm - sqrt(discriminant)) / d_dot_d;
        if (plusDiscriminant < 0.0 and minusDiscriminant < 0.0) {
            return intersection;
        }
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
    }

    return intersection;
}

Plane::Plane(Vec3 normal, double d, Material *nm) {
    normal_ = normal;
    d_ = d;
    material_ = Material(nm->dr, nm->dg, nm->db, nm->sr, nm->sg, nm->sb, nm->ir, nm->ig, nm->ib, nm->r);
}


Intersection Plane::intersect(Ray ray, double minT, double maxT) {
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

Triangle::Triangle(Point p1, Point p2, Point p3, Vec3 normal, Material *nm) {
    p1_ = p1;
    p2_ = p2;
    p3_ = p3;
    normal_ = normal;
    material_ = Material(nm->dr, nm->dg, nm->db, nm->sr, nm->sg, nm->sb, nm->ir, nm->ig, nm->ib, nm->r);
}

Intersection Triangle::intersect(Ray ray, double minT, double maxT) {
    Intersection intersection;
    double a = p1_.x - p2_.x;
    double b = p1_.y - p2_.y;
    double c = p1_.z - p2_.z;

    double d = p1_.x - p3_.x;
    double e = p1_.y - p3_.y;
    double f = p1_.z - p3_.z;

    double j = p1_.x - ray.origin.x;
    double k = p1_.y - ray.origin.y;
    double l = p1_.z - ray.origin.z;

    double g = ray.direction.x;
    double h = ray.direction.y;
    double i = ray.direction.z;

    double M = a * (e * i - h * f) + b * (g * f - d * i) + c * (d * h - e * g);
    double t = -(f * (a * k - j * b) + e * (j * c - a * l) + d * (b * l - k * c)) / M;

    if (t < 0.0) {
        return intersection;
    }

    double gamma = (i * (a * k - j * b) + h * (j * c - a * l) + g * (b * l - k * c)) / M;
    if ((gamma < 0.0) || (gamma > 1.0)) {
        return intersection;
    }

    double beta = (j * (e * i - h * f) + k * (g * f - d * i) + l * (d * h - e * g)) / M;
    if ((beta < 0.0) || (beta > 1.0 - gamma)) {
        return intersection;
    }

    intersection.t_ = t;
    intersection.intersected_ = true;
    intersection.closestPoint_ = ray.origin + ray.direction * t;
    intersection.surfaceNormal_ = normal_;

    return intersection;
}

void Plane::getType() {
    std::cout << "plane";
}

void Sphere::getType() {
    std::cout << "sphere";
}

void Triangle::getType() {
    std::cout << "triangle";
}