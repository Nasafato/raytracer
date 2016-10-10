#include "camera.h"
#include <iostream>

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
