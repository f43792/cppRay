#ifndef __source_h__
#define __source_h__

#include "vect.h"
#include "color.h"

class Source {
    // private:
    //     Vect m_position;
    //     Color m_color;
    public:
        Source();
        Source(Vect, Color);

        virtual Vect getPosition() { return Vect(0, 0, 0); }
        virtual Color getColor() {return Color(1, 1, 1, 1); }
};

Source::Source () {}
//     m_position = Vect(0.0, 0.0, 0.0);
//     m_color = Color(1.0, 1.0, 1.0, 0.0);
// }

// Source::Source(Vect p, Color c) {
//     m_position = p;
//     m_color = c;
// }


#endif