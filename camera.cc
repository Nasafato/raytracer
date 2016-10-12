#include "camera.h"
#include "intersection.h"
#include <iostream>

using namespace std;

Camera::Camera() {
    eye = Point();
    direction = Vec3();
    u = Vec3();
    v = Vec3();
    w = Vec3();

    focalLength = 0.0;

    imageWidth = 0.0;
    imageHeight = 0.0;

    widthPixels = 0.0;
    heightPixels = 0.0;

    l = 0;
    r = 0;
    t = 0;
    b = 0;
}

Camera::Camera(Point pos, Vec3 dir, double d, double iw, double ih, int wp, int hp) {
    eye = pos;
    w = dir.reverse();
    w.normalize();
    direction = dir;

    if (1.0 - fabs(w.y) < 0.0001) {
        u = Vec3(1.0, 0.0, 0.0);
        v = w.cross(u);
    } else {
        Vec3 up = Vec3(0.0, 1.0, 0.0);
        u = up.cross(w);
        v = w.cross(u);
    }

    u.normalize();
    v.normalize();

    focalLength = d;
    imageWidth = iw;
    imageHeight = ih;

    l = -(iw / 2.0);
    r = iw / 2.0;
    t = ih / 2.0;
    b = -(ih / 2.0);

    // std::cout << "l: " << l << ", r: " << r << ", t: " << t << ", b: " << b << std::endl;

    widthPixels = wp;
    heightPixels = hp;
}

Ray Camera::getRayForPixel(int x, int y) {
    double uScalar = l + (r - l) * (x + 0.5) / widthPixels;
    double vScalar = t + (b - t) * (y + 0.5) / heightPixels;
    Vec3 dir = w * (-focalLength) + (u * uScalar) + (v * vScalar);
    dir.normalize();

    return Ray(eye, dir);
}

Imf::Rgba Camera::calculatePixel(int x, int y, vector<Surface *> surfaces, vector<Light *> lights) {
    Ray ray = getRayForPixel(x, y);
    Imf::Rgba rgba = Imf::Rgba(0.0, 0.0, 0.0, 1.0);
    double minT = std::numeric_limits<double>::max();
    bool intersected = false;
    Surface* closestSurface = NULL;

    Material material;
    for (int i = 0; i < surfaces.size(); i++) {
        Intersection intersection = surfaces[i]->intersect(ray);
        // std::cout << "Discriminant is " << discriminant << std::endl;
        if (intersection.intersected_) {
            intersected = true;
            if (intersection.t_ < minT) {
                minT = intersection.t_;
                material = surfaces[i]->material;
            }
        }
    }

    if (intersected) {
        // Vec3 surfaceNormal = closestSurface->getSurfaceNormal(ray, minT);
        // rgba.r = closestSurface->material.dr;
        // rgba.g = closestSurface->material.dg;
        // rgba.b = closestSurface->material.db;
        rgba.r = material.dr;
        rgba.g = material.dg;
        rgba.b = material.db;

    }

    return rgba;
}

void Camera::writeRgba (const char fileName[], const Imf::Rgba *pixels, int width, int height) {
    Imf::RgbaOutputFile file (fileName, width, height, Imf::WRITE_RGBA);
    file.setFrameBuffer (pixels, 1, width);
    file.writePixels (height);
}

void Camera::writeScene(const char filename[], vector<Surface *> surfaces, vector<Light *> lights) {
    Imf::Array2D<Imf::Rgba> pixels;
    pixels.resizeErase(heightPixels, widthPixels);

    for (int y = 0; y < heightPixels; y++) {
        for (int x = 0; x < widthPixels; x++) {
            pixels[y][x] = calculatePixel(x, y, surfaces, lights);
        }
    }
    writeRgba(filename, &pixels[0][0], widthPixels, heightPixels);
}

