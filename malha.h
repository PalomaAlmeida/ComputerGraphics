#ifndef MALHA_H
#define MALHA_H

#include <iostream>
#include "objeto.h"
#include "vetor.h"
#include <vector>
#include <cmath>

using namespace std;

class Malha: public Objeto{
    public:
        Malha(vetor centro, ponto p1, ponto p2, ponto p3, vetor k_d, vetor k_e, vetor k_a, int exp_especular): Objeto(centro, k_d,k_e,k_a, exp_especular){}
        
        Malha(){}

        Malha(ponto p1, ponto p2, ponto p3, const char* fileName): Objeto(fileName){}
    
    public:
        vetor centro;
        std::vector<vetor*> vertices;

        class Face {
            public:
                ponto p1, p2, p3;
                vetor normal;

            public:
                Face(){}
                Face(ponto p1, ponto p2, ponto p3): p1(p1), p2(p2), p3(p3){ 
                    calcula_normal();
                }

                void calcula_normal(){
                    normal = produto_vetorial((p2-p1),(p3-p1));
                }

                void set_p1(double aresta, ponto centro){
                    this->p1 = ponto(centro.x()+aresta/2, centro.y(), centro.z()+aresta/2);
                }
                void set_p2(double aresta, ponto centro){
                    this->p2 = ponto(centro.x()+aresta/2, centro.y(), centro.z()+aresta/2);
                }
                void set_p3(double aresta, ponto centro){
                    this->p3 = ponto(centro.x()+aresta/2, centro.y(), centro.z()+aresta/2);
                }

                vetor get_p1(){ return p1; }
                vetor get_p2(){ return p2;}
                vetor get_p3(){ return p3; }

                vetor get_normal(){ return normal; }
        };

        std::vector<Face*> faces;

    public:
        std::vector<vetor*> vertices;

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

            for(auto ponto_luz: Luz::luzes_pontuais){

                auto L = ponto_luz->posicao_luz- p;
                auto n_dot_l = produto_vetor(normal/normal.comprimento(),L);

                Raio p_int(p,L);
                bool malha_possui_sombra = Objeto::calcular_objeto_mais_proximo_intersecao(p_int,0.001,1).second != INFINITY;
                if(malha_possui_sombra){continue;}

                if(n_dot_l > 0){
                    i += (ponto_luz->intensidade_luz * k_d) * (n_dot_l  / (normal.comprimento() * L.comprimento()));
                }

                if(exp_especular != -1){
                    auto R = 2*normal*n_dot_l - L;
                    auto r_dot_l = produto_vetor(R,-direcao_luz.direcao());
                    if(r_dot_l > 0){
                        i += (ponto_luz->intensidade_luz * k_e) * pow(r_dot_l/(R.comprimento()*L.comprimento()),exp_especular);
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

        void transformacao() {
            Matriz M = Matriz::identidade(4);
            for(Matriz m:this->get_transformation()) M = M * m;

            this->centro = (M * Matriz::vetor_para_matriz(this->centro, 1)).matriz_para_vetor();  

            for(vetor *&vertice : this->vertices) {
                vetor v = (M * Matriz::vetor_para_matriz(*vertice, 1)).matriz_para_vetor();  

                vertice->x(v.x());
                vertice->y(v.y()); 
                vertice->z(v.z());
                vertice->a(v.a());
            }
            
            this->limpar_transformacao();
        }

        void atualizar_normal() {
            for(Face * f: this->faces) f->calcula_normal();
            for(Face * f: this->faces) f->normal = f->normal * this->get_invertida();
        }

        void atualizar_normal(Matriz M) {
            for(Face * f: this->faces) f->normal = (~M * Matriz::vetor_para_matriz(f->normal, true)).matriz_para_vetor();
        }

        
    public:
        vector<ponto> lista_vertices;
        vetor r1, r2;
        vetor normal;

};


#endif