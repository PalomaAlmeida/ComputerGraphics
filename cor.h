#ifndef COR_H
#define COR_H
#include <ostream>
#include <cmath>
#include "vetor.h"

int COLOR_SCHEMA = 255;

class Cor{
  public:
    double c[3];
  public:
    Cor() : c{0,0,0} {}
    Cor(double r, double g, double b) : c{r, g, b} {}

    double r() const { return c[0]; }
    double g() const { return c[1]; }
    double b() const { return c[2]; }
};

void pintar(std::ostream &out, Cor cor_pixel) {
  out << floor( cor_pixel.r() * COLOR_SCHEMA) << ' ' << floor(cor_pixel.g() * COLOR_SCHEMA) << ' ' << floor(cor_pixel.b() * COLOR_SCHEMA) << '\n';
}


inline Cor operator*(const Cor &v2, double t){
  return Cor(t*v2.r(),t*v2.g(),t*v2.b());
}

inline Cor operator*(const Cor &v2, const vetor &v1){
  return Cor(v1.x()*v2.r(), v1.y()*v2.g(), v1.z()*v2.b());
}

#endif