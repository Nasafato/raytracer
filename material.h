#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include "vec3.h"
#include "material.h"

class Material {
public:
    Vec3 diffuse_;
    Vec3 specular_;
    Vec3 idealSpecular_;
    double r_;

    // friend std::ostream &operator<<(std::ostream &os, Material &m) {
    //     return os<<"<"<<m.dr<<","<<m.dg<<","<<m.db<<","<<m.sr<<","<<m.sg<<","<<m.sb<<","<<m.ir<<","<<m.ig<<","<<m.ib<<","<<m.r<<">";
    // }

    Material();
    Material(double, double, double, double, double, double, double, double, double, double);

    Vec3 phongShading(const double& normalDotH) const;
    Vec3 lambertianShading(const double& normalDotLight) const;
    static Vec3 backShading (const double& normalDotLight, const Vec3& lightRgb);

};

#endif