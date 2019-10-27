#ifndef __ray_h__
#define __ray_h__

#include "vect.h"

class Ray {
    Vect m_origin, m_direction;
    public:
    Ray();
    Ray(Vect, Vect);

    Vect getOrigin() { return m_origin; }
    Vect getDirection() {return m_direction; }
};

Ray::Ray () {
    m_origin = Vect(0.0, 0.0, 0.0);
    m_direction = Vect(1.0, 0.0, 0.0);
}

Ray::Ray(Vect o, Vect d) {
    m_origin = o;
    m_direction = d;
}


#endif