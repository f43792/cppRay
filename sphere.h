#ifndef __sphere_h__
#define __sphere_h__

#include <cmath>
#include "object.h"
#include "vect.h"
#include "color.h"

class Sphere : public Object {
    private:
        Vect m_center;
        double m_radius;
        Color m_color;
    public:
        Sphere();
        Sphere(Vect, double, Color);

        Vect getPosition() { return m_center; }
        Color getColor() {return m_color; }
        double getRadius() {return m_radius; }

        Vect getNormalAt(Vect point){
            // normal always points away from center of sphere
            Vect normal = point.add(m_center.negative()).normalize();
            return normal;
        }

        double findIntersection(Ray ray) {
            Vect ray_origin = ray.getOrigin();
            double ray_origin_x = ray_origin.getX();
            double ray_origin_y = ray_origin.getY();
            double ray_origin_z = ray_origin.getZ();

            Vect ray_direction = ray.getDirection();
            double ray_direction_x = ray_direction.getX();
            double ray_direction_y = ray_direction.getY();
            double ray_direction_z = ray_direction.getZ();

            Vect sphere_center = m_center;
            double sphere_center_x = sphere_center.getX();
            double sphere_center_y = sphere_center.getY();
            double sphere_center_z = sphere_center.getZ();

            double a = 1; //normalized
            double b = (2*(ray_origin_x - sphere_center_x)*ray_direction_x) + 
                       (2*(ray_origin_y - sphere_center_y)*ray_direction_y) + 
                       (2*(ray_origin_z - sphere_center_z)*ray_direction_z);
            double c = pow(ray_origin_x - sphere_center_x, 2) + 
                       pow(ray_origin_y - sphere_center_y, 2) +
                       pow(ray_origin_z - sphere_center_z, 2);

            double discriminant = b*b - 4*c;

            if (discriminant > 0) {
                // The ray intersectts the sphere
                // he firat root (intersection)
                double r1 = ((-1*b - sqrt(discriminant))/2) - 0.000001;
                if (r1 > 0){
                    // The first root is the smallest positive root
                    return r1;
                } else {
                    // The second root is the positive root
                    double r2 = ((sqrt(discriminant) - b) / 2) - 0.000001;
                    return r2;
                }
            } else {
                // Ray missed the sphere
                return -1;
            }


        }
};

Sphere::Sphere () {
    m_center = Vect(0.0, 0.0, 0.0);
    m_color = Color(0.5, 0.5, 0.5, 0.0);
    m_radius = 1.0;
}

Sphere::Sphere(Vect pos, double rad, Color col) {
    m_center = pos;
    m_color = col;
    m_radius = rad;
}


#endif