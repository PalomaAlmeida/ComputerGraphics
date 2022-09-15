#ifndef SPHERE_H
#define SPHERE_H

#include "vetor.h"
#include "raio.h"
#include <cmath>

using namespace std;

class esfera{
    public:
        esfera(){}
        esfera(const ponto& centro_esfera, const double raio): centro(centro_esfera), raio(raio){}

        pair<double,double> calcular_raizes_intersecao(const raio& r){
             vetor p = r.origem() - centro;

            auto a = produto_vetor(r.direcao(),r.direcao());
            auto b = 2.0 * produto_vetor(p, r.direcao());
            auto c = produto_vetor(p, p) - raio*raio;

            auto descriminante = b*b - 4*a*c;
            if(descriminante < 0){
                return {INFINITY,INFINITY};
            }

            auto t1 = (-b + sqrt(descriminante)) / (2*a);
            auto t2 = (-b - sqrt(descriminante)) / (2*a);
            return {t1,t2};
        }

    public:
        ponto centro;
        double raio;
};

#endif
