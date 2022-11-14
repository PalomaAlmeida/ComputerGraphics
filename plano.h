#ifndef PLANE_H
#define PLANE_H

#include "objeto.h"
#include "vetor.h"
#include "raio.h"
#include "luz_pontual.h"
#include <cmath>

using namespace std;

class Plano: public Objeto{
    public:
        Plano(){}
        Plano(const ponto& pont_especific, const vetor& normal_plano) : ponto_especific(pont_especific), normal(normal_plano){}
        Plano(const ponto& pont_especific, const vetor& normal_plano, const Cor& cor) : ponto_especific(pont_especific), normal(normal_plano), cor_plano(cor){}
        Plano(const ponto& pont_especific, const vetor& normal_plano, const Cor& cor, const int especular) : ponto_especific(pont_especific), normal(normal_plano), cor_plano(cor), exp_especular(especular){}
        Plano(const ponto& pont_especific, const vetor& normal_plano, const char* fileName, int m) : ponto_especific(pont_especific), normal(normal_plano), Objeto(fileName, m){}

        ponto pont_especific() const {return ponto_especific;}
        vetor normal_plano() const { return normal;}
        Cor getCor() override {return cor_plano;}
        int especular() const {return exp_especular;}

        ponto calcula_plano(ponto ponto_qualquer) const {
            return (ponto_qualquer - ponto_especific) * normal;
        }; 

        pair<double,double> calcular_intersecao(const Raio& r) override{
            ponto p0 = r.origem();
            auto t_int = produto_vetor( (ponto_especific - p0), normal) / produto_vetor(r.direcao(), normal);
            
            if(t_int <= 0){
                return {INFINITY,INFINITY};
            }

            return {t_int,t_int};
        };

        double calcular_intensidade_luz(const Raio& direcao_luz, double t, const luz_pontual& ponto_luz, double luz_ambiente) override{
            double i = 0;

            ponto p = direcao_luz.origem() + t*direcao_luz.direcao();

            auto L = ponto_luz.posicao_ponto()- p;
            auto n_dot_l = produto_vetor(normal/normal.comprimento(),L);

            Raio p_int(p,L);
            bool plano_possui_sombra = Objeto::calcular_objeto_mais_proximo_intersecao(p_int,0.001,1).second != INFINITY;
            if(plano_possui_sombra){return luz_ambiente;}

            if(n_dot_l > 0){
                i += ponto_luz.intensidade_luz() * (n_dot_l  / (normal.comprimento() * L.comprimento()));
            }

            if(exp_especular != -1){
                auto R = 2*normal*n_dot_l - L;
                auto r_dot_l = produto_vetor(R,-direcao_luz.direcao());
                if(r_dot_l > 0){
                    i += ponto_luz.intensidade_luz() * pow(r_dot_l/(R.comprimento()*L.comprimento()),exp_especular);
                }
            }
            return min(i + luz_ambiente,1.0);
        };

    public:
        ponto ponto_especific;
        vetor normal;
        Cor cor_plano;
        int exp_especular;

};

#endif