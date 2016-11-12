#include <iostream>
#include <cmath>
#include <algorithm>
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

Intersection BoundingBox::intersect(Ray ray, double minT, double maxT, int flag) {
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

    Intersection intersection = Intersection(false, 0.0, 0.0, Point(0.0, 0.0, 0.0));

    if ((t_xmin > t_ymax) || (t_xmin > t_zmax)
        || (t_ymin > t_xmax) || (t_ymin > t_zmax)
        || (t_zmin > t_xmax) || (t_zmin > t_ymax)) {
        return intersection;
    } else {
        // cout << t_xmin << ", " << t_ymin << ", " << t_zmin << endl;
        t_xmin = max((double)0.0, t_xmin);
        t_ymin = max((double)0.0, t_ymin);
        t_zmin = max((double)0.0, t_zmin);
        intersection.t_ = max(max(t_xmin, t_ymin), t_zmin);
        // cout << "min is " << intersection.t_ << endl;
        if (intersection.t_ <= 0.0) {
            return intersection;
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
        // cout << "Intersection is " << t << endl;
        // cout << "Normal is " << normal << endl;
        return intersection;
    }
}




Sphere::Sphere(Point ncenter, double nradius, Material* nm) {
    center_ = ncenter;
    radius = nradius;

    Vec3 minVec = Vec3(-nradius, -nradius, -nradius);
    Vec3 maxVec = Vec3(nradius, nradius, nradius);

    Point minPoint = ncenter + minVec;
    Point maxPoint = ncenter + maxVec;

    boundingBox_ = BoundingBox(minPoint, maxPoint);
    material_ = nm;
}


Intersection Sphere::intersect(Ray ray, double minT, double maxT, int flag) {

    Intersection intersection = Intersection(false, 0.0, 0.0, Point(0.0, 0.0, 0.0));



    Intersection bboxIntersection = boundingBox_.intersect(ray, minT, maxT, flag);
    if (!bboxIntersection.intersected_) {
        return intersection;
    }

    if (flag == 1) {
        return bboxIntersection;
    }

    if (flag == -1) {
        // use BVH
    }

    Vec3 e_c = ray.origin - center_;
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
        intersection.surfaceNormal_ = (closestPoint - center_) / radius;
        if (intersection.surfaceNormal_.magnitude() > 1.0) {
            intersection.surfaceNormal_.normalize();
        }
    }

    return intersection;
}

Plane::Plane(Vec3 normal, double d, Material *nm) {
    normal_ = normal;
    d_ = d;
    material_ = nm;
}


Intersection Plane::intersect(Ray ray, double minT, double maxT, int flag) {
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

    boundingBox_ = BoundingBox(minPoint, maxPoint);

    normal_ = normal;
    material_ = nm;
}

Intersection Triangle::intersect(Ray ray, double minT, double maxT, int flag) {

    Intersection intersection = Intersection(false, 0.0, 0.0, Point(0.0, 0.0, 0.0));
    Intersection bboxIntersection = boundingBox_.intersect(ray, minT, maxT, flag);



    if (!bboxIntersection.intersected_) {
        return intersection;
    }

    if (flag == 1) {
        return bboxIntersection;
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

// BvhNode::BvhNode() {
//     left_ = NULL;
//     right_ = NULL;

// }

// BoundingBox BvhNode::combine(BoundingBox& b1, BoundingBox& b2) {


// }


// bool sortByX(Surface* s1, Surface* s2) { return s1->center_.x < s2->center_.x; }
// bool sortByY(Surface* s1, Surface* s2) { return s1->center_.y < s2->center_.y; }
// bool sortByZ(Surface* s1, Surface* s2) { return s1->center_.z < s2->center_.z; }

// void BvhNode::createNode(vector<Surface *> surfaces, int axis) {
//     int n = surfaces.size();
//     if (n == 1) {
//         left_ = surfaces[0];
//         right_ = NULL;
//     } else if (n == 2) {
//         left_ = surfaces[0];
//         right_ = surfaces[1];
//     } else {
//         if (axis % 3 == 0) {
//             sort(surfaces.begin(), surfaces.end(), sortByZ);
//         } else if (axis % 3 == 1) {
//             sort(surfaces.begin(), surfaces.end(), sortByY);
//         } else {
//             sort(surfaces.begin(), surfaces.end(), sortByX);
//         }
//         left_ = new BvhNode(vector<Surface *>(surfaces.begin(), surfaces.begin() + (n/2 - 1)), axis + 1 % 3);
//         right_ = new BvhNode(vector<Surface *>(surfaces.begin() + (n/2), surfaces.end()), axis + 1 % 3);
//         // right_ = new BvhNode(surfaces[n/2:], axis + 1 % 3);
//         // box
//     }
// }

void Plane::getType() {
    std::cout << "plane";
}

void Sphere::getType() {
    std::cout << "sphere";
}

void Triangle::getType() {
    std::cout << "triangle";
}