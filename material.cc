#include <cmath>
#include <limits>

#include "material.h"

using namespace std;

Material::Material()
{
    diffuse_ = specular_ = Vec3(0.5, 0.5, 0.5);
    idealSpecular_ = Vec3(0.0, 0.0, 0.0);
    r_ = 0.5;
}

Material::Material(double ndr, double ndg, double ndb, double nsr, double nsg, double nsb, double nir, double nig, double nib, double nr)
{
    diffuse_ = Vec3(ndr, ndg, ndb);
    specular_ = Vec3(nsr, nsg, nsb);
    idealSpecular_ = Vec3(nir, nig, nib);
    r_ = nr;
}

Vec3 Material::phongShading(const double& normalDotH) const {
    return specular_ * pow(max((const double)0.0, normalDotH), r_);
}

Vec3 Material::lambertianShading(const double& normalDotLight) const {
    return diffuse_ * max((const double)0.0, normalDotLight);
}

Vec3 Material::backShading (const double& normalDotLight, const Vec3& lightRgb) {
    if (normalDotLight > 0.0) {
        return Vec3(1.0, 1.0, 0.0) * lightRgb * normalDotLight;
    } else {
        return Vec3(0.0, 0.0, 0.0);
    }
}

