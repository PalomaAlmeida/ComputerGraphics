#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <list>
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
        virtual double calcular_intensidade_luz(const Raio& direcao_luz, double raiz_mais_proxima, const luz_pontual& ponto_luz, double luz_ambiente) = 0;
        virtual Cor getCor() = 0;
        vetor current_color;
        int m, width, height, channel;
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

        Objeto(const char* fileName, int m){
            this->hasTexture = true;
            int8* image = stbi_load(fileName, &this->width, &this->height, &this->channel, 0);
	        if (image == NULL) {
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
        void set_current_color(int i, int j) { this->current_color = this->image_pixels[i % this->height][j % this->width]; }

    public:
        static list<Objeto*> objetos;
};

#endif