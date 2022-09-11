#ifndef RAY_H
#define RAY_H

#include "vetor.h"

class raio {
  public:
    raio() {}
    raio(const ponto& origem, const vetor& direcao): orig(origem), dir(direcao)
    {}

    ponto origem() const  { return orig; }
    vetor direcao() const { return dir; }

    ponto calcular_raio(double t) const {
      return orig + t*dir;
    }

  public:
    ponto orig;
    vetor dir;
};



#endif