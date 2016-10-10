#include "camera.h"
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

Camera::Camera(Point pos, Vec3 dir, float d, float iw, float ih, int wp, int hp) {
    eye = pos;

    direction = dir;

    u = direction.cross(Vec3(0.0, 1.0, 0.0));
    u.normalize();

    v = u.cross(direction);
    v.normalize();

    w = direction * -1.0;
    w.normalize();

    focalLength = d;
    imageWidth = iw;
    imageHeight = ih;

    l = -(iw / 2);
    r = iw / 2;
    t = ih / 2;
    b = -(ih / 2);

    std::cout << "l: " << l << ", r: " << r << ", t: " << t << ", b: " << b << std::endl;

    widthPixels = wp;
    heightPixels = hp;
}

Ray Camera::getRayForPixel(int x, int y) {
    float uScalar = l + (r - l) * (x + 0.5) / widthPixels;
    float vScalar = b + (t - b) * (y + 0.5) / heightPixels;
    Vec3 dir = w * (-focalLength) + u * uScalar + v * vScalar;
    dir.normalize();

    return Ray(eye, dir);
}

Imf::Rgba Camera::calculatePixel(int x, int y, vector<Surface *> surfaces) {
    Ray ray = getRayForPixel(x, y);
    Imf::Rgba rgba = Imf::Rgba(0.0, 0.0, 0.0, 1.0);
    float minT = std::numeric_limits<float>::max();
    float testT;
    bool intersected = false;
    Surface* closestSurface = surfaces[0];

    for (int i = 0; i < surfaces.size(); i++) {
        testT = surfaces[i]->intersect(ray);
        if (testT < minT & testT != -1.0) {
            minT = testT;
            closestSurface = surfaces[i];
            intersected = true;
        }
    }

    if (intersected) {
        rgba.r = closestSurface->material.dr;
        rgba.g = closestSurface->material.dg;
        rgba.b = closestSurface->material.db;
    }

    return rgba;
}

void Camera::writeRgba (const char fileName[], const Imf::Rgba *pixels, int width, int height) {
    Imf::RgbaOutputFile file (fileName, width, height, Imf::WRITE_RGBA);
    file.setFrameBuffer (pixels, 1, width);
    file.writePixels (height);
}

void Camera::writeScene(const char filename[], vector<Surface *> surfaces) {
    Imf::Array2D<Imf::Rgba> pixels;
    pixels.resizeErase(heightPixels, widthPixels);

    for (int y = 0; y < heightPixels; y++) {
        for (int x = 0; x < widthPixels; x++) {
            pixels[heightPixels - y - 1][x] = calculatePixel(x, y, surfaces);
        }
    }
    writeRgba(filename, &pixels[0][0], widthPixels, heightPixels);
}

