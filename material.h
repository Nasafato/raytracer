#ifndef MATERIAL_H
#define MATERIAL_H

class Material {
public:
    float dr, dg, db;
    float sr, sg, sb;
    float ir, ig, ib;
    float r;

    Material();
    Material(float, float, float, float, float, float, float, float, float, float);
};

#endif