#ifndef COR_H
#define COR_H
#include <ostream>
#include <cmath>

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
  out << cor_pixel.r() << ' ' << cor_pixel.g() << ' ' << cor_pixel.b() << '\n';
}


inline Cor operator*(const Cor &v2, double t){
  return Cor(floor(t*v2.r()),floor(t*v2.g()),floor(t*v2.b()));
}

inline Cor operator*(const Cor &v2, const vetor &v1){
  return Cor(floor(v1.x()*v2.r()), floor(v1.y()*v2.g()), floor(v1.z()*v2.b()));
}

#endif