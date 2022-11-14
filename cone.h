#ifndef CONE_H
#define CONE_H

#include "vetor.h"
#include "objeto.h"

using namespace std;

class Cone: public Objeto{
    public: 
        Cone(ponto c, vetor d, double h, double r, double especular, vetor k_d, vetor k_e, vetor k_a): centro_base(c), direcao(d), altura(h), raio_base(r), Objeto(k_d,k_e,k_a, especular){}

        pair<double,double> calcular_intersecao(const Raio& r) override{
            auto vertice = centro_base + altura*direcao;
            auto v = vertice - r.origem();
            auto cos2_teta = altura / sqrt(pow(altura,2) + pow(raio_base,2));

            double a = pow(produto_vetor(r.direcao(), direcao), 2) - (produto_vetor(r.direcao(), r.direcao()) *cos2_teta);
            double b = 2*(produto_vetor(v,r.direcao()) * cos2_teta - produto_vetor(v, direcao)*produto_vetor(r.direcao(), direcao));
            double c = pow(produto_vetor(v, direcao), 2) - produto_vetor(v,v) * cos2_teta;

            auto descriminante = b*b - 4*a*c;
            if(descriminante < 0){
                return {INFINITY,INFINITY};
            }

            auto t1 = (-b + sqrt(descriminante)) / (2*a);
            auto t2 = (-b - sqrt(descriminante)) / (2*a);
            double raiz1 = (double) INFINITY;
            double raiz2 = (double) INFINITY;

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

        vetor calcular_intensidade_luz(const Raio& direcao_luz, double raiz_mais_proxima, const luz_pontual& ponto_luz, vetor luz_ambiente) override {
            vetor i = luz_ambiente * k_a;

            ponto p = direcao_luz.origem() + raiz_mais_proxima*direcao_luz.direcao();

            auto vetor_normal_cone = p - centro_base;
            auto normal_cone = vetor_normal_cone/vetor_normal_cone.comprimento();
            auto L = ponto_luz.posicao_ponto()- p;
            auto n_dot_l = produto_vetor(normal_cone,L);

            Raio p_int(p,L);
            bool objeto_possui_sombra = Objeto::calcular_objeto_mais_proximo_intersecao(p_int,0.001,1).second != INFINITY;

            if(objeto_possui_sombra){return luz_ambiente * k_a;}

            if(n_dot_l > 0){
                i += (ponto_luz.intensidade_luz() * k_d) * (n_dot_l  / (normal_cone.comprimento() * L.comprimento())) ;
            }

            if(exp_especular != -1){
                auto R = 2*normal_cone*n_dot_l - L;
                auto r_dot_v = produto_vetor(R,-direcao_luz.direcao());
                if(r_dot_v > 0){
                    i += (ponto_luz.intensidade_luz() * k_e) * pow(r_dot_v/(R.comprimento()*L.comprimento()),exp_especular);
                }
            }

            //Dividir todos pela maior componente de i se alguma componente for maior que 1
            if(i.x() > 1 || i.y() > 1 || i.z() > 1){
                double maior_componente = max( max(i.x(),i.y()) , i.z());
                return i/maior_componente;
            }

            return i;
        }

    public:
        ponto centro_base;
        vetor direcao;
        double altura;
        double raio_base;
};
#endif