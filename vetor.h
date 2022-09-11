#ifndef VETOR_H
#define VETOR_H
#include <cmath>
#include <ostream>

class vetor{
  public:
    double v[3];
  public:
    vetor() : v{0,0,0} {}
    vetor(double e0, double e1, double e2) : v{e0, e1, e2} {}

    double x() const { return v[0]; }
    double y() const { return v[1]; }
    double z() const { return v[2]; }

    vetor operator-() const { return vetor(-v[0], -v[1], -v[2]); }
    double operator[](int i) const { return v[i]; }
    double& operator[](int i) { return v[i]; }

    vetor& operator+=(const vetor &v2) {
      v[0] += v2.v[0];
      v[1] += v2.v[1];
      v[2] += v2.v[2];
      return *this;
    }

    vetor& operator*=(const double t) {
      v[0] *= t;
      v[1] *= t;
      v[2] *= t;
      return *this;
    }

    vetor& operator/=(const double t) {
      return *this *= 1/t;
    }

    double comprimento() const {
      return sqrt(comprimento_ao_quadrado());
    }

    double comprimento_ao_quadrado() const {
      return v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
    }

};

using ponto = vetor;

inline std::ostream& operator<<(std::ostream &out, const vetor &v2) {
  return out << v2.v[0] << ' ' << v2.v[1] << ' ' << v2.v[2];
}

inline vetor operator+(const vetor &u, const vetor &v2) {
  return vetor(u.v[0] + v2.v[0], u.v[1] + v2.v[1], u.v[2] + v2.v[2]);
}

inline vetor operator-(const vetor &u, const vetor &v2) {
  return vetor(u.v[0] - v2.v[0], u.v[1] - v2.v[1], u.v[2] - v2.v[2]);
}

inline vetor operator*(const vetor &u, const vetor &v2){
  return vetor(u.v[0] * v2.v[0], u.v[1] * v2.v[1], u.v[2] * v2.v[2]);
}

inline vetor operator*(double t, const vetor &v2) {
  return vetor(t*v2.v[0], t*v2.v[1], t*v2.v[2]);
  }

inline vetor operator*(const vetor &v2, double t) {
  return t * v2;
}

inline vetor operator/(vetor v, double t) {
  return (1/t) * v;
}


inline double produto_vetor(const vetor &u, const vetor &v){
  return u.v[0] * v.v[0] + u.v[1] * v.v[1] + u.v[2] * v.v[2];
}

#endif