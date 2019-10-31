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