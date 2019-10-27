#ifndef __camera_h__
#define __camera_h__

#include "vect.h"

class Camera {
    Vect m_pos, m_dir, m_right, m_down;
    public:
    Camera();
    Camera(Vect, Vect, Vect, Vect);

    Vect getPosition() { return m_pos; }
    Vect getDirection() {return m_dir; }
    Vect getRight() {return m_right; }
    Vect getDown() {return m_down; }
};

Camera::Camera () {
    m_pos = Vect(0.0, 0.0, 0.0);
    m_dir = Vect(0.0, 0.0, 1.0);
    m_right = Vect(0.0, 0.0, 0.0);
    m_down = Vect(0.0, 0.0, 0.0);
}

Camera::Camera(Vect position, Vect direction, Vect right, Vect down) {
    m_pos = position;
    m_dir = direction;
    m_right = right;
    m_down = down;
}

#endif