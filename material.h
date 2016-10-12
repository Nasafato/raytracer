#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>

class Material {
public:
    double dr, dg, db;
    double sr, sg, sb;
    double ir, ig, ib;
    double r;

    friend std::ostream &operator<<(std::ostream &os, Material &m) {
        return os<<"<"<<m.dr<<","<<m.dg<<","<<m.db<<","<<m.sr<<","<<m.sg<<","<<m.sb<<","<<m.ir<<","<<m.ig<<","<<m.ib<<","<<m.r<<">";
    }


    Material();
    Material(double, double, double, double, double, double, double, double, double, double);
};

#endif