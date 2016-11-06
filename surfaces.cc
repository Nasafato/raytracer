#include <iostream>
#include <cmath>
#include "surfaces.h"

using namespace std;

BoundingBox::BoundingBox() {
    minPoint_ = Point();
    maxPoint_ = Point();
}

BoundingBox::BoundingBox(Point minPoint, Point maxPoint) {
    minPoint_ = minPoint;
    maxPoint_ = maxPoint;
}

bool BoundingBox::intersect(Ray ray, double minT, double maxT) {
    Vec3 d = ray.direction;

    double t_xmin;
    double t_xmax;
    double t_ymin;
    double t_ymax;
    double t_zmin;
    double t_zmax;

    double a_x = 1 / d.x;
    if (a_x >= 0) {
        t_xmin = a_x * (minPoint_.x - ray.origin.x);
        t_xmax = a_x * (maxPoint_.x - ray.origin.x);
    } else {
        t_xmax = a_x * (minPoint_.x - ray.origin.x);
        t_xmin = a_x * (maxPoint_.x - ray.origin.x);
    }

    double a_y = 1 / d.y;
    if (a_y >= 0) {
        t_ymin = a_y * (minPoint_.y - ray.origin.y);
        t_ymax = a_y * (maxPoint_.y - ray.origin.y);
    } else {
        t_ymax = a_y * (minPoint_.y - ray.origin.y);
        t_ymin = a_y * (maxPoint_.y - ray.origin.y);
    }

    double a_z = 1 / d.z;
    if (a_z >= 0) {
        t_zmin = a_z * (minPoint_.z - ray.origin.z);
        t_zmax = a_z * (maxPoint_.z - ray.origin.z);
    } else {
        t_zmax = a_z * (minPoint_.z - ray.origin.z);
        t_zmin = a_z * (maxPoint_.z - ray.origin.z);
    }

    if ((t_xmin > t_ymax) || (t_xmin > t_zmax) || (t_ymin > t_xmax) || (t_ymin > t_zmax) || (t_zmin > t_xmax) || (t_zmin > t_ymax)) {
        return false;
    } else {
        return true;
    }
}

Sphere::Sphere(Point ncenter, double nradius, Material* nm) {
    center = ncenter;
    radius = nradius;

    Vec3 minVec = Vec3(-nradius, -nradius, -nradius);
    Vec3 maxVec = Vec3(nradius, nradius, nradius);

    Point minPoint = ncenter + minVec;
    Point maxPoint = ncenter + maxVec;

    boundingBox_ = BoundingBox(minPoint, maxPoint);
    material_ = Material(nm->dr, nm->dg, nm->db, nm->sr, nm->sg, nm->sb, nm->ir, nm->ig, nm->ib, nm->r);
}


Intersection Sphere::intersect(Ray ray, double minT, double maxT) {

    Intersection intersection = Intersection(false, 0.0, 0.0, Point(0.0, 0.0, 0.0));

    if (!boundingBox_.intersect(ray, minT, maxT)) {
        return intersection;
    }

    Vec3 e_c = ray.origin - center;
    Vec3 d = ray.direction;
    double d_dot_d = d.dot(d);
    double discriminantTermOne = d.dot(e_c) * d.dot(e_c);
    double discriminantTermTwo = (e_c.dot(e_c) - (radius * radius)) * d_dot_d;
    double discriminant = discriminantTermOne - discriminantTermTwo;


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
    Point minPoint = Point(p1.x, p1.y, p1.z);
    Point maxPoint = Point(p1.x, p1.y, p1.z);

    p1_ = p1;
    p2_ = p2;
    p3_ = p3;

    minPoint.x = min(min(p1.x, p2.x), p3.x);
    minPoint.y = min(min(p1.y, p2.y), p3.y);
    minPoint.z = min(min(p1.z, p2.z), p3.z);

    maxPoint.x = max(max(p1.x, p2.x), p3.x);
    maxPoint.y = max(max(p1.y, p2.y), p3.y);
    maxPoint.z = max(max(p1.z, p2.z), p3.z);

    boundingBox_ = BoundingBox(minPoint, maxPoint);

    normal_ = normal;
    material_ = Material(nm->dr, nm->dg, nm->db, nm->sr, nm->sg, nm->sb, nm->ir, nm->ig, nm->ib, nm->r);
}

Intersection Triangle::intersect(Ray ray, double minT, double maxT) {
    Intersection intersection;

    if (!boundingBox_.intersect(ray, minT, maxT)) {
        return intersection;
    }

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