#include "camera.h"
#include <cmath>
#include <iostream>
#include <limits>

using namespace std;
using namespace Imf;

Camera::Camera() {
    eye = Point();
    direction = Vec3();
    u = Vec3();
    v = Vec3();
    w = Vec3();
    ambientLight_ = AmbientLight();

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
    ambientLight_ = AmbientLight();

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

void Camera::calculateShading(Rgba &rgba, Ray &ray, Intersection &intersection, Material &material, vector<Light *> &lights, vector<Surface *> &surfaces, double minT, double maxT) {

    for (int i = 0; i < lights.size(); i++) {
        Vec3 lightVector = (lights[i]->position_ - intersection.closestPoint_).normalize();
        Vec3 normal = intersection.surfaceNormal_;
        Vec3 v = ray.direction.reverse().normalize();
        Vec3 h = (v + lightVector) / (v + lightVector).magnitude();

        double phong = material.r;
        double zero = 0.0;
        double attenuationFactor = 1.0 / pow((lights[i]->position_ - intersection.closestPoint_).magnitude(), 2.0);
        double dr, dg, db;
        double sr, sg, sb;
        dr = material.dr;
        dg = material.dg;
        db = material.db;
        sr = material.sr;
        sg = material.sg;
        sb = material.sb;

        Intersection lightIntersection;
        Ray lightRay = Ray(intersection.closestPoint_ + lightVector * (double)0.05, lightVector);

        // See if the shadow ray intersects an object
        for (int s = 0; s < surfaces.size(); s++) {
            Intersection potentialIntersection = surfaces[s]->intersect(lightRay, minT, maxT);
            if (potentialIntersection.intersected_) {
                Vec3 unNormalizedLightVector = (lights[i]->position_ - intersection.closestPoint_);
                double distance = std::sqrt(unNormalizedLightVector.dot(unNormalizedLightVector));
                if (potentialIntersection.t_ <= distance) {
                    lightIntersection = potentialIntersection;
                    break;
                }
            }
        }

        double normalDotLight = normal.dot(lightVector);
        if (normalDotLight < 0.0) {
            dr = 1.0;
            dg = 1.0;
            db = 0.0;
            sr = 0.0;
            sg = 0.0;
            sb = 0.0;
            normalDotLight *= -1.0;
        }

        if (lightIntersection.intersected_ == false) {
            rgba.r += (dr * max(normalDotLight, zero) + sr * pow(max(zero, normal.dot(h)), phong)) * lights[i]->r * attenuationFactor;
            rgba.g += (dg * max(normalDotLight, zero) + sg * pow(max(zero, normal.dot(h)), phong)) * lights[i]->g * attenuationFactor;
            rgba.b += (db * max(normalDotLight, zero) + sb * pow(max(zero, normal.dot(h)), phong)) * lights[i]->b * attenuationFactor;
        }
    }
}

Imf::Rgba Camera::calculatePixel(Ray &ray, vector<Surface *> &surfaces, vector<Light *> &lights, double minT, double maxT, int recurseLimit) {
    Rgba rgba = Rgba(0.0, 0.0, 0.0, 1.0);

    if (recurseLimit == 0) {
        return rgba;
    }

    Intersection currentIntersection;
    Surface* closestSurface = NULL;
    double currentMinT = maxT;

    for (int i = 0; i < surfaces.size(); i++) {
        Intersection intersection = surfaces[i]->intersect(ray, minT, maxT);
        if (intersection.intersected_ && intersection.t_ < currentMinT) {
            currentMinT = intersection.t_;
            closestSurface = surfaces[i];
            currentIntersection = intersection;
        }
    }

    if (!currentIntersection.intersected_) {
        return rgba;
    }
    double ar, ag, ab;
    ar = ambientLight_.r_;
    ag = ambientLight_.g_;
    ab = ambientLight_.b_;

    rgba.r += ar * closestSurface->material_.dr;
    rgba.g += ag * closestSurface->material_.dg;
    rgba.b += ab * closestSurface->material_.db;

    calculateShading(rgba, ray, currentIntersection, closestSurface->material_, lights, surfaces, minT, maxT);
    if (closestSurface->material_.ir == 0.0 && closestSurface->material_.ig == 0.0 && closestSurface->material_.ib == 0.0) {
        return rgba;
    }

    Vec3 refRayDir = ray.direction - (currentIntersection.surfaceNormal_ * (ray.direction.dot(currentIntersection.surfaceNormal_)) * 2);
    Ray reflectedRay = Ray(currentIntersection.closestPoint_ + refRayDir * 0.05, refRayDir);
    Rgba refRgba = calculatePixel(reflectedRay, surfaces, lights, minT, maxT, recurseLimit - 1);
    rgba.r += closestSurface->material_.ir * refRgba.r;
    rgba.g += closestSurface->material_.ig * refRgba.g;
    rgba.b += closestSurface->material_.ib * refRgba.b;

    return rgba;
}

void Camera::writeRgba (const char fileName[], const Imf::Rgba *pixels, int width, int height) {
    Imf::RgbaOutputFile file (fileName, width, height, Imf::WRITE_RGBA);
    file.setFrameBuffer (pixels, 1, width);
    file.writePixels (height);
}

void Camera::writeScene(const char filename[], vector<Surface *> &surfaces, vector<Light *> &lights) {
    Imf::Array2D<Imf::Rgba> pixels;
    pixels.resizeErase(heightPixels, widthPixels);

    for (int y = 0; y < heightPixels; y++) {
        for (int x = 0; x < widthPixels; x++) {
            Ray ray = getRayForPixel(x, y);
            int recurseLimit = 20;
            double minT = 0.001;
            double maxT = std::numeric_limits<double>::max();;
            pixels[y][x] = calculatePixel(ray, surfaces, lights, minT, maxT, recurseLimit);
        }
    }
    writeRgba(filename, &pixels[0][0], widthPixels, heightPixels);
}

