#include <iostream>
#include <vector>

#include "pixelinfo.h"
#include "ppmutil.cpp"

#include "vect.h"
#include "ray.h"
#include "camera.h"
// #include "color.h"
#include "light.h"
#include "object.h"
#include "sphere.h"
#include "plane.h"

using namespace std;

void fillBase(int w, int h, int m, PixelInfo *pixels) {
    int idx;
    int width = w;
    int height = h;
    int margin = m;
    int wm = width - margin;
    int wh = height - margin;

    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            idx = y * width + x;
            if ((x > margin && x < width-margin) && (y > margin && y < height-margin)) {
                pixels[idx].r = (wm / 32) * (x / (wm / 8));
                pixels[idx].g = (wh / 32) * (y / (wh / 8));
                pixels[idx].b = 176;
            } else {
                pixels[idx].r = 255;
                pixels[idx].g = 255;
                pixels[idx].b = 255;
            }
        }
    }
}

int winningObjectIndex(vector<double> object_intersections) {
    // return the index of the intersection
    int idx_min_value;

    // Prevent unnessessary calculations
    if (object_intersections.size() == 0) {
        // if there no intersections
        cout << "no intersection" << endl;
        return -1;
    } else if (object_intersections.size() == 1) {
        // Just one intersection
        if (object_intersections.at(0) > 0) {
            // If that intersection is greather than 0, them it's
            return 0;
        } else {
            // Otherwise the only value is negative
            cout << "only value is negative" << endl;
            return -1;
        }
    } else {
        // otherwise there is more than one intersection
        // Verify the maximum value in the vector
        double max = 0;
        for (int i = 0; i < object_intersections.size(); i++) {
            if (max < object_intersections.at(i)) {
                max = object_intersections.at(i);
            }
        }
        // Then starting from the maximum value, find the minimum
        if (max > 0){
            // Only positive intersections
            for (int idx = 0; idx < object_intersections.size(); idx++) {
                if (object_intersections.at(idx) > 0 && object_intersections.at(idx) <= max) {
                    max = object_intersections.at(idx);
                    idx_min_value = idx;
                }
            }
            return idx_min_value;
        } else {
            // All intersections are negative
            cout << "all intersections are negative" << endl;
            return -1;
        }
        
    }
}

void render(int w, int h, double ar, Camera cam, PixelInfo *pixels, vector<Object*> objects) {
    int idx;
    int width = w;
    int height = h;
    double aspectRadio = ar;
    double xamnt, yamnt;

    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            idx = y * width + x;
            pixels[idx].r = x/3;
            pixels[idx].g = y/3;
            pixels[idx].b = 198;

            // No anti-aliasing for now
            if (width > height) {
                // Img wider tham tall
                xamnt = ((x+0.5)/width) * aspectRadio - (((width-height)/(double)height)/2);
                yamnt = ((height - y) + 0.5)/height;
            } else if(height > width) {
                // Img taller tham wide
                xamnt = (x + 0.5) / width;
                yamnt = (((height - y) + 0.5) / height)/aspectRadio - (((height - width)/(double)width) / 2);
            } else {
                // Square image
                xamnt = (x + 0.5)/width;
                yamnt = ((height - y) + 0.5)/height;
            }

            Vect cam_ray_origin = cam.getPosition();
            Vect cam_ray_direction = cam.getDirection().add(cam.getRight().mult(xamnt - 0.5).add(cam.getDown().mult(yamnt - 0.5))).normalize();

            Ray cam_ray(cam_ray_origin, cam_ray_direction);

            vector<double> intersections;

            for (int idx = 0; idx < objects.size(); idx++) {
                intersections.push_back(objects.at(idx)->findIntersection(cam_ray));
            }

            int idx_objs = winningObjectIndex(intersections);

            if (idx_objs >= 0) cout << "winning obj: " << idx_objs << endl;

        }
    }
}

int main(int argc, char *argv[]){
    int width = 1920 / 2;
    int height = 1080 / 2;//static_cast<int>(width * 0.818);
    int margin = 32;
    double aspectRatio = (double)width / (double)height;
    cout << "\nRendering [" << width << ", " << height << "]..."; cout.flush();
    // int n = width * height;
    PixelInfo *pixels = new PixelInfo[width * height];

    Vect O(0, 0, 0);
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

    Color white_light (1.0, 1.0, 1.0, 0.0);
    Color pretty_green (0.5, 1.0, 0.5, 0.3);
    Color maroon (0.5, 0.25, 0.25, 0);
    Color gray (0.5, 0.5, 0.5, 0.0);
    Color black (0.0, 0.0, 0.0, 0.0);

    Vect light_pos (-7, 10, -10);
    Light scene_light (light_pos, white_light);

    // Scene objects
    Sphere scene_sphere (O, 1, pretty_green);
    Plane scene_plane (Y, -1, maroon);

    vector<Object*> scene_objects;
    scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere));
    scene_objects.push_back(dynamic_cast<Object*>(&scene_plane));




    // Stop here!
    // https://youtu.be/vE5c2hTRLZM?list=PLHm_I0tE5kKPPWXkTTtOn8fkcwEGZNETh&t=2

    //fillBase(width, height, margin, pixels);
    render(width, height, aspectRatio, scene_cam, pixels, scene_objects);

    saveppm("scene.ppm", width, height, pixels);
    cout << "OK\n" << endl;

    return 0;
}