#ifndef PLANE_H
#define PLANE_H

#include "vetor.h"
#include "raio.h"
#include "luz_pontual.h"
#include <cmath>

using namespace std;

class plano{
    public:
        plano(){}
        plano(const ponto& pont_especific, const vetor& normal_plano) : ponto_especific(pont_especific), normal(normal_plano){}

        ponto pont_especific() const {return ponto_especific;}
        vetor normal_plano() const { return normal;}

        ponto calcula_plano(ponto ponto_qualquer) const {
            return (ponto_qualquer - ponto_especific) * normal;
        }; 

        double calcula_intersecao(const raio& r){
            double prod_escalar = produto_vetor(r.direcao(),normal);
            auto sub = ponto_especific - r.origem();
            double prod_escalar2 = produto_vetor(sub,normal);

            double t = prod_escalar2/prod_escalar;
           
            if(t < 0){
                return {};
            }
            return t; // r.origem() + (t*r.direcao());
        };

        double calcular_luz_plano(const raio& direcao_luz, double t, const luz_pontual& ponto_luz, int exp_especular){
            double i = 0;

            auto p_int = direcao_luz.origem() + t*direcao_luz.direcao();

            auto L = ponto_luz.posicao_ponto()- p_int;
            auto normal_plano = normal/normal.comprimento();
            auto n_dot_l = produto_vetor(normal_plano/normal_plano.comprimento(),L);

            if(n_dot_l > 0){
                i += ponto_luz.intensidade_luz() * n_dot_l  / (normal.comprimento() * L.comprimento()) ;
            }

            if(exp_especular != -1){
                auto R = 2*normal*n_dot_l - L;
                auto r_dot_l = produto_vetor(R,-direcao_luz.direcao());
                if(r_dot_l > 0){
                i += ponto_luz.intensidade_luz() * pow(r_dot_l/(R.comprimento()*L.comprimento()),exp_especular);
                }
            }
        };

    public:
        ponto ponto_especific;
        vetor normal;


};

#endif