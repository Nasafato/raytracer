#ifndef BVHNODE_H
#define BVHNODE_H
#include "surfaces.h"
#include "constants.h"
#include <vector>
#include <cmath>
#include <algorithm>


class BvhNode: public Surface {

public:
    bool intersect(Ray&, Intersection&, double, double, int);
    BvhNode();
    BvhNode(std::vector<Surface *>, int);
    Surface *left_;
    Surface *right_;
    BoundingBox bbox_;
    BoundingBox getBoundingBox();
    // void createNode(std::vector<Surface *>, int);
    void getType();

    BvhNode(std::vector<Surface*> &objects, int l, int r, int axis);

    void surround(const std::vector<Surface*> objects, int l, int r);

    bool intersect(const Ray &r, Intersection &intersection, int flag);

    bool shadowTest(const Ray &r, Intersection &intersection, int flag);
};

#endif /* BVH_h */
