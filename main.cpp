#include <iostream>
#include <vector>
#include <sstream>
#include <ostream>
#include <chrono>

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

bool _DEBUG_ = false;

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
        if (_DEBUG_) {std::cout << "no intersection" << endl; }
        return -1;
    } else if (object_intersections.size() == 1) {
        // Just one intersection
        if (object_intersections.at(0) > 0) {
            // If that intersection is greather than 0, them it's
            if (_DEBUG_) {std::cout << "only value is zero" << endl; }
            return 0;
        } else {
            // Otherwise the only value is negative
            if (_DEBUG_) {std::cout << "only value is negative" << endl; }
            return -1;
        }
    } else {
        // otherwise there is more than one intersection
        // Verify the maximum value in the vector
        double max_int_value = 0;
        for (int i = 0; i < object_intersections.size(); i++) {
            if (max_int_value < object_intersections.at(i)) {
                max_int_value = object_intersections.at(i);
            }
        }
        // Then starting from the maximum value, find the minimum
        if (max_int_value > 0){
            // Only positive intersections
            for (int idx = 0; idx < object_intersections.size(); idx++) {
                if (object_intersections.at(idx) > 0 && object_intersections.at(idx) <= max_int_value) {
                    max_int_value = object_intersections.at(idx);
                    idx_min_value = idx;
                }
            }
        
            return idx_min_value;

        } else {
            // All intersections are negative
            if (_DEBUG_) {cout << "all intersections are negative" << endl; }
            return -1;
        }
        
    }
}

string showVect(Vect &vect) {
    std::ostringstream result;
    string dtab = "\t\t";
    result << "Vector: " << vect.getX() << dtab << vect.getY() << dtab << vect.getZ() << endl;
    return result.str();
}

void render(int w, int h, Camera cam, PixelInfo *pixels, vector<Object*> objects) {
    int idx;
    int width_counter = 0;
    int width = w;
    int height = h;
    double aspectRadio = (double)width / (double)height;
    double xamnt, yamnt;
    // std::cout << "Objects to render: " << objects.size() << endl;

    for(int y = 0; y < height; y++){ //for height
        for(int x = 0; x < width; x++){ //for width
            idx = y * width + x;

            // No anti-aliasing for now
            if (width > height) {
                // Img wider than tall
                xamnt = ((x+0.5)/width) * aspectRadio - (((width-height)/(double)height)/2); ///ok
                yamnt = ((height - y) + 0.5)/height; ///ok
            } else if(height > width) {
                // Img taller than wide
                xamnt = (x + 0.5) / width; ///ok
                yamnt = (((height - y) + 0.5) / height)/aspectRadio - (((height - width)/(double)width) / 2); ///ok
            } else {
                // Square image
                xamnt = (x + 0.5)/width; ///ok
                yamnt = ((height - y) + 0.5)/height; ///ok
            }


            Vect cam_ray_origin = cam.getPosition();
            Vect cam_dir = cam.getDirection();
            Vect cam_right = cam.getRight();
            Vect cam_down = cam.getDown();
            //Vect cam_ray_direction = cam.getDirection().add(cam.getRight().mult(xamnt - 0.5).add(cam.getDown().mult(yamnt - 0.5))).normalize();
            Vect cam_ray_direction = cam_dir.add(cam_right.mult(xamnt - 0.5).add(cam_down.mult(yamnt - 0.5))).normalize();
            //cout << showVect(cam_ray_direction);
            
            Ray cam_ray(cam_ray_origin, cam_ray_direction);

            vector<double> intersections;

            for (int idx_obj = 0; idx_obj < objects.size(); idx_obj++) {
                intersections.push_back(objects.at(idx_obj)->findIntersection(cam_ray));
            }

            // cout << intersections.size();

            int idx_objs = winningObjectIndex(intersections);

            if (_DEBUG_){
            width_counter++;
            if (width_counter == width) {
                    width_counter = 0;
                    std::cout << endl;
                } else {
                    if (idx_objs == -1) std::cout << "x";
                    else if(idx_objs == 0) std::cout << "o";
                    else std::cout << ".";
                }
            }

            if (idx_objs == -1) {
                pixels[idx].r = 0;
                pixels[idx].g = 0;
                pixels[idx].b = 0;
            } else {
                // Hit a object. Get color from it.
                Color color = objects.at(idx_objs)->getColor();
                pixels[idx].r = color.getRed();
                pixels[idx].g = color.getGreen();
                pixels[idx].b = color.getBlue();
            }
        }
    }
}


int main(int argc, char *argv[]){
    int width;//640; //1920 / 2;
    int height;//480; //1080 / 2;//static_cast<int>(width * 0.818);    
    if (_DEBUG_) {
        width = 128;//640; //1920 / 2;
        height = 64;//480; //1080 / 2;//static_cast<int>(width * 0.818);
    } else {
        int multiplier = 1;
        width = 1920*multiplier;//640; //1920 / 2;
        height = 1080*multiplier;//480; //1080 / 2;//static_cast<int>(width * 0.818);
    }
    int margin = 32;
    std::cout << "\nRendering [" << width << ", " << height << "]";// << "]..."; 
    if (_DEBUG_) {std::cout << endl;}
    else {std::cout << "..."; }
    std::cout.flush();
    // int n = width * height;
    PixelInfo *pixels = new PixelInfo[width * height];

    Vect O(0, 0, 0); ///ok
    Vect X(1, 0, 0); ///ok
    Vect Y(0, 1, 0); ///ok
    Vect Z(0, 0, 1); ///ok

    Vect look_at(0.0, 0.0, 0.0); ///ok
    Vect campos = Vect(3.0, 1.5, -4.0); ///ok
    Vect diff_btw(campos.getX() - look_at.getX(), campos.getY() - look_at.getY(), campos.getZ() - look_at.getZ()); ///ok

    Vect camdir = diff_btw.negative().normalize(); ///ok
    Vect camright = Y.crossProduct(camdir).normalize(); ///ok
    Vect camdown = camright.crossProduct(camdir); ///ok

    Camera scene_cam(campos, camdir, camright, camdown); ///ok

    Color white_light (1.0, 1.0, 1.0, 0.0); ///ok
    Color pretty_green (0.5, 1.0, 0.5, 0.3); ///ok
    Color maroon (0.5, 0.25, 0.25, 0); ///ok
    Color gray (0.5, 0.5, 0.5, 0.0); ///ok
    Color black (0.0, 0.0, 0.0, 0.0); ///ok

    Light scene_light (Vect(-7, 10, -10), white_light); ///ok

    // Scene objects
    Sphere scene_sphere (Vect(0, 0, 0), 1, pretty_green); ///ok
    Plane scene_plane (Vect(0, 1, 0), -1, maroon); // -1 is the distance (using normal) from the scene center.

    vector<Object*> scene_objects;
    scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere));
    scene_objects.push_back(dynamic_cast<Object*>(&scene_plane));




    // Stop here!
    // https://youtu.be/vE5c2hTRLZM?list=PLHm_I0tE5kKPPWXkTTtOn8fkcwEGZNETh&t=2

    //fillBase(width, height, margin, pixels);
    auto timeBegin = std::chrono::high_resolution_clock::now();
    render(width, height, scene_cam, pixels, scene_objects);
    auto timeEnd = std::chrono::high_resolution_clock::now();
    auto timeDelta = std::chrono::duration_cast<std::chrono::microseconds>( timeEnd - timeBegin).count();

    std::cout << "OK\n" << endl;
    std::cout << "Time by pixel: " << timeDelta / 1000000.0 / (double)(width * height) << endl; 
    std::cout << "Total time: " << timeDelta / 1000000.0 << "s" << endl; 
    std::cout.flush();
    saveppm("scene.ppm", width, height, pixels);

    return 0;
}