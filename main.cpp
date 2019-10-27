#include <iostream>

#include "color.h"
#include "ppmutil.cpp"

#include "vect.h"
#include "ray.h"
#include "camera.h"

using namespace std;

int main(int argc, char *argv[]){
    int width = 1024;
    int height = 768;//static_cast<int>(width * 0.818);
    int margin = 32;
    cout << "\nRendering [" << width << ", " << height << "]..."; cout.flush();
    // int n = width * height;
    Color *pixels = new Color[width * height];

    Vect X(1, 0, 0);
    Vect Y(0, 1, 0);
    Vect Z(0, 0, 1);

    Vect look_at(0.0, 0.0, 0.0);
    Vect campos = Vect(3.0, 1.5, -4.0);
    Vect diff_btw(campos.getX() - look_at.getX(), campos.getY() - look_at.getY(), campos.getZ() - look_at.getZ());

    Vect camdir = diff_btw.negative().normalize();
    Vect camright = Y.crossProduct(camdir).normalize();
    Vect camdown = camright.crossProduct(camdir);

    Camera scene_cam(campos, camdir, camright, camdown);

    // Stop here!
    // https://youtu.be/sEdvjkFMTw4?list=PLHm_I0tE5kKPPWXkTTtOn8fkcwEGZNETh&t=2042

    int idx;
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            idx = y * width + x;
            if ((x > margin && x < width-margin) && (y > margin && y < height-margin)) {
                pixels[idx].r = 76 * (x / 255);
                pixels[idx].g = 76 * (y / 255);
                pixels[idx].b = 176;
            } else {
                pixels[idx].r = 255;
                pixels[idx].g = 255;
                pixels[idx].b = 255;
            }
        }
    }

    saveppm("scene.ppm", width, height, pixels);
    cout << "OK\n" << endl;


    // Vect a = Vect(10,10,10);
    // Vect b = Vect(8,7,6);
    // Vect c = a.mult(b);
    // cout << c.getX() << " " << c.getY() << " " << c.getZ() << endl;


    return 0;
}