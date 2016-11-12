#include "light.h"

PointLight::PointLight() {
    position_ = Point(0.0, 0.0, 0.0);
    rgb_ = Vec3(0.0, 0.0, 0.0);
}

PointLight::PointLight(Point pos, double nr, double ng, double nb) {
    position_ = pos;
    rgb_ = Vec3(nr, ng, nb);
}

Vec3 PointLight::getRgb() const {
    return rgb_;
}

char PointLight::getType() const {
    return 'p';
}

AmbientLight::AmbientLight() {
    rgb_ = Vec3(0.0, 0.0, 0.0);
}

AmbientLight::AmbientLight(double r, double g, double b) {
    rgb_ = Vec3(r, g, b);
}

Vec3 AmbientLight::getRgb() const {
    return rgb_;
}

char AmbientLight::getType() const {
    return 'a';
}