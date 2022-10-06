#ifndef SPHERE_H
#define SPHERE_H

#include "raio.h"
#include "vetor.h"
#include "luz_pontual.h"
#include <cmath>
#include <list>

using namespace std;

class Esfera{
    public:
        Esfera(){}
        Esfera(const ponto& centro, const double raio): centro(centro), raio(raio){}

        ponto centro_esfera() {return centro;}
        double raio_esfera() { return raio; }

        pair<double,double> calcular_raizes_intersecao(const Raio& r){
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

        double calcular_intensidade_luz_esfera(list<Esfera> esferas, const Raio& direcao_luz, double raiz_mais_proxima, const luz_pontual& ponto_luz, int exp_especular, double luz_ambiente){
            
            double i = 0;
            ponto p = direcao_luz.origem() + raiz_mais_proxima*direcao_luz.direcao();

            auto vetor_normal_esfera = p - centro_esfera();
            auto L = ponto_luz.posicao_ponto()- p;
            auto normal_esfera = vetor_normal_esfera/vetor_normal_esfera.comprimento();
            auto n_dot_l = produto_vetor(normal_esfera/normal_esfera.comprimento(),L);

            Raio p_int(p,L);
            pair<Esfera, double> esfera_e_raizes_sombra = calcular_raiz_mais_proxima_interseccao(esferas,p_int,0.001,1);
            //cout << "Entrou aqui: " << esfera_e_raizes_sombra.second << "\n";
            if(esfera_e_raizes_sombra.second != INFINITY){return luz_ambiente;}

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

            return i + luz_ambiente;
        }


        static pair<Esfera,double> calcular_raiz_mais_proxima_interseccao(list<Esfera> esferas, Raio& r, double t_min, double t_max){
            double raiz_mais_proxima = INFINITY;
            Esfera esfera_mais_proxima;

            for(auto i= esferas.begin(); i!=esferas.end(); i++){
                auto raizes = i->calcular_raizes_intersecao(r);
                    
                if(raizes.first < raiz_mais_proxima && (raizes.first >= t_min && raizes.first < t_max)){
                    raiz_mais_proxima = raizes.first;
                    esfera_mais_proxima = *i;
                }
                if(raizes.second < raiz_mais_proxima && (raizes.second >= t_min && raizes.second < t_max)){
                    raiz_mais_proxima = raizes.second;
                    esfera_mais_proxima = *i;
                }
                    
            }
            return {esfera_mais_proxima, raiz_mais_proxima};

            }

    public:
        ponto centro;
        double raio;
    
};

#endif
