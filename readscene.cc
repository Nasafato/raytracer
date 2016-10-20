#include <cmath>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <limits>
#include <ImfRgbaFile.h>
#include <ImfStringAttribute.h>
#include <ImfMatrixAttribute.h>
#include <ImfArray.h>
#include "structures.h"

using namespace std;

#define IM_DEBUGGING

double getTokenAsdouble (string inString, int whichToken)
{

    double thisdoubleVal = 0.;    // the return value

    if (whichToken == 0) {
        cerr << "error: the first token on a line is a character!" << endl;
        exit (-1);
    }

    // c++ string class has no super-easy way to tokenize, let's use c's:
    char *cstr = new char [inString.size () + 1];

    strcpy (cstr, inString.c_str());

    char *p = strtok (cstr, " ");
    if (p == 0) {
        cerr << "error: the line has nothing on it!" << endl;
        exit (-1);
    }

    for (int i = 0; i < whichToken; i++) {
        p = strtok (0, " ");
        if (p == 0) {
            cerr << "error: the line is not long enough for your token request!" << endl;
            exit (-1);
        }
    }

    thisdoubleVal = atof (p);

    delete[] cstr;

    return thisdoubleVal;
}


// read the scene file.
Camera parseSceneFile (char *filename, std::vector<Surface *>& surfaces, std::vector<Light *>& lights)
{
    Camera camera;
    Material* currentMaterial = new Material();

    ifstream inFile(filename);    // open the file
    string line;

    if (! inFile.is_open ()) {  // if it's not open, error out.
        cerr << "can't open scene file" << endl;
        exit (-1);
    }

    while (! inFile.eof ()) {   // go through every line in the file until finished

        getline (inFile, line); // get the line

        switch (line[0])  {     // we'll decide which command based on the first character

            case 's': {
                // it's a sphere, load in the parameters

                double x, y, z, r;
                x = getTokenAsdouble (line, 1);
                y = getTokenAsdouble (line, 2);
                z = getTokenAsdouble (line, 3);
                r = getTokenAsdouble (line, 4);

                Point center = Point(x, y, z);
                Sphere *sphere = new Sphere(center, r, currentMaterial);
                surfaces.push_back(sphere);

#ifdef IM_DEBUGGING
                cout << "got a sphere with ";
                cout << "parameters: " << x << " " << y << " " << z << " " << r << endl;
#endif
                break;
            }

            case 't': {   // x1 y1 z1 x2 y2 z2 x3 y3 z3
                double x1, y1, z1;
                double x2, y2, z2;
                double x3, y3, z3;
                x1 = getTokenAsdouble(line, 1);
                y1 = getTokenAsdouble(line, 2);
                z1 = getTokenAsdouble(line, 3);
                x2 = getTokenAsdouble(line, 4);
                y2 = getTokenAsdouble(line, 5);
                z2 = getTokenAsdouble(line, 6);
                x3 = getTokenAsdouble(line, 7);
                y3 = getTokenAsdouble(line, 8);
                z3 = getTokenAsdouble(line, 9);

                Point p1 = Point(x1, y1, z1);
                Point p2 = Point(x2, y2, z2);
                Point p3 = Point(x3, y3, z3);

                Vec3 u = p1 - p2;
                Vec3 v = p3 - p1;
                Vec3 normal = u.cross(v);
                normal.normalize();

                Triangle *triangle = new Triangle(p1, p2, p3, normal, currentMaterial);
                // surfaces.push_back(triangle);
                break;

            }
            case 'p': {  // plane
                double x, y, z, d;
                x = getTokenAsdouble (line, 1);
                y = getTokenAsdouble (line, 2);
                z = getTokenAsdouble (line, 3);
                d = getTokenAsdouble (line, 4);

                Vec3 normal = Vec3(x, y, z);
                Plane *plane = new Plane(normal, d, currentMaterial);
                surfaces.push_back(plane);

#ifdef IM_DEBUGGING
                cout << "got a plane with ";
                cout << "parameters: " << x << " " << y << " " << z << " " << d << endl;
#endif

                break;
            }

            // camera:
            case 'c': { // camera
                double x, y, z, vx, vy, vz, d, iw, ih, pw, ph;
                x = getTokenAsdouble (line, 1);
                y = getTokenAsdouble (line, 2);
                z = getTokenAsdouble (line, 3);

                vx = getTokenAsdouble (line, 4);
                vy = getTokenAsdouble (line, 5);
                vz = getTokenAsdouble (line, 6);

                d = getTokenAsdouble (line, 7);

                iw = getTokenAsdouble (line, 8);
                ih = getTokenAsdouble (line, 9);

                pw = getTokenAsdouble (line, 10);
                ph = getTokenAsdouble (line, 11);

                Point position = Point(x, y, z);
                Vec3 direction = Vec3(vx, vy, vz);

                camera = Camera(position, direction, d, iw, ih, pw, ph);

#ifdef IM_DEBUGGING
                cout << "got a camera with ";
                cout << "parameters: " << x << ", " << ph << endl;
#endif
                break;
            }

            // lights:
            case 'l':   // light

                // slightly different from the rest, we need to examine the second param,
                // which is at the third position on the line:
                switch (line[2]) {
                    case 'p': { // point light
                        double x, y, z, r, g, b;
                        x = getTokenAsdouble(line, 2);
                        y = getTokenAsdouble(line, 3);
                        z = getTokenAsdouble(line, 4);
                        r = getTokenAsdouble(line, 5);
                        g = getTokenAsdouble(line, 6);
                        b = getTokenAsdouble(line, 7);
                        // cout << "x: " << x << ", y: " << y << ", z: " << z << ", r: " << r << ", g: " << g << ", b: " << b << endl;
                        PointLight *light = new PointLight(Point(x, y, z), r, g, b);
                        lights.push_back(light);
                    }
                    case 'd':   // directional light
                        break;
                    case 'a':   // ambient light
                        break;
                }
                break;

            // materials:
            case 'm': {// material
                double ndr, ndg, ndb, nsr, nsg, nsb, nir, nig, nib, nr;
                ndr = getTokenAsdouble (line, 1);
                ndg = getTokenAsdouble (line, 2);
                ndb = getTokenAsdouble (line, 3);

                nsr = getTokenAsdouble (line, 4);
                nsg = getTokenAsdouble (line, 5);
                nsb = getTokenAsdouble (line, 6);

                nr = getTokenAsdouble (line, 7);

                nir = getTokenAsdouble (line, 8);
                nig = getTokenAsdouble (line, 9);
                nib = getTokenAsdouble (line, 10);

            #ifdef IM_DEBUGGING
                cout << "got a material with ";
                cout << "parameters: " << ndr << ", " << ndg << ", " << ndb << endl;
            #endif

                delete currentMaterial;
                currentMaterial = new Material(ndr, ndg, ndb, nsr, nsg, nsb, nir, nig, nib, nr);
                break;
            }

            case '/':
                // don't do anything, it's a comment
                break;
            //
            // options
            //
            case 'o':   // make your own options if you wish
                break;
        }
    }

    delete currentMaterial;
    currentMaterial = NULL;
    return camera;
}


int main (int argc, char *argv[])
{

    if (argc != 3) {
        cerr << "useage: raytra scenefilename" << endl;
        return -1;
    }

    vector<Surface *> surfaces;
    vector<Light *> lights;
    Camera camera = parseSceneFile (argv[1], surfaces, lights);
    camera.writeScene(argv[2], surfaces, lights);

    for (int i = 0; i < surfaces.size(); i++) {
        delete surfaces[i];
        surfaces[i] = NULL;
    }

    return 0;
}
