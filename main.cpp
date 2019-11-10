#include <iostream>
#include <vector>
#include <sstream>
#include <ostream>
#include <chrono>
#include <iomanip>
#include <cmath>

#include "pixelinfo.h"
#include "ppmutil.cpp"

#include "vect.h"
#include "ray.h"
#include "camera.h"
// #include "color.h"
#include "light.h"
#include "object.h"
#include "source.h"
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

Color getColorAt(   Vect intersect_position, 
                    Vect intersecting_ray_direction, 
                    vector<Object*> objects, //Array of objects in the scene
                    int idx_objs, 
                    vector <Source*> lights, //Array of lights in the scene
                    double accuracy, 
                    double ambiant_light) {
    //Color result;

    //Store current object pointer
    Object * curr_object = objects.at(idx_objs);
    ///Color object_color = curr_object->getColor();
    Color object_color = objects.at(idx_objs)->getColor();
    ///Vect object_normal = curr_object->getNormalAt(intersect_position);
    Vect object_normal = objects.at(idx_objs)->getNormalAt(intersect_position);

    // if (object_color.getSpecial() == 2) {
    //         // checkered/tile floor pattern
            
    //         int square = (int)floor(intersect_position.getX()) + (int)floor(intersect_position.getZ());
            
    //         if ((square % 2) == 0) {
    //                 // black tile
    //                 object_color.setRed(0);
    //                 object_color.setGreen(0);
    //                 object_color.setBlue(0);
    //         }
    //         else {
    //                 // white tile
    //                 object_color.setRed(1);
    //                 object_color.setGreen(1);
    //                 object_color.setRed(1);
    //         }
    // }

    Color final_color = object_color.colorScalar(ambiant_light);

    for (int light_index = 0; light_index < lights.size(); light_index++) {
        Source * current_light = lights.at(light_index);
        Vect light_direction = current_light->getPosition().add(intersect_position.negative()).normalize();
        // std::cout << "X: " << light_direction.getX() << " Y: " << light_direction.getY() << " Z: " << light_direction.getZ() << std::endl;

        float cosine_angle = object_normal.dotProduct(light_direction); //angle between light source and normal vector of object

        if (cosine_angle > 0) {
            bool shadowed = false; //by default value

            Vect distance_to_light = current_light->getPosition().add(intersect_position.negative()).normalize();
            float distance_to_light_magnitude = distance_to_light.magnitude();

            Ray shadow_ray(intersect_position, current_light->getPosition().add(intersect_position.negative()).normalize());

            vector<double> secondary_intersections;

            for (int curr_object_idx = 0; curr_object_idx < objects.size() && shadowed == false; curr_object_idx++) {
                secondary_intersections.push_back(objects.at(curr_object_idx)->findIntersection(shadow_ray));
            }
/////////////////////////////////////////////////////
            for (int sec_int_idx = 0; sec_int_idx < secondary_intersections.size(); sec_int_idx++) {
                if (secondary_intersections.at(sec_int_idx) > accuracy) {
                    if (secondary_intersections.at(sec_int_idx) <= distance_to_light_magnitude) {
                        shadowed = true;
                    }
                }
                //break;
            }
/////////////////////////////////////////////////////

            if (shadowed == false) {
              //final_color = final_color.colorAdd(winning_object_color.colorMultiply(light_sources.at(light_index)->getLightColor()).colorScalar(cosine_angle));
                final_color = final_color.Add(object_color.Mult(current_light->getColor()).colorScalar(cosine_angle));
            
            // } else {
            //     final_color = final_color.Sub(object_color.Mult(current_light->getColor()).colorScalar(-cosine_angle));
            // }

            if (object_color.getSpecial() > 0 && object_color.getSpecial() < 1.0) {
                double dot1 = object_normal.dotProduct(intersecting_ray_direction.negative());
                Vect scalar1 = object_normal.mult(dot1);
                Vect add1 = scalar1.add(intersecting_ray_direction);
                Vect scalar2 = add1.mult(2);
                Vect add2 = intersecting_ray_direction.negative().add(scalar2);
                Vect reflection_direction = add2.normalize();

                double specular = reflection_direction.dotProduct(light_direction);
                if(specular > 0) {
                    specular = pow(specular, 10);
                    final_color = final_color.Add(current_light->getColor().colorScalar(specular*object_color.getSpecial()));
                    }
                }
            }

        }

    }

    return final_color.clip();//result;
}


void render(int w, int h, Camera cam, PixelInfo *pixels, vector<Object*> objects, vector<Source*> lights) {
    int idx;
    int width_counter = 0;
    int width = w;
    int height = h;
    double aspectRadio = (double)width / (double)height;
    double xamnt, yamnt;
    double ambiant_light = 0.2;
    double accuracy = 0.01;
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

            for (int i = 0; i < objects.size(); i++) {
                // Array (or vector in cpp) with all intersections found
                intersections.push_back(objects.at(i)->findIntersection(cam_ray));
            }

            // cout << intersections.size();

            int idx_objs_winning = winningObjectIndex(intersections);

            if (_DEBUG_){
            width_counter++;
            if (width_counter == width) {
                    width_counter = 0;
                    std::cout << endl;
                } else {
                    if (idx_objs_winning == -1) std::cout << "x";
                    else if(idx_objs_winning == 0) std::cout << "o";
                    else std::cout << ".";
                }
            }

            if (idx_objs_winning == -1) {
                // Default (not intersect) color
                pixels[idx].r = 0.0;//0.047;
                pixels[idx].g = 0.0;//0.238;
                pixels[idx].b = 0.0;//0.723;
            } else {
                // Hit a object. Get color from it.
                if (intersections.at(idx_objs_winning) > accuracy) {
                    // Determine the position and direction of intersection point
                    Vect intersect_position = cam_ray_origin.add(cam_ray_direction.mult(intersections.at(idx_objs_winning)));
                    Vect intersecting_ray_direction = cam_ray_direction;

                    // Color intersection_color = objects.at(idx_objs_winning)->getColor();
                    Color intersection_color =  getColorAt(intersect_position, 
                                                           intersecting_ray_direction, 
                                                           objects, //Array of objects in the scene
                                                           idx_objs_winning, 
                                                           lights, //Array of lights in the scene
                                                           accuracy, 
                                                           ambiant_light);

                    pixels[idx].r = intersection_color.getRed();
                    pixels[idx].g = intersection_color.getGreen();
                    pixels[idx].b = intersection_color.getBlue();
                }
            //}
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
        bool big = true;
        if (big == true){
            width = 1920*multiplier;//640; //1920 / 2;
            height = 1080*multiplier;//480; //1080 / 2;//static_cast<int>(width * 0.818);
        } else {
            width = 640*multiplier;//640; //1920 / 2;
            height = 480*multiplier;//480; //1080 / 2;//static_cast<int>(width * 0.818);
        }
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
    Vect campos = Vect(-3.0, 1.5, -4.0); ///ok
    Vect diff_btw(campos.getX() - look_at.getX(), campos.getY() - look_at.getY(), campos.getZ() - look_at.getZ()); ///ok

    Vect camdir = diff_btw.negative().normalize(); ///ok
    Vect camright = Y.crossProduct(camdir).normalize(); ///ok
    Vect camdown = camright.crossProduct(camdir); ///ok

    Camera scene_cam(campos, camdir, camright, camdown); ///ok

    Color white_light (0.9, 0.9, 0.9, 0.0); ///ok
    Color pretty_green (0.35, 0.85, 0.35, 0.3); ///ok
    Color floor_color (0.75, 0.75, 0.8, 0); ///ok
    Color gray (0.5, 0.5, 0.5, 0.0); ///ok
    Color black (0.0, 0.0, 0.0, 0.0); ///ok

    Light scene_light (Vect(7, 10, -10), white_light); ///ok
    Light scene_light_2 (Vect(-1, 13, 5), white_light); ///ok
    vector<Source*> scene_lights;
    scene_lights.push_back(dynamic_cast<Source*>(&scene_light));
    scene_lights.push_back(dynamic_cast<Source*>(&scene_light_2));

    // Scene objects
    Sphere scene_sphere_2 (Vect( 2.0, -0.5, -1.5), 0.5, Color(0.85, 0.35, 0.35, 0.1)); ///ok
    Sphere scene_sphere_1 (Vect(0, 0, 0), 1, Color(0.35, 0.85, 0.35, 0.5)); ///ok
    Sphere scene_sphere_3 (Vect(-2.2, -0.5,  1.2), 0.5, Color(0.35, 0.35, 0.85, 0.9)); ///ok
    Plane scene_plane (Vect(0, 1, 0), -1, floor_color); // -1 is the distance (using normal) from the scene center.

    vector<Object*> scene_objects;
    
    scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere_1));
    scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere_2));
    scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere_3));
    
    scene_objects.push_back(dynamic_cast<Object*>(&scene_plane));





    // Stop here!
    // https://youtu.be/vE5c2hTRLZM?list=PLHm_I0tE5kKPPWXkTTtOn8fkcwEGZNETh&t=4923

    //fillBase(width, height, margin, pixels);
    auto timeBegin = std::chrono::high_resolution_clock::now();
    render(width, height, scene_cam, pixels, scene_objects, scene_lights);
    auto timeEnd = std::chrono::high_resolution_clock::now();
    auto timeDelta = std::chrono::duration_cast<std::chrono::microseconds>( timeEnd - timeBegin).count();

    
    //std::cout << std::setfill('0') << std::setw(3) << i << "] ";
    std::cout << "OK\n" << endl;
    std::cout << fixed << setprecision(8);
    std::cout << "Time by pixel: " << timeDelta / 1000000.0 / (double)(width * height) << 's' << endl; 
    std::cout << fixed << setprecision(4);
    std::cout << "Total time: " << timeDelta / 1000000.0 << 's' << endl; 
    std::cout.flush();
    saveppm("scene.ppm", width, height, pixels);

    // Tests playground
    bool _PLAYGROUND_ = false;
    if (_PLAYGROUND_) {
        double_t golden_ratio = (sqrt(5)+1)/2;
    Color c(0.94, golden_ratio, M_PI, M_PI / golden_ratio);
    double * res;

    res = c.getAll();
    std::cout << fixed << setprecision(18);
    std::cout << res[0] << " : " << res[1] << " : " << res[2] << " : " << res[3] << " : " << endl;

    }

    return 0;
}