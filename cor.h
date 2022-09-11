#ifndef COR_H
#define COR_H
#include <ostream>

class cor{
  public:
    double c[3];
  public:
    cor() : c{0,0,0} {}
    cor(double r, double g, double b) : c{r, g, b} {}

    double r() const { return c[0]; }
    double g() const { return c[1]; }
    double b() const { return c[2]; }
};

void pintar(std::ostream &out, cor cor_pixel) {
  out << cor_pixel.r() << ' ' << cor_pixel.g() << ' ' << cor_pixel.b() << '\n';
}

#endif