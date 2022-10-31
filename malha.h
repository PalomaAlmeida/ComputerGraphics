#ifndef MALHA_H
#define MALHA_H

#include <iostream>
#include "objeto.h"
#include "vetor.h"
#include <vector>

using namespace std;

class Malha: public Objeto{
    public:
        Malha(ponto p1, ponto p2, ponto p3){
            lista_vertices.push_back(p1);
            lista_vertices.push_back(p2);
            lista_vertices.push_back(p3);

            r1 = p2-p1;
            r2 = p3-p1;
        }

    public:
        pair<double, double> calcular_intersecao(const Raio& r) override{
            vetor normal = produto_vetorial(r1, r2);

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
        vetor calcular_intensidade_luz(const Raio& direcao_luz, double raiz_mais_proxima, const luz_pontual& ponto_luz, vetor luz_ambiente) override {
            return vetor(0,1,0);
        }

        
    public:
        vector<ponto> lista_vertices;
        vetor r1, r2;

};


#endif