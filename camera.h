#ifndef CAMERA_H
#define CAMERA_H

#include <ImfRgbaFile.h>
#include <ImfStringAttribute.h>
#include <ImfMatrixAttribute.h>
#include <ImfArray.h>
#include <vector>
#include "surfaces.h"
#include "vec3.h"
#include "point.h"
#include "ray.h"
#include "light.h"
#include "intersection.h"

class Camera {
public:
    Point eye;

    Vec3 direction;
    Vec3 u;
    Vec3 v;
    Vec3 w;

    double l, r, t, b;

    double focalLength;

    double imageWidth;
    double imageHeight;
    int widthPixels;
    int heightPixels;

    Camera();
    Camera(Point, Vec3, double, double, double, int, int);
    void writeScene(const char[], std::vector<Surface *>, std::vector<Light *>);

private:
    Ray getRayForPixel(int, int);
    Imf::Rgba calculatePixel(int, int, std::vector<Surface *>, std::vector<Light *>);
    void calculateShading(double[3], Ray, Intersection, Material, std::vector<Light *>, std::vector<Surface *>);
    void writeRgba(const char[], const Imf::Rgba *, int, int);

};


#endif