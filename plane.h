#ifndef __plane_h__
#define __plane_h__

#include <cmath>
#include "object.h"
#include "vect.h"
#include "color.h"

class Plane : public Object {
    private:
        Vect m_normal;
        double m_distance;
        Color m_color;
    public:
        Plane();
        Plane(Vect, double, Color);

        Vect getNormal() { return m_normal; }
        Color getColor() {return m_color; }
        double getDistance() {return m_distance; }

        Vect getNormalAt(Vect point) {
            return m_normal;
        }

        double findIntersection(Ray ray) {
            Vect ray_direction = ray.getDirection();

            double a = ray_direction.dotProduct(m_normal);

            if (a == 0) {
                // ray is parallel to the plane
                return -1.0;
            } else {
                double b = m_normal.dotProduct(ray.getOrigin().add(m_normal.mult(m_distance).negative()));
                return -1*b/a;
            }

        }

};

Plane::Plane () {
    m_normal = Vect(1.0, 0.0, 0.0);
    m_color = Color(0.5, 0.5, 0.5, 0.0);
    m_distance = 1.0;
}

Plane::Plane(Vect pos, double rad, Color col) {
    m_normal = pos;
    m_color = col;
    m_distance = rad;
}


#endif