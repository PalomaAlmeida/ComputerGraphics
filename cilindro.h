#ifndef CILINDER_H
#define CILINDER_H

#include "objeto.h"
#include "vetor.h"

class Cilindro: public Objeto{
    public:
        Cilindro(ponto c, vetor d, double h, double r, vetor k_d, vetor k_e, vetor k_a, double especular): centro_base(c), direcao(d), altura(h), raio_base(r), exp_especular(especular), k_a(k_a), k_d(k_d), k_e(k_e){}

        pair<double,double> calcular_intersecao(const Raio& r) override{
            auto v = (r.origem() - centro_base) - (produto_vetor(r.origem() - centro_base, direcao) * direcao);
            auto w = r.direcao() - (produto_vetor(r.direcao(), direcao) * direcao);

            double a = produto_vetor(w,w);
            double b = 2*produto_vetor(v,w);
            double c = produto_vetor(v,v) - (raio_base * raio_base);

            auto descriminante = b*b - 4*a*c;
            if(descriminante < 0){
                return {INFINITY,INFINITY};
            }

            auto t1 = (-b + sqrt(descriminante)) / (2*a);
            auto t2 = (-b - sqrt(descriminante)) / (2*a);
            double raiz1 = (double) INFINITY;
            double raiz2 = (double) INFINITY;
            double raiz3 = (double) INFINITY;
            double raiz4 = (double) INFINITY;

            auto projecao_t1 = produto_vetor((r.origem() + t1 * r.direcao()) - centro_base,direcao);
            auto projecao_t2 = produto_vetor((r.origem() + t2 * r.direcao()) - centro_base,direcao);
            
            if(projecao_t1 <= altura && projecao_t1 >= 0){
                raiz1 = t1;
            }
            if(projecao_t2 <= altura && projecao_t2 >= 0){
                raiz2 = t2;
            }

            return {raiz1,raiz2};
        }

        vetor calcular_intensidade_luz(const Raio& direcao_luz, double raiz_mais_proxima) override {
            vetor i = Luz::luz_ambiente->intensidade_luz * k_a;

            ponto p = direcao_luz.origem() + raiz_mais_proxima*direcao_luz.direcao();

            for(auto ponto_luz: Luz::luzes_pontuais){

                auto vetor_normal_cilindro = p - centro_base;
                auto normal_cilindro = vetor_normal_cilindro/vetor_normal_cilindro.comprimento();
                auto L = ponto_luz->posicao_luz - p;
                auto n_dot_l = produto_vetor(normal_cilindro,L);

                Raio p_int(p,L);
                bool objeto_possui_sombra = Objeto::calcular_objeto_mais_proximo_intersecao(p_int,0.001,1).second != INFINITY;

                if(objeto_possui_sombra){continue;}

                if(n_dot_l > 0){
                    i += (ponto_luz->intensidade_luz * k_d) * (n_dot_l  / (normal_cilindro.comprimento() * L.comprimento()));
                }

                if(exp_especular != -1){
                    auto R = 2*normal_cilindro*n_dot_l - L;
                    auto r_dot_v = produto_vetor(R,-direcao_luz.direcao());
                    if(r_dot_v > 0){
                        i += (ponto_luz->intensidade_luz * k_e) * pow(r_dot_v/(R.comprimento()*L.comprimento()),exp_especular);
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

        void transformacao(){
            Matriz m = Matriz::identidade(4);
            for(Matriz M:this->get_transformation()) m = m * M;

            this->centro = (m * Matriz::vetor_para_matriz(this->centro, true)).matriz_para_vetor();  
            this->direcao = (m * Matriz::vetor_para_matriz(this->direcao, true)).matriz_para_vetor(); 
            this->atualizar_normal();

            this->limpar_transformacao();
        }

        void atualizar_normal() {
            vetor vc = (direcao-this->centro); 
            vetor dc = (vc / vc.comprimento()) * this->get_invertida(); 
            height = vc.comprimento();
        }
        void atualizar_normal(Matriz m) {}

    public:
        ponto centro_base;
        vetor direcao;
        double altura;
        double raio_base;
        double exp_especular;
        vetor k_a;
        vetor k_d;
        vetor k_e;
};


#endif