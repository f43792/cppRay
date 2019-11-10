#ifndef __color_h__
#define __color_h__

class Color {
    private:
        double m_red, m_green, m_blue, m_special;
    public:
        Color();
        Color(double, double, double, double);

        double getRed() { return m_red; }
        double getGreen() { return m_green; }
        double getBlue() { return m_blue; }
        double getSpecial() { return m_special; }
        double * getAll();

        double setRed(double value) { m_red = value; };
        double setGreen(double value) { m_green = value; };
        double setBlue(double value) { m_blue = value; };
        double setSpecial(double value) { m_special = value; };

        double brightness() {
            return (m_red + m_green + m_blue) / 3;
        }

        Color colorScalar(double scalar) {
            return Color (m_red*scalar, m_green*scalar, m_blue*scalar, m_special);
        }

        Color Add(Color c) {
            return Color(m_red + c.getRed(), m_green + c.getGreen(), m_blue + c.getBlue(), m_special);
        }

        Color Sub(Color c) {
            return Color(m_red - c.getRed(), m_green - c.getGreen(), m_blue - c.getBlue(), m_special);
        }

        Color Mult(Color c) {
            return Color(m_red * c.getRed(), m_green * c.getGreen(), m_blue * c.getBlue(), m_special);
        }

        Color Average(Color c) {
            return Color((m_red + c.getRed()) / 2,
            (m_green + c.getGreen()) / 2, 
            (m_blue + c.getBlue()) / 2, 
            m_special);
        }

        Color clip() {
            double alllight = m_green + m_green + m_blue;
            double excesslight = alllight - 3;
            if (excesslight > 0) {
                m_red = m_red + excesslight*(m_green/alllight);
                m_green = m_green + excesslight*(m_green/alllight);
                m_blue = m_blue + excesslight*(m_blue/alllight);
            }
            if (m_red > 1) {m_red = 1;}
            if (m_green > 1) {m_green = 1;}
            if (m_blue > 1) {m_blue = 1;}
            if (m_red < 0) {m_red = 0;}
            if (m_green < 0) {m_green = 0;}
            if (m_blue < 0) {m_blue = 0;}
            
            return Color (m_red, m_green, m_blue, m_special);
        }

};

Color::Color () {
    // Default color is gray
    m_red = 0.5;
    m_green = 0.5;
    m_blue = 0.5;
}

Color::Color(double r, double g, double b, double s) {
    m_red = r;
    m_green = g;
    m_blue = b;
    m_special = s;
}

double * Color::getAll() {
    static double result[4];
    result[0] = m_red;
    result[1] = m_green;
    result[2] = m_blue;
    result[3] = m_special;

    return result;
}


#endif
