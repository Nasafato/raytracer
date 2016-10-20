#include "camera.h"
#include <cmath>
#include <iostream>
#include <limits>

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

void Camera::calculateShading(double rgba[3], Ray ray, Intersection intersection, Material material, vector<Light *> lights, vector<Surface *> surfaces) {
    for (int i = 0; i < lights.size(); i++) {
        Vec3 lightVector = (lights[i]->position_ - intersection.closestPoint_).normalize();
        Vec3 normal = intersection.surfaceNormal_;
        Vec3 v = ray.direction.reverse().normalize();
        Vec3 h = (v + lightVector) / (v + lightVector).magnitude();

        double phong = material.r;
        double zero = 0.0;
        double attenuationFactor = 1.0 / pow((lights[i]->position_ - intersection.closestPoint_).magnitude(), 2.0);

        Intersection lightIntersection;
        Ray lightRay = Ray(intersection.closestPoint_ + lightVector * (double)0.05, lightVector);

        for (int s = 0; s < surfaces.size(); s++) {
            Intersection potentialIntersection = surfaces[s]->intersect(lightRay);
            if (potentialIntersection.intersected_) {
                Vec3 unNormalizedLightVector = (lights[i]->position_ - intersection.closestPoint_);
                double distance = std::sqrt(unNormalizedLightVector.dot(unNormalizedLightVector));
                if (potentialIntersection.t_ <= distance) {
                    lightIntersection = potentialIntersection;
                    break;
                }
            }
        }

        if (lightIntersection.intersected_ == false) {
            rgba[0] += (material.dr * max(normal.dot(lightVector), zero) + material.sr * pow(max(zero, normal.dot(h)), phong)) * lights[i]->r * attenuationFactor;
            rgba[1] += (material.dg * max(normal.dot(lightVector), zero) + material.sg * pow(max(zero, normal.dot(h)), phong)) * lights[i]->g * attenuationFactor;
            rgba[2] += (material.db * max(normal.dot(lightVector), zero) + material.sb * pow(max(zero, normal.dot(h)), phong)) * lights[i]->b * attenuationFactor;
        }
    }
}

Imf::Rgba Camera::calculatePixel(int x, int y, vector<Surface *> surfaces, vector<Light *> lights) {
    Ray ray = getRayForPixel(x, y);
    Imf::Rgba rgba = Imf::Rgba(0.0, 0.0, 0.0, 1.0);
    double minT = std::numeric_limits<double>::max();
    Intersection currentIntersection;
    Surface* closestSurface = NULL;

    for (int i = 0; i < surfaces.size(); i++) {
        Intersection intersection = surfaces[i]->intersect(ray);
        if (intersection.intersected_ && intersection.t_ < minT) {
            minT = intersection.t_;
            closestSurface = surfaces[i];
            currentIntersection = intersection;
        }
    }

    if (currentIntersection.intersected_) {
        double rgbaValues[] = {0.0, 0.0, 0.0};
        // closestSurface->getType();
        calculateShading(rgbaValues, ray, currentIntersection, closestSurface->material_, lights, surfaces);
        rgba.r = rgbaValues[0];
        rgba.g = rgbaValues[1];
        rgba.b = rgbaValues[2];
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

