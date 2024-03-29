#ifndef __light_h__
#define __light_h__

#include "source.h"
#include "vect.h"
#include "color.h"

class Light : public Source {
    private:
        Vect m_position;
        Color m_color;
    public:
        Light();
        Light(Vect, Color);

        virtual Vect getPosition() { return m_position; }
        virtual Color getColor() {return m_color; }
};

Light::Light () {
    m_position = Vect(0.0, 0.0, 0.0);
    m_color = Color(1.0, 1.0, 1.0, 0.0);
}

Light::Light(Vect p, Color c) {
    m_position = p;
    m_color = c;
}


#endif