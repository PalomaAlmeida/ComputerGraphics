#include <iostream>
#include "cor.h"
#include "vetor.h"
#include "raio.h"
#include <cmath>

using namespace std;

auto posicao_luz = ponto(0,5,0);
auto intensidade_luz = 0.5;
auto intensidade_luz_ambiente = 0.3;

pair<double,double> calcular_raizes(const ponto& centro_esfera, double raio_esfera, const raio& r){
  vetor p = r.origem() - centro_esfera;

  auto a = produto_vetor(r.direcao(),r.direcao());
  auto b = 2.0 * produto_vetor(p, r.direcao());
  auto c = produto_vetor(p, p) - raio_esfera*raio_esfera;

  auto descriminante = b*b - 4*a*c;
  if(descriminante < 0){
    return {INFINITY,INFINITY};
  }

  auto t1 = (-b + sqrt(descriminante)) / (2*a);
  auto t2 = (-b - sqrt(descriminante)) / (2*a);
  return {t1,t2};
}

double calcular_intensidade_difusa_esfera(const raio& direcao_luz, const ponto& centro_esfera, double raiz_mais_proxima){
  double i = 0;

  auto p_int = direcao_luz.origem() + raiz_mais_proxima*direcao_luz.direcao();

  auto normal_esfera = p_int - centro_esfera;
  auto L = posicao_luz - p_int;
  auto n_dot_l = produto_vetor(normal_esfera/normal_esfera.comprimento(),L);

  if(n_dot_l > 0){
    i = intensidade_luz * n_dot_l  / (normal_esfera.comprimento() * L.comprimento()) ;
  }

  return i + intensidade_luz_ambiente;

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

        //Calcula as raizes e verifica se há interseção entre o raio e a esfera
        auto raizes = calcular_raizes(centro_esfera,raio_esfera,r);
        
        if(!isinf(raizes.first) && !isinf(raizes.second)){
          auto intensidade_luz_difusa = calcular_intensidade_difusa_esfera(r,centro_esfera,min(raizes.first,raizes.second));
          cor_pixel = cor(255,0,0) * intensidade_luz_difusa;
        }
        else{
          cor_pixel = cor(100,100,100);
        }
        
        pintar(std::cout, cor_pixel);
      }
    }

}

