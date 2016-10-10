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

Material::Material(float ndr, float ndg, float ndb, float nsr, float nsg, float nsb, float nir, float nig, float nib, float nr)
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
