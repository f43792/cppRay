#ifndef __object_h__
#define __object_h__

#include "ray.h"
#include "vect.h"
#include "color.h"

class Object {
    private:
        // Vect m_center;
        // double m_radius;
        // Color m_color;
    public:
        Object();
        // Object(Vect, double, Color);

        // Vect getPosition() { return m_center; }
        Color getColor() {return Color(0.0, 0.0, 0.0, 0); }
        double findIntersection(Ray ray) {
            return 0;
        }
};

Object::Object () { }


#endif