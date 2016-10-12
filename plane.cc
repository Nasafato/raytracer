#include "plane.h"

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
    Point p = ray.origin + (ray.direction * t);
    Vec3 normal = normal_;
    normal.normalize();

    intersection.intersected_ = true;
    intersection.t_ = t;
    intersection.closestPoint_ = p;
    intersection.surfaceNormal_ = normal;

    return intersection;


}