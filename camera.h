#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "point.h"
#include "ray.h"

class Camera {
public:
    Point eye;

    Vec3 direction;
    Vec3 u;
    Vec3 v;
    Vec3 w;

    float l, r, t, b;

    float focalLength;

    float imageWidth;
    float imageHeight;
    int widthPixels;
    int heightPixels;

    Camera();
    Camera(Point, Vec3, float, float, float, int, int);
    Ray getRayForPixel(int, int);
};


#endif