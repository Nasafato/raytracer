#include <iostream>
#include <cmath>
#include <algorithm>
#include "surfaces.h"
#include "constants.h"

using namespace std;

BoundingBox::BoundingBox() {
    minPoint_ = Point();
    maxPoint_ = Point();
}

BoundingBox::BoundingBox(Point minPoint, Point maxPoint) {
    minPoint_ = minPoint;
    maxPoint_ = maxPoint;
}

bool BoundingBox::intersect(Ray& ray, Intersection& intersection, double minT, double maxT, int flag) {
    double t_xmin;
    double t_xmax;
    double t_ymin;
    double t_ymax;
    double t_zmin;
    double t_zmax;
    double tmin;
    double tmax;
    Vec3 d = ray.direction;
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


    if ((t_xmin > t_ymax) || (t_xmin > t_zmax)
        || (t_ymin > t_xmax) || (t_ymin > t_zmax)
        || (t_zmin > t_xmax) || (t_zmin > t_ymax)) {
        return false;
    } else {
        t_xmin = max((double)0.0, t_xmin);
        t_ymin = max((double)0.0, t_ymin);
        t_zmin = max((double)0.0, t_zmin);
        intersection.t_ = max(max(t_xmin, t_ymin), t_zmin);
        if (intersection.t_ <= 0.0) {
            return false;
        }
        intersection.intersected_ = true;
        intersection.closestPoint_ = ray.origin + d * intersection.t_;
        Vec3 normal;
        if (intersection.t_ == t_xmin) {
            normal = a_x >= 0 ? Vec3(-1.0, 0.0, 0.0) : Vec3(1.0, 0.0, 0.0);
        } else if (intersection.t_ == t_ymin) {
            normal = a_y >= 0 ? Vec3(0.0, -1.0, 0.0) : Vec3(0.0, 1.0, 0.0);
        } else if (intersection.t_ == t_zmin) {
            normal = a_z >= 0 ? Vec3(0.0, 0.0, -1.0) : Vec3(0.0, 0.0, 1.0);
        }
        intersection.surfaceNormal_ = normal;
        return true;
    }
}


Sphere::Sphere(Point ncenter, double nradius, Material* nm) {
    center_ = ncenter;
    radius = nradius;

    Vec3 minVec = Vec3(-nradius, -nradius, -nradius);
    Vec3 maxVec = Vec3(nradius, nradius, nradius);

    Point minPoint = ncenter + minVec;
    Point maxPoint = ncenter + maxVec;

    bbox_ = BoundingBox(minPoint, maxPoint);
    material_ = nm;
}


bool Sphere::intersect(Ray& ray, Intersection& intersection, double minT, double maxT, int flag) {
    intersection.material_ = material_;

    if (!bbox_.intersect(ray, intersection, minT, maxT, flag)) {
        return false;
    }

    if (flag == BBOX_ONLY) {
        return true;
    }

    Vec3 e_c = ray.origin - center_;
    Vec3 d = ray.direction;
    double d_dot_d = d.dot(d);
    double discriminantTermOne = d.dot(e_c) * d.dot(e_c);
    double discriminantTermTwo = (e_c.dot(e_c) - (radius * radius)) * d_dot_d;
    double discriminant = discriminantTermOne - discriminantTermTwo;
    if (discriminant < 0.0) {
        return false;
    }

    double firstTerm = d.reverse().dot(e_c);
    double plusDiscriminant = (firstTerm + sqrt(discriminant)) / d_dot_d;
    double minusDiscriminant = (firstTerm - sqrt(discriminant)) / d_dot_d;
    if (plusDiscriminant < 0.0 and minusDiscriminant < 0.0) {
        return false;
    }
    double minimumT = min(max(plusDiscriminant, (double)0.0), max(minusDiscriminant, (double)0.0));
    Point closestPoint = ray.origin + (ray.direction * minimumT);

    intersection.closestPoint_ = closestPoint;
    intersection.t_ = minimumT;
    intersection.intersected_ = true;
    intersection.discriminant_ = discriminant;
    intersection.surfaceNormal_ = (closestPoint - center_) / radius;
    if (intersection.surfaceNormal_.magnitude() > 1.0) {
        intersection.surfaceNormal_.normalize();
    }

    return true;
}

Plane::Plane(Vec3 normal, double d, Material *nm) {
    normal_ = normal;
    d_ = d;
    material_ = nm;
}


bool Plane::intersect(Ray& ray, Intersection& intersection, double minT, double maxT, int flag) {
    double numerator = -(ray.origin.toVec3().dot(normal_) - d_);
    double denominator = ray.direction.dot(normal_);
    if (denominator == 0.0) {
        return false;
    }

    double t = numerator / denominator;
    if (t < 0.0) {
        return false;
    }

    Point p = ray.origin + (ray.direction * t);
    Vec3 normal = normal_;
    normal.normalize();

    intersection.intersected_ = true;
    intersection.t_ = t;
    intersection.closestPoint_ = p;
    intersection.surfaceNormal_ = normal;
    return true;
}

Triangle::Triangle(Point p1, Point p2, Point p3, Vec3 normal, Material *nm) {
    Point minPoint = Point(p1.x, p1.y, p1.z);
    Point maxPoint = Point(p1.x, p1.y, p1.z);

    double cx = (p1.x + p2.x + p3.x) / 3;
    double cy = (p1.y + p2.y + p3.y) / 3;
    double cz = (p1.z + p2.z + p3.z) / 3;

    center_ = Point(cx, cy, cz);

    p1_ = p1;
    p2_ = p2;
    p3_ = p3;

    minPoint.x = min(min(p1.x, p2.x), p3.x);
    minPoint.y = min(min(p1.y, p2.y), p3.y);
    minPoint.z = min(min(p1.z, p2.z), p3.z);

    maxPoint.x = max(max(p1.x, p2.x), p3.x);
    maxPoint.y = max(max(p1.y, p2.y), p3.y);
    maxPoint.z = max(max(p1.z, p2.z), p3.z);

    a = p1_.x - p2_.x;
    b = p1_.y - p2_.y;
    c = p1_.z - p2_.z;

    d = p1_.x - p3_.x;
    e = p1_.y - p3_.y;
    f = p1_.z - p3_.z;

    bbox_ = BoundingBox(minPoint, maxPoint);

    normal_ = normal;
    material_ = nm;
}

BoundingBox Triangle::getBoundingBox() {
    return bbox_;
}

BoundingBox Plane::getBoundingBox() {
    return bbox_;
}

BoundingBox Sphere::getBoundingBox() {
    return bbox_;
}


bool Triangle::intersect(Ray& ray, Intersection& intersection, double minT, double maxT, int flag) {
    intersection.material_ = material_;

    if (!bbox_.intersect(ray, intersection, minT, maxT, flag)) {
        return false;
    }

    if (flag == BBOX_ONLY) {
        return true;
    }

    double j = p1_.x - ray.origin.x;
    double k = p1_.y - ray.origin.y;
    double l = p1_.z - ray.origin.z;

    double g = ray.direction.x;
    double h = ray.direction.y;
    double i = ray.direction.z;

    double M = a * (e * i - h * f) + b * (g * f - d * i) + c * (d * h - e * g);
    double t = -(f * (a * k - j * b) + e * (j * c - a * l) + d * (b * l - k * c)) / M;

    if (t < 0.0) {
        return false;
    }

    double gamma = (i * (a * k - j * b) + h * (j * c - a * l) + g * (b * l - k * c)) / M;
    if ((gamma < 0.0) || (gamma > 1.0)) {
        return false;
    }

    double beta = (j * (e * i - h * f) + k * (g * f - d * i) + l * (d * h - e * g)) / M;
    if ((beta < 0.0) || (beta > 1.0 - gamma)) {
        return false;
    }

    intersection.t_ = t;
    intersection.intersected_ = true;
    intersection.closestPoint_ = ray.origin + ray.direction * t;
    intersection.surfaceNormal_ = normal_;

    return true;
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
