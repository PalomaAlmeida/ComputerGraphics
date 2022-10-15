#ifndef SPHERE_H
#define SPHERE_H

#include "raio.h"
#include "vetor.h"
#include "luz_pontual.h"
#include "objeto.h"
#include <cmath>
#include <list>

using namespace std;

class Esfera: public Objeto{
    public:
        Esfera(){}
        Esfera(const ponto& centro, const double raio): centro(centro), raio(raio){}
        Esfera(const ponto& centro, const double raio, const Cor& cor): centro(centro), raio(raio), cor_esfera(cor){}
        Esfera(const ponto& centro, const double raio, const Cor& cor, const int especular): centro(centro), raio(raio), cor_esfera(cor), exp_especular(especular){}

        ponto centro_esfera() {return centro;}
        double raio_esfera() { return raio; }
        Cor getCor() override { return cor_esfera;}

        pair<double,double> calcular_intersecao(const Raio& r) override{
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

        double calcular_intensidade_luz(const Raio& direcao_luz, double raiz_mais_proxima, const luz_pontual& ponto_luz, double luz_ambiente) override{
            double i = 0;

            ponto p = direcao_luz.origem() + raiz_mais_proxima*direcao_luz.direcao();

            auto vetor_normal_esfera = p - centro_esfera();
            auto normal_esfera = vetor_normal_esfera/vetor_normal_esfera.comprimento();
            auto L = ponto_luz.posicao_ponto()- p;
            auto n_dot_l = produto_vetor(normal_esfera,L);

            Raio p_int(p,L);
            bool objeto_possui_sombra = Objeto::calcular_objeto_mais_proximo_intersecao(p_int,0.001,1).second != INFINITY;

            if(objeto_possui_sombra){return luz_ambiente;}

            if(n_dot_l > 0){
                i += ponto_luz.intensidade_luz() * (n_dot_l  / (normal_esfera.comprimento() * L.comprimento())) ;
            }

            if(exp_especular != -1){
                auto R = 2*normal_esfera*n_dot_l - L;
                auto r_dot_v = produto_vetor(R,-direcao_luz.direcao());
                if(r_dot_v > 0){
                    i += ponto_luz.intensidade_luz() * pow(r_dot_v/(R.comprimento()*L.comprimento()),exp_especular);
                }
            }

            return min(i + luz_ambiente,1.0);
        }


    public:
        ponto centro;
        double raio;
        Cor cor_esfera;
        int exp_especular;
};

#endif
