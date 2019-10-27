#ifndef __vect_h__
#define __vect_h__

#include <math.h>

class Vect {
    double m_x, m_y, m_z;
    public:
    Vect();
    Vect(double, double, double);

    double getX() { return m_x; }
    double getY() { return m_y; }
    double getZ() { return m_z; }
    // double[] getAll() { return [x, y, z]; }


    double magnitude() {
        return sqrt((m_x*m_x) + (m_y*m_y) + (m_z*m_z));
    }

    Vect normalize() {
        // double magnitude = sqrt((m_x*m_x) + (m_y*m_y) + (m_z*m_z));
        double magnitude = this->magnitude();
        return Vect(m_x / magnitude, m_y / magnitude, m_z / magnitude);
    }

    Vect negative() {
        return Vect(-m_x, -m_y, -m_z);
    }

    double dotProduct(Vect v) {
        return m_x*v.getX() + m_y*v.getY() + m_z*v.getZ();
    }

    Vect crossProduct(Vect v) {
        return Vect(m_y*v.getZ() - m_z*v.getY(), 
                    m_z*v.getX() - m_x*v.getZ(), 
                    m_x*v.getY() - m_y*v.getX());
    }

    Vect add(Vect v) {
        return Vect(m_x+v.getX(), m_y+v.getY(), m_z+v.getZ());
    }

    Vect sub(Vect v) {
        return Vect(m_x-v.getX(), m_y-v.getY(), m_z-v.getZ());
    }

    Vect mult(double scalar) {
        return Vect(m_x*scalar, m_y*scalar, m_z*scalar);
    }

    Vect mult(Vect v) {
        return Vect(m_x*v.getX(), m_y*v.getY(), m_z*v.getZ());
    }

    Vect div(Vect v) {
        return Vect(m_x/v.getX(), m_y/v.getY(), m_z/v.getZ());
    }




};

Vect::Vect () {
    m_x = 0.0;
    m_y = 0.0;
    m_z = 0.0;
}

Vect::Vect(double x, double y, double z) {
    m_x = x;
    m_y = y;
    m_z = z;
}


#endif
