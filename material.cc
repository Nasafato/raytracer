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

Vec3 Material::phongShading(const double normalDotH, const double phong) const {
    return specular_ * pow(max((const double)0.0, normalDotH), phong);
}

Vec3 Material::lambertianShading(const double normalDotLight) const {
    return diffuse_ * max((const double)0.0, normalDotLight);
}

// static Vec3 backShading (
//                     const Vector &L,    // unit vector to the light
//                     const Vector &e,    // unit vector to the viewer
//                     const Vector &N,    // unit surface normal
//                     const Vector &L_e) {


// }

