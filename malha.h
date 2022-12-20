#ifndef MALHA_H
#define MALHA_H

#include <iostream>
#include "objeto.h"
#include "vetor.h"
#include <vector>

using namespace std;

class Malha: public Objeto{
    public:
        Malha(ponto p1, ponto p2, ponto p3, vetor k_d, vetor k_e, vetor k_a, int exp_especular): k_a(k_a), k_d(k_d), k_e(k_e), exp_especular(exp_especular){
            lista_vertices.push_back(p1);
            lista_vertices.push_back(p2);
            lista_vertices.push_back(p3);

            r1 = p2-p1;
            r2 = p3-p1;
            normal = produto_vetorial(r1,r2);
        }

        Malha(ponto p1, ponto p2, ponto p3, const char* fileName): Objeto(fileName){
            lista_vertices.push_back(p1);
            lista_vertices.push_back(p2);
            lista_vertices.push_back(p3);

            r1 = p2-p1;
            r2 = p3-p1;
            normal = produto_vetorial(r1,r2);
        }
        Malha(){}

        pair<double, double> calcular_intersecao(const Raio& r) override{

            double t_intersect = - produto_vetor((r.origem() - lista_vertices[0]), normal) / produto_vetor(r.direcao(),normal);
            vetor p_intersecao = r.origem() + t_intersect * r.direcao();

            ponto v = p_intersecao - lista_vertices[0];

            double c1 = produto_vetor(produto_vetorial((lista_vertices[2] - p_intersecao), (lista_vertices[0] - p_intersecao)), normal) / produto_vetor(r1, produto_vetorial(r2, normal));
            double c2 = produto_vetor(produto_vetorial((lista_vertices[0] - p_intersecao), (lista_vertices[1] - p_intersecao)), normal) / produto_vetor(r1, produto_vetorial(r2, normal));
            double c3 = 1 - c1 - c2;

            if(c1 < 0 || c2 < 0 || c3 < 0){
                return {INFINITY,INFINITY};
            }
            
            return {t_intersect,t_intersect};

        }
        vetor calcular_intensidade_luz(const Raio& direcao_luz, double t) override {
            vetor i = Luz::luz_ambiente->intensidade_luz * k_a;

            ponto p = direcao_luz.origem() + t*direcao_luz.direcao();

            for(auto ponto_luz: Luz::luzes){

                auto L = ponto_luz->posicao_luz- p;
                auto n_dot_l = produto_vetor(normal/normal.comprimento(),L);

                vetor intensidade_luz = ponto_luz->intensidade_luz;
            
                if(dynamic_cast<LuzSpot*>(ponto_luz) != NULL){
                    intensidade_luz = dynamic_cast<LuzSpot*>(ponto_luz)->calcular_intensidade_luz_spot(L) * ponto_luz->intensidade_luz;
                }

                Raio p_int(p,L);
                bool malha_possui_sombra = Objeto::calcular_objeto_mais_proximo_intersecao(p_int,0.001,1).second != INFINITY;
                if(malha_possui_sombra){continue;}

                if(n_dot_l > 0){
                    i += (intensidade_luz * k_d) * (n_dot_l  / (normal.comprimento() * L.comprimento()));
                }

                if(exp_especular != -1){
                    auto R = 2*normal*n_dot_l - L;
                    auto r_dot_l = produto_vetor(R,-direcao_luz.direcao());
                    if(r_dot_l > 0){
                        i += (intensidade_luz * k_e) * pow(r_dot_l/(R.comprimento()*L.comprimento()),exp_especular);
                    }
                }

            }

            //Dividir todos pela maior componente de i se alguma componente for maior que 1
            if(i.x() > 1 || i.y() > 1 || i.z() > 1){
                double maior_componente = max( max(i.x(),i.y()) , i.z());
                return i/maior_componente;
            }

            return i;
        }
        
        void transformacao(){}
        void atualizar_normal(){}
        void atualizar_normal(Matriz M){}
    public:
        vector<ponto> lista_vertices;
        vetor r1, r2;
        vetor normal;
        vetor k_a, k_d, k_e;
        int exp_especular;

};


#endif