#include "ray.h"

Ray::Ray(Point newOrigin, Vec3 dir) {
    origin = newOrigin;
    direction = dir;
}