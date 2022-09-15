#ifndef SPHERE_H
#define SPHERE_H

#include "vetor.h"
#include "raio.h"
#include "luz_pontual.h"
#include <cmath>

using namespace std;

class esfera{
    public:
        esfera(){}
        esfera(const ponto& centro, const double raio): centro(centro), raio(raio){}

        ponto centro_esfera() {return centro;}
        double raio_esfera() { return raio; }

        pair<double,double> calcular_raizes_intersecao(const raio& r){
             vetor p = r.origem() - centro_esfera();

            auto a = produto_vetor(r.direcao(),r.direcao());
            auto b = 2.0 * produto_vetor(p, r.direcao());
            auto c = produto_vetor(p, p) - raio_esfera()*raio_esfera();

            auto descriminante = b*b - 4*a*c;
            if(descriminante < 0){
                return {INFINITY,INFINITY};
            }

            auto t1 = (-b + sqrt(descriminante)) / (2*a);
            auto t2 = (-b - sqrt(descriminante)) / (2*a);
            return {t1,t2};
        }

        double calcular_intensidade_luz_esfera(const raio& direcao_luz, double raiz_mais_proxima, const luz_pontual& ponto_luz, int exp_especular){
            double i = 0;

            auto p_int = direcao_luz.origem() + raiz_mais_proxima*direcao_luz.direcao();

            auto vetor_normal_esfera = p_int - centro_esfera();
            auto L = ponto_luz.posicao_ponto()- p_int;
            auto normal_esfera = vetor_normal_esfera/vetor_normal_esfera.comprimento();
            auto n_dot_l = produto_vetor(normal_esfera/normal_esfera.comprimento(),L);

            if(n_dot_l > 0){
                i += ponto_luz.intensidade_luz() * n_dot_l  / (normal_esfera.comprimento() * L.comprimento()) ;
            }

            if(exp_especular != -1){
                auto R = 2*normal_esfera*n_dot_l - L;
                auto r_dot_l = produto_vetor(R,-direcao_luz.direcao());
                if(r_dot_l > 0){
                i += ponto_luz.intensidade_luz() * pow(r_dot_l/(R.comprimento()*L.comprimento()),exp_especular);
                }
            }

            return i;
        }

    public:
        ponto centro;
        double raio;
    
};

#endif
