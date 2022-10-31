#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <vector>
#include <cmath>
#include "esfera.h"
#include "raio.h"
#include "cor.h"
#include "luz_pontual.h"

using namespace std;

class Objeto{
    public:
        virtual pair<double, double> calcular_intersecao(const Raio& r) = 0;
        virtual vetor calcular_intensidade_luz(const Raio& direcao_luz, double raiz_mais_proxima, const luz_pontual& ponto_luz, vetor luz_ambiente) = 0;

        static pair<Objeto*, double> calcular_objeto_mais_proximo_intersecao(Raio& r, double t_min, double t_max){
            double raiz_mais_proxima = INFINITY;
            Objeto* objeto_mais_proximo;

            for(auto i= objetos.begin(); i!=objetos.end(); i++){

                auto raizes = (*i)->calcular_intersecao(r);
                    
                if(raizes.first < raiz_mais_proxima && (raizes.first >= t_min && raizes.first < t_max)){
                    raiz_mais_proxima = raizes.first;
                    objeto_mais_proximo = *i;
                }
                if(raizes.second < raiz_mais_proxima && (raizes.second >= t_min && raizes.second < t_max)){
                    raiz_mais_proxima = raizes.second;
                    objeto_mais_proximo = *i;
                }
                    
            }
            return {objeto_mais_proximo, raiz_mais_proxima};
        }

    public:
        static vector<Objeto*> objetos;
};

#endif