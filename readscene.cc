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

float getTokenAsFloat (string inString, int whichToken)
{

    float thisFloatVal = 0.;    // the return value

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

    thisFloatVal = atof (p);

    delete[] cstr;

    return thisFloatVal;
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

                float x, y, z, r;
                x = getTokenAsFloat (line, 1);
                y = getTokenAsFloat (line, 2);
                z = getTokenAsFloat (line, 3);
                r = getTokenAsFloat (line, 4);

                Point center = Point(x, y, z);
                Sphere *sphere = new Sphere(center, r, currentMaterial);
                surfaces.push_back(sphere);

#ifdef IM_DEBUGGING
                cout << "got a sphere with ";
                cout << "parameters: " << x << " " << y << " " << z << " " << r << endl;
#endif
                break;
            }

            case 't':   // triangle
                break;

            case 'p':   // plane
                break;

            // camera:
            case 'c': { // camera
                float x, y, z, vx, vy, vz, d, iw, ih, pw, ph;
                x = getTokenAsFloat (line, 1);
                y = getTokenAsFloat (line, 2);
                z = getTokenAsFloat (line, 3);

                vx = getTokenAsFloat (line, 4);
                vy = getTokenAsFloat (line, 5);
                vz = getTokenAsFloat (line, 6);

                d = getTokenAsFloat (line, 7);

                iw = getTokenAsFloat (line, 8);
                ih = getTokenAsFloat (line, 9);

                pw = getTokenAsFloat (line, 10);
                ph = getTokenAsFloat (line, 11);

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
                        float x, y, z, r, g, b;
                        x = getTokenAsFloat(line, 2);
                        y = getTokenAsFloat(line, 3);
                        z = getTokenAsFloat(line, 4);
                        r = getTokenAsFloat(line, 5);
                        g = getTokenAsFloat(line, 6);
                        b = getTokenAsFloat(line, 7);
                        cout << "x: " << x << ", y: " << y << ", z: " << z << ", r: " << r << ", g: " << g << ", b: " << b << endl;
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
                float ndr, ndg, ndb, nsr, nsg, nsb, nir, nig, nib, nr;
                ndr = getTokenAsFloat (line, 1);
                ndg = getTokenAsFloat (line, 2);
                ndb = getTokenAsFloat (line, 3);

                nsr = getTokenAsFloat (line, 4);
                nsg = getTokenAsFloat (line, 5);
                nsb = getTokenAsFloat (line, 6);

                nir = getTokenAsFloat (line, 7);
                nig = getTokenAsFloat (line, 8);
                nib = getTokenAsFloat (line, 9);

                nr = getTokenAsFloat (line, 10);

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
