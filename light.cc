#include "light.h"

PointLight::PointLight() {
    position_ = Point(0.0, 0.0, 0.0);
    r = 0.0;
    g = 0.0;
    b = 0.0;
}

PointLight::PointLight(Point pos, double nr, double ng, double nb) {
    position_ = pos;
    r = nr;
    g = ng;
    b = nb;
}

AmbientLight::AmbientLight() {
    r_ = 0.0;
    g_ = 0.0;
    b_ = 0.0;
}

AmbientLight::AmbientLight(double r, double g, double b) {
    r_ = r;
    g_ = g;
    b_ = b;
}