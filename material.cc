#include "material.h"

Material::Material()
{
    dr = 0.5;
    dg = 0.5;
    db = 0.5;
    sr = 0.5;
    sg = 0.5;
    sb = 0.5;
    ir = 0.5;
    ig = 0.5;
    ib = 0.5;
    r = 0.5;
}

Material::Material(double ndr, double ndg, double ndb, double nsr, double nsg, double nsb, double nir, double nig, double nib, double nr)
{
    dr = ndr;
    dg = ndg;
    db = ndb;
    sr = nsr;
    sg = nsg;
    sb = nsb;
    ir = nir;
    ig = nig;
    ib = nib;
    r = nr;
}
