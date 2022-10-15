#ifndef CILINDER_H
#define CILINDER_H

#include "objeto.h"
#include "vetor.h"

class Cilindro: public Objeto{
    public:
        Cilindro(ponto c, vetor d, double h, double r, double especular, Cor cor): centro_base(c), direcao(d), altura(h), raio_base(r), exp_especular(especular), cor(cor){}
        Cor getCor() override {return cor;}

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

        double calcular_intensidade_luz(const Raio& direcao_luz, double raiz_mais_proxima, const luz_pontual& ponto_luz, double luz_ambiente) override {
            double i = 0;

            ponto p = direcao_luz.origem() + raiz_mais_proxima*direcao_luz.direcao();

            auto vetor_normal_cilindro = p - centro_base;
            auto normal_cilindro = vetor_normal_cilindro/vetor_normal_cilindro.comprimento();
            auto L = ponto_luz.posicao_ponto()- p;
            auto n_dot_l = produto_vetor(normal_cilindro,L);

            Raio p_int(p,L);
            bool objeto_possui_sombra = Objeto::calcular_objeto_mais_proximo_intersecao(p_int,0.001,1).second != INFINITY;

            if(objeto_possui_sombra){return luz_ambiente;}

            if(n_dot_l > 0){
                i += ponto_luz.intensidade_luz() * (n_dot_l  / (normal_cilindro.comprimento() * L.comprimento())) ;
            }

            if(exp_especular != -1){
                auto R = 2*normal_cilindro*n_dot_l - L;
                auto r_dot_v = produto_vetor(R,-direcao_luz.direcao());
                if(r_dot_v > 0){
                    i += ponto_luz.intensidade_luz() * pow(r_dot_v/(R.comprimento()*L.comprimento()),exp_especular);
                }
            }

            return min(i + luz_ambiente,1.0);
        }

    public:
        ponto centro_base;
        vetor direcao;
        double altura;
        double raio_base;
        double exp_especular;
        Cor cor;
};


#endif