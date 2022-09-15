#include <iostream>
#include "cor.h"
#include "vetor.h"
#include "raio.h"
#include "esfera.h"
#include "luz_pontual.h"
#include <cmath>

using namespace std;

auto posicao_luz1 = ponto(0,5,0);
auto intensidade_luz = 0.7;
luz_pontual luz1(posicao_luz1, intensidade_luz);

auto expoente_especular = 1000;

int main() {
  
    // Qtd pixels (divisão dos quadrados da "tela de mosquito")

    const int largura_imagem = 512;
    const int altura_imagem = 512;
    std::cout << "P3\n" << largura_imagem << ' ' << altura_imagem << "\n255\n";

    // Tamanho do canvas

    auto hJanela = 1.0;
    auto wJanela = 1.0;
    auto dJanela = 1.0;

    auto Dx = wJanela / (largura_imagem);
    auto Dy = hJanela / (altura_imagem); 

    // Origem (olho do observador)
    auto origem = ponto(0, 0, 0);

    double raio_esfera = 0.3;
    auto centro_esfera = -(ponto(0,0,dJanela)+ponto(0,0,raio_esfera));  
  
    for (int j = 0; j < altura_imagem; ++j) {
      for (int i = 0; i < largura_imagem; ++i) { 
        cor cor_pixel;

        //Coordenadas do centro do quadrado da "tela de mosquito" (pixel)
        auto x = -(wJanela / 2) + Dx/2 + i*Dx;
        auto y = (hJanela/2) - Dy/2 - j*Dy;

        //Raio que sai da origem para o pixel
        raio r(origem, ponto(x,y,-dJanela));
        esfera e1(centro_esfera, raio_esfera);

        //Calcula as raizes, verifica se há interseção entre o raio e a esfera e retorna o par de raízes se houver
        auto raizes = e1.calcular_raizes_intersecao(r);
        
        if(!isinf(raizes.first) && !isinf(raizes.second)){
          auto intensidade_luz_difusa = e1.calcular_intensidade_luz_esfera(r,min(raizes.first,raizes.second), luz1, expoente_especular);
          cor_pixel = cor(255,0,0) * intensidade_luz_difusa;
        }
        else{
          cor_pixel = cor(100,100,100);
        }
        
        pintar(std::cout, cor_pixel);
      }
    }

}

