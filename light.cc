#include "light.h"

PointLight::PointLight() {
    position = Point(0.0, 0.0, 0.0);
    r = 0.0;
    g = 0.0;
    b = 0.0;
}

PointLight::PointLight(Point pos, float nr, float ng, float nb) {
    position = pos;
    r = nr;
    g = ng;
    b = nb;
}