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

void Camera::calculateShading(Vec3 &rgba, Ray &ray, BvhNode* bvhRoot, Intersection &intersection, Material* material, vector<Light *> &lights, vector<Surface *> &surfaces, double minT, double maxT, bool flipped, int flag) {

    for (auto light: lights) {
        if (light->getType() != 'p') {
            continue;
        }

        Vec3 lightVector = (light->position_ - intersection.closestPoint_).normalize();
        Vec3 normal = flipped ? (intersection.surfaceNormal_ * -1.0) : intersection.surfaceNormal_;
        Vec3 v = ray.direction.reverse().normalize();
        Vec3 h = (v + lightVector) / (v + lightVector).magnitude();

        Intersection lightIntersection;
        Ray lightRay = Ray(intersection.closestPoint_ + lightVector * (double)0.001, lightVector);

        // See if the shadow ray intersects an object
        if (flag == -1) {
            lightIntersection = bvhRoot->intersect(lightRay, minT, maxT, flag);
        } else {
            for (auto surface: surfaces) {
                Intersection potentialIntersection = surface->intersect(lightRay, minT, maxT, flag);
                if (potentialIntersection.intersected_) {
                    Vec3 unNormalizedLightVector = (light->position_ - intersection.closestPoint_);
                    double distance = std::sqrt(unNormalizedLightVector.dot(unNormalizedLightVector));
                    if (potentialIntersection.t_ <= distance) {
                        lightIntersection = potentialIntersection;
                        break;
                    }
                }
            }
        }

        if (lightIntersection.intersected_ == true) {
            return;
        }

        double normalDotLight = normal.dot(lightVector);
        double attenuationFactor = 1.0 / pow((light->position_ - intersection.closestPoint_).magnitude(), 2.0);
        double normalDotH = normal.dot(h);

        if (flipped) {
            rgba += Material::backShading(normalDotLight, light->getRgb()) * attenuationFactor;
        } else {
            Vec3 colors = (material->lambertianShading(normalDotLight) + material->phongShading(normalDotH)) *
                    (light->getRgb() *attenuationFactor);
            rgba += colors;
        }
    }
}

Vec3 Camera::calculatePixel(Ray &ray, int rayType, BvhNode* bvhRoot, vector<Surface *> &surfaces,
                            vector<Light *> &lights, double minT, double maxT,
                            int recurseLimit, int flag) {
    Vec3 rgba = Vec3(0.0, 0.0, 0.0);
    if (recurseLimit == 0) {
        return rgba;
    }

    Intersection currentIntersection;
    Material* material = NULL;
    double currentMinT = maxT;

    if (flag == -1) {
        currentIntersection = bvhRoot->intersect(ray, minT, maxT, flag);
        material = currentIntersection.material_;
    } else {
        for (auto surface: surfaces) {
            Intersection intersection = surface->intersect(ray, minT, maxT, flag);
            if (intersection.intersected_ && intersection.t_ < currentMinT) {
                currentMinT = intersection.t_;
                material = surface->material_;
                currentIntersection = intersection;
            }
        }
    }

    if (!currentIntersection.intersected_) {
        return rgba;
    }

    // add ambient stuff

    bool flipped = false;
    if (-1.0 * ray.direction.dot(currentIntersection.surfaceNormal_) < 0.0) {
        flipped = true;
    }

    if (rayType == 1) {
        if (flipped) {
            rgba += ambientLight_.rgb_ * Vec3(1.0, 1.0, 0.0);
        } else {
            rgba += ambientLight_.rgb_ * material->diffuse_;
        }
    }

    calculateShading(rgba, ray, bvhRoot, currentIntersection, material, lights, surfaces, minT, maxT, flipped, flag);
    if (material->idealSpecular_ == Vec3(0.0, 0.0, 0.0) || flipped) {
        return rgba;
    }

    Vec3 refRayDir = ray.direction - (currentIntersection.surfaceNormal_ * (ray.direction.dot(currentIntersection.surfaceNormal_)) * 2);
    Ray reflectedRay = Ray(currentIntersection.closestPoint_ + refRayDir * 0.05, refRayDir);
    Vec3 refRgba = calculatePixel(reflectedRay, 2, bvhRoot, surfaces, lights, minT, maxT, recurseLimit - 1, flag);

    rgba += (material->idealSpecular_ * refRgba);

    return rgba;
}

void Camera::writeRgba (const char fileName[], const Imf::Rgba *pixels, int width, int height) {
    Imf::RgbaOutputFile file (fileName, width, height, Imf::WRITE_RGBA);
    file.setFrameBuffer (pixels, 1, width);
    file.writePixels (height);
}

void Camera::writeScene(const char filename[], BvhNode* bvhRoot, vector<Surface *> &surfaces, vector<Light *> &lights, int flag) {
    Imf::Array2D<Imf::Rgba> pixels;
    pixels.resizeErase(heightPixels, widthPixels);

    for (int y = 0; y < heightPixels; y++) {
        for (int x = 0; x < widthPixels; x++) {
            Ray ray = getRayForPixel(x, y);
            int recurseLimit = 5;
            double minT = 0.001;
            double maxT = std::numeric_limits<double>::max();;
            Vec3 colors = calculatePixel(ray, 1, bvhRoot, surfaces, lights, minT, maxT, recurseLimit, flag);
            pixels[y][x] = Rgba(colors.x, colors.y, colors.z, 1.0);
        }
    }
    writeRgba(filename, &pixels[0][0], widthPixels, heightPixels);
}

