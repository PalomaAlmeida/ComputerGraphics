#ifndef PLANE_H
#define PLANE_H

#include "objeto.h"
#include "vetor.h"
#include "raio.h"
#include "luz.h"
#include "luz_spot.h"
#include <cmath>


using namespace std;

class Plano: public Objeto{
    public:
        Plano(){}
        Plano(const ponto& pont_especific, const vetor& normal_plano, vetor k_d, vetor k_e, vetor k_a) : ponto_especific(pont_especific), normal(normal_plano), Objeto(pont_especific,k_d,k_e,k_a){}
        Plano(const ponto& pont_especific, const vetor& normal_plano, vetor k_d, vetor k_e, vetor k_a, const int especular) : ponto_especific(pont_especific), normal(normal_plano), Objeto(pont_especific, k_d,k_e,k_a,especular){}
        Plano(const ponto& pont_especific, const vetor& normal_plano, const char* fileName) : ponto_especific(pont_especific), normal(normal_plano), Objeto(fileName){}

        ponto pont_especific() const {return ponto_especific;}
        vetor normal_plano() const { return normal;}
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

        vetor calcular_intensidade_luz(const Raio& direcao_luz, double t) override{
            vetor i = Luz::luz_ambiente->intensidade_luz * k_a;

            ponto p = direcao_luz.origem() + t*direcao_luz.direcao();

            for(auto ponto_luz: Luz::luzes){

                auto L = ponto_luz->posicao_luz - p;
                vetor intensidade_luz = ponto_luz->intensidade_luz;
            
                if(dynamic_cast<LuzSpot*>(ponto_luz) != NULL){
                    intensidade_luz = dynamic_cast<LuzSpot*>(ponto_luz)->calcular_intensidade_luz_spot(L) * ponto_luz->intensidade_luz;
                }

                auto n_dot_l = produto_vetor(normal/normal.comprimento(),L);

                Raio p_int(p,L);
                bool plano_possui_sombra = Objeto::calcular_objeto_mais_proximo_intersecao(p_int,0.001,1).second != INFINITY;
                if(plano_possui_sombra){continue;}

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
        };

        void transformacao() {
            Matriz M = Matriz::identidade(4);
            for(Matriz m:this->get_transformation()) M = M * m;

            this->ponto_especific = (M * Matriz::vetor_para_matriz(this->ponto_especific ,1)).matriz_para_vetor();  
            this->normal = (M * Matriz::vetor_para_matriz(this->normal, 0)).matriz_para_vetor(); 
            this->normal = this->normal/this->normal.comprimento(); 
            
            this->limpar_transformacao();
        }
        void atualizar_normal() { this->normal = this->normal; }
        void atualizar_normal(Matriz m) {}

    public:
        ponto ponto_especific;
        vetor normal;

};

#endif