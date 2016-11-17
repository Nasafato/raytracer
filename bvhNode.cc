#include "bvhNode.h"

using namespace std;

void BvhNode::getType() {
    cout << "bvhnode";
}
bool sortByX(Surface* s1, Surface* s2) { return s1->center_.x < s2->center_.x; }
bool sortByY(Surface* s1, Surface* s2) { return s1->center_.y < s2->center_.y; }
bool sortByZ(Surface* s1, Surface* s2) { return s1->center_.z < s2->center_.z; }

BoundingBox BvhNode::getBoundingBox() {
    return bbox_;
}

bool BvhNode::intersect(Ray& ray, Intersection& intersection, double minT, double maxT, int flag) {

    if (right_ == NULL) {
        if (left_->intersect(ray, intersection, minT, maxT, flag)) {
            return true;
        }
    } else if (bbox_.intersect(ray, intersection, minT, maxT, flag)) {
        Intersection leftIntersection, rightIntersection;
        bool leftHit = false, rightHit = false;
        if (left_) {
            leftHit = left_->intersect(ray, leftIntersection, minT, maxT, flag);
        }

        if (right_) {
            rightHit = right_->intersect(ray, rightIntersection, minT, maxT, flag);
        }

        if (!leftHit && !rightHit) {
            return false;
        } else if (leftHit && rightHit) {
            if (leftIntersection.t_ < rightIntersection.t_) {
                intersection = leftIntersection;
                return true;
            } else {
                intersection = rightIntersection;
                return true;
            }
        } else if (leftHit) {
            intersection = leftIntersection;
            return true;
        } else if (rightHit) {
            intersection = rightIntersection;
            return true;
        }
    }

    intersection.intersected_ = false;
    return false;
}


BvhNode::BvhNode() {
    left_ = NULL;
    right_ = NULL;
}

BvhNode::BvhNode(vector<Surface *>& surfaces, int left, int right, int axis) {
    if (left == right) {
        left_ = surfaces[left];
        right_ = NULL;
    } else if (left < right) {
        surround(surfaces, left, right);
        if (left == right - 1) {
            left_ = surfaces[left];
            right_ = surfaces[right];
        } else {
            int mid = left + (right - left) / 2;
            switch (axis) {
                case 0:
                    nth_element(surfaces.begin() + left, surfaces.begin() + mid,
                                surfaces.begin() + right + 1, sortByX);
                    break;

                case 1:
                    nth_element(surfaces.begin() + left, surfaces.begin() + mid,
                                surfaces.begin() + right + 1, sortByY);
                    break;

                case 2:
                    nth_element(surfaces.begin() + left, surfaces.begin() + mid,
                                surfaces.begin() + right + 1, sortByZ);
                    break;
            }
            left_ = new BvhNode(surfaces, left, mid, (axis + 1) % 3);
            right_ = new BvhNode(surfaces, mid + 1, right, (axis + 1) % 3);
        }
    }
}


void BvhNode::surround(const vector<Surface*> objs, int l, int r){
    double minX, minY, minZ;
    double maxX, maxY, maxZ;

    minX = minY = minZ = numeric_limits<double>::max();
    maxX = maxY = maxZ = numeric_limits<double>::min();
    for(int i = l; i <= r; ++i){
        minX = min(minX, objs[i]->bbox_.minPoint_.x);
        minY = min(minY, objs[i]->bbox_.minPoint_.y);
        minZ = min(minZ, objs[i]->bbox_.minPoint_.z);

        maxX = max(maxX, objs[i]->bbox_.maxPoint_.x);
        maxY = max(maxY, objs[i]->bbox_.maxPoint_.y);
        maxZ = max(maxZ, objs[i]->bbox_.maxPoint_.z);
    }

    bbox_ = BoundingBox(Point(minX - epsilon, minY - epsilon, minZ - epsilon), Point(maxX + epsilon, maxY + epsilon, maxZ + epsilon));
}