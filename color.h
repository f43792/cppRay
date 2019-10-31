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
        // double[] getAll() { return [x, y, z]; }

        double setRed(double value) { m_red = value; };
        double setGreen(double value) { m_green = value; };
        double setBlue(double value) { m_blue = value; };
        double setSpecial(double value) { m_special = value; };
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


#endif
