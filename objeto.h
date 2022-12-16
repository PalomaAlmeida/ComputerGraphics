#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <vector>
#include <cmath>
#include "raio.h"
#include "cor.h"
#include "luz_pontual.h"
#include "vetor.h"
#include "matriz.h"
#include "stb_image.h"

using namespace std;
typedef unsigned char int8;

class Objeto{
    public:
        virtual pair<double, double> calcular_intersecao(const Raio& r) = 0;
        virtual vetor calcular_intensidade_luz(const Raio& direcao_luz, double raiz_mais_proxima) = 0;
        vetor k_d, k_e, k_a;
        ponto centro;
        int exp_especular = -1;
        int width, height, channel;
		bool hasTexture = false;
		std::vector<std::vector<vetor>> image_pixels;
        std::vector<Matriz> transformacoes;
        int inverter = 1;
        virtual void transformacao() = 0;
        virtual void atualizar_normal() = 0;
        virtual void atualizar_normal(Matriz M) = 0;

        static pair<Objeto*, double> calcular_objeto_mais_proximo_intersecao(Raio& r, double t_min, double t_max){
            double raiz_mais_proxima = INFINITY;
            Objeto* objeto_mais_proximo;

            for(auto i= objetos.begin(); i!=objetos.end(); i++){

                auto raizes = (*i)->calcular_intersecao(r);
                    
                if(raizes.first < raiz_mais_proxima && (raizes.first >= t_min && raizes.first < t_max)){
                    raiz_mais_proxima = raizes.first;
                    objeto_mais_proximo = *i;
                }
                if(raizes.second < raiz_mais_proxima && (raizes.second >= t_min && raizes.second < t_max)){
                    raiz_mais_proxima = raizes.second;
                    objeto_mais_proximo = *i;
                }
                    
            }
            return {objeto_mais_proximo, raiz_mais_proxima};
        }

        Objeto(){}

        Objeto(ponto centro, vetor k_d, vetor k_e, vetor k_a): k_a(k_a), k_d(k_d), k_e(k_e){}

        Objeto(ponto centro, vetor k_d, vetor k_e, vetor k_a, int especular): k_a(k_a), k_d(k_d), k_e(k_e), exp_especular(especular){}

        Objeto(const char* fileName){
            this->hasTexture = true;
            int8* image = stbi_load(fileName, &this->width, &this->height, &this->channel, 0);

	        if (image == NULL) {
                cout << "Imagem não encontrada ";
	        	exit(1);
	        }

	        this->image_pixels.resize(this->height, std::vector<vetor>(this->width));

	        double r, g, b;
            for (int i = 0, c = 0; i < this->height; i++) {
                for (int j = 0; j < this->width; j++) {
                    r = 1.0 * image[c++] / 255, g = 1.0 * image[c++] / 255, b = 1.0 * image[c++] / 255;
                    this->image_pixels[i][j] = vetor(r, g, b);
                }
            }
            stbi_image_free(image);
        }

        void set_current_color(int i, int j) { 
            auto image_pixels = this->image_pixels[i % this->height][j % this->width];
            this->k_d = vetor(image_pixels[0], image_pixels[1], image_pixels[2]);
            this->k_e = vetor(image_pixels[0], image_pixels[1], image_pixels[2]);
            this->k_a = vetor(image_pixels[0], image_pixels[1], image_pixels[2]);
        }

        void rotacao_x(double angulo){
            Matriz r_x = Matriz::matriz_rotacao_x(angulo);
            this->transformacoes.push_back(r_x);
            this->transformacao();
            this->atualizar_normal();
        }

        void rotacao_y(double angulo){
            Matriz r_y = Matriz::matriz_rotacao_y(angulo);
            this->transformacoes.push_back(r_y);
            this->transformacao();
            this->atualizar_normal();
        }

        void rotacao_z(double angulo){
            Matriz r_z = Matriz::matriz_rotacao_z(angulo);
            this->transformacoes.push_back(r_z);
            this->transformacao();
            this->atualizar_normal();
        }

        //vetores de tamanho diferentes
        void escalar(double x, double y, double z){
            Matriz escalonamento = Matriz::matriz_de_escala(x, y, z);
            this->transformacoes.push_back(escalonamento);
            this->transformacao();
            if(x != y && x != z && z != y) this->atualizar_normal(escalonamento);
            else this->atualizar_normal();
        }
        //vetores de mesmo tamanho
        void escalar(double tam){
            Matriz escalonamento = Matriz::matriz_de_escala(tam, tam, tam);
            this->transformacoes.push_back(escalonamento);
            this->transformacao();
            this->atualizar_normal();
        }

        void cisalhamento_xy(double angulo){
            Matriz cis = Matriz::matriz_cisalhamento_xy(angulo);
            this->transformacoes.push_back(cis);
            this->transformacao();
            this->atualizar_normal();
        }

        void cisalhamento_xz(double angulo){
            Matriz cis = Matriz::matriz_cisalhamento_xz(angulo);
            this->transformacoes.push_back(cis);
            this->transformacao();
            this->atualizar_normal();
        }
        void cisalhamento_yx(double angulo){
            Matriz cis = Matriz::matriz_cisalhamento_yx(angulo);
            this->transformacoes.push_back(cis);
            this->transformacao();
            this->atualizar_normal();
        }

        void cisalhamento_yz(double angulo){
            Matriz cis = Matriz::matriz_cisalhamento_yz(angulo);
            this->transformacoes.push_back(cis);
            this->transformacao();
            this->atualizar_normal();
        }

        void cisalhamento_zx(double angulo){
            Matriz cis = Matriz::matriz_cisalhamento_zx(angulo);
            this->transformacoes.push_back(cis);
            this->transformacao();
            this->atualizar_normal();
        }

        void cisalhamento_zy(double angulo){
            Matriz cis = Matriz::matriz_cisalhamento_zy(angulo);
            this->transformacoes.push_back(cis);
            this->transformacao();
            this->atualizar_normal();
        }

        void reflexao_xy(){
            this->inverter = this->inverter * -1;
            Matriz reflection = Matriz::matriz_reflexao_xy();
            this->transformacoes.push_back(reflection);  
            this->transformacao();
            this->atualizar_normal();
        }

        void reflexao_yz(){
            this->inverter = this->inverter * -1;
            Matriz reflection = Matriz::matriz_reflexao_yz();
            this->transformacoes.push_back(reflection);  
            this->transformacao();
            this->atualizar_normal();
        }

        void reflexao_xz(){
            this->inverter = this->inverter * -1;
            Matriz reflection = Matriz::matriz_reflexao_xz();
            this->transformacoes.push_back(reflection);  
            this->transformacao();
            this->atualizar_normal();
        }

        void translacao(vetor v){
            Matriz translacao = Matriz::matriz_translacao(v - this-> centro);
            this->transformacoes.push_back(translacao);
            this->transformacao();
        }
        void reflexao(vetor p, vetor n){
            this->inverter = this->inverter * -1;
            Matriz reflexao = Matriz::reflexao_na_matriz(p, n);
            this->transformacoes.push_back(reflexao);
            this->transformacao();
            this->atualizar_normal();
        }
        void mundo_para_camera(Matriz m_c){
            this->transformacoes.push_back(m_c);
            this->transformacao();
            this->atualizar_normal();
        }
        void camera_para_mundo(Matriz c_m){
            this->transformacoes.push_back(c_m);
            this->transformacao();
            this->atualizar_normal();
        }

        void set_transformacao(Matriz m){ 
            this->transformacoes.push_back(m); 
        }

        std::vector<Matriz> get_transformation(){ return this->transformacoes; }
        void limpar_transformacao() { this->transformacoes.clear(); }

        int get_invertida(){
            return this->inverter;
        }

    public:
        static vector<Objeto*> objetos;
};

#endif