#ifndef LIGHT_H
#define LIGHT_H

#include "point.h"

class Light {

public:
    Vec3 rgb_;
    Point position_;
    virtual char getType() const = 0;
    virtual Vec3 getRgb() const = 0;
};

class PointLight: public Light {
public:
    PointLight();
    PointLight(Point, double, double, double);
    char getType() const;
    Vec3 getRgb() const;
};

class AmbientLight: public Light{
public:
    AmbientLight();
    AmbientLight(double, double, double);
    char getType() const;
    Vec3 getRgb() const;
};



#endif
