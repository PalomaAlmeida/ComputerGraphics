#include <iostream>
#include "cor.h"
#include "vetor.h"
#include "raio.h"
#include "esfera.h"
#include "luz_pontual.h"
#include <cmath>
#include <list>

using namespace std;

auto posicao_luz1 = ponto(0,5,0);
auto intensidade_luz = 0.7;
auto intensidade_luz_ambiente = 0.1;

luz_pontual luz1(posicao_luz1, intensidade_luz);

list<Esfera> esferas;
list<luz_pontual> luzes;

auto expoente_especular = 1000;

cor calcular_cor_pixel(list<Esfera> esferas, Esfera esfera_mais_proxima, double raiz_mais_proxima, Raio& r, double luz_ambiente){
  cor cor_pixel;
    
  if(!isinf(raiz_mais_proxima)){
    auto intensidade_luz_difusa = esfera_mais_proxima.calcular_intensidade_luz_esfera(esferas, r,raiz_mais_proxima, luz1, expoente_especular, luz_ambiente);
    cor_pixel = cor(255,0,0) * intensidade_luz_difusa;
  }
  else{
    cor_pixel = cor(100,100,100);
  }

  return cor_pixel;
}

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

    double raio_esfera1 = 0.3;
    auto centro_esfera1 = -(ponto(0,0,dJanela)+ponto(0,0,raio_esfera1));  

    double raio_esfera2 = 1.0;
    auto centro_esfera2 = -(ponto(0.5,1.5,dJanela)+ponto(0,0,raio_esfera2));
  
    esferas.push_back(Esfera(centro_esfera1, raio_esfera1));
    esferas.push_back(Esfera(centro_esfera2,raio_esfera2));

    for (int j = 0; j < altura_imagem; ++j) {
      for (int i = 0; i < largura_imagem; ++i) { 

        //Coordenadas do centro do quadrado da "tela de mosquito" (pixel)
        auto x = -(wJanela / 2) + Dx/2 + i*Dx;
        auto y = (hJanela/2) - Dy/2 - j*Dy;

        //Raio que sai da origem para o pixel
        Raio r(origem, ponto(x,y,-dJanela));

        //Calcula as raizes, verifica se há interseção entre o raio e as esferas e retorna a cor do pixel
        pair<Esfera,double> esfera_e_raiz_mais_proximas = Esfera().calcular_raiz_mais_proxima_interseccao(esferas,r,1.0,(double) INFINITY);
        cor cor_pixel = calcular_cor_pixel(esferas,esfera_e_raiz_mais_proximas.first, esfera_e_raiz_mais_proximas.second, r, intensidade_luz_ambiente);

        pintar(std::cout, cor_pixel);
      }
    }

}

