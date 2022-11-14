#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <vector>
#include <cmath>
#include <vector>
#include "raio.h"
#include "cor.h"
#include "luz_pontual.h"
#include "vetor.h"
#include "stb_image.h"

using namespace std;
typedef unsigned char int8;

class Objeto{
    public:
        virtual pair<double, double> calcular_intersecao(const Raio& r) = 0;
        virtual vetor calcular_intensidade_luz(const Raio& direcao_luz, double raiz_mais_proxima, const luz_pontual& ponto_luz, vetor luz_ambiente) = 0;
        vetor k_d, k_e, k_a;
        int exp_especular = -1;
        int width, height, channel;
		bool hasTexture = false;
		std::vector<std::vector<vetor>> image_pixels;

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

        Objeto(vetor k_d, vetor k_e, vetor k_a): k_a(k_a), k_d(k_d), k_e(k_e){}

        Objeto(vetor k_d, vetor k_e, vetor k_a, int especular): k_a(k_a), k_d(k_d), k_e(k_e), exp_especular(especular){}

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

    public:
        static vector<Objeto*> objetos;
};

#endif