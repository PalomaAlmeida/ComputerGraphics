#include <iostream>
#include "cor.h"
#include "vetor.h"
#include "raio.h"
#include "esfera.h"
#include "plano.h"
#include "luz_pontual.h"
#include <cmath>

using namespace std;

auto posicao_luz1 = ponto(0,30,-60);
auto intensidade_luz = 0.7;
luz_pontual luz1(posicao_luz1, intensidade_luz);

auto expoente_especular = 1000;

auto intensidade_luz_plano_chao = 1.0;
luz_pontual luz_chao(posicao_luz1, intensidade_luz_plano_chao);

auto intensidade_luz_plano_fundo = 1.0;
luz_pontual luz_fundo(posicao_luz1, intensidade_luz_plano_fundo);

int main() {
  
    // Qtd pixels (divisão dos quadrados da "tela de mosquito")

    const int largura_imagem = 500;
    const int altura_imagem = 500;
    std::cout << "P3\n" << largura_imagem << ' ' << altura_imagem << "\n255\n";

    // Tamanho do canvas

    auto hJanela = 60.0;
    auto wJanela = 60.0;
    auto dJanela = -30.0;

    auto Dx = wJanela / (largura_imagem);
    auto Dy = hJanela / (altura_imagem); 

    // Origem (olho do observador)
    auto origem = ponto(0, 0, 0);

    double raio_esfera = 40.0;
    auto centro_esfera = -(ponto(0,0,dJanela)+ponto(0,0,-100));  

    ponto pi_planochao = ponto(0, -raio_esfera, 0);
    vetor n_bar_planochao = vetor(0,1,0);

    ponto pi_planofundo = ponto(0, 0, -200);
    vetor n_bar_planofundo = vetor(0,0,1);
  
    for (int j = 0; j < altura_imagem; ++j) {
      for (int i = 0; i < largura_imagem; ++i) { 
        cor cor_pixel;
        cor cor_plano_chao;
        cor cor_plano_fundo;

        //Coordenadas do centro do quadrado da "tela de mosquito" (pixel)
        auto x = -(wJanela / 2) + Dx/2 + i*Dx;
        auto y = (hJanela/2) - Dy/2 - j*Dy;

        //Raio que sai da origem para o pixel
        raio r(origem, ponto(x,y,-dJanela));
        plano plano_chao(pi_planochao, n_bar_planochao);
        plano plano_fundo(pi_planofundo, n_bar_planofundo);
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

        // Verifica se há interseção entre o raio e o plano 
        auto intersecPlanoChao = plano_chao.calcula_intersecao(r);
        auto intersecPlanoFundo = plano_fundo.calcula_intersecao(r);

        if(intersecPlanoChao != NULL){
          auto intens_luz= plano_chao.calcular_luz_plano(r, intersecPlanoChao, luz1, expoente_especular);
          cor_plano_chao = cor(100,100,100)* intensidade_luz_plano_chao;
        }; 

        if(intersecPlanoFundo != NULL){
          auto intens_luz= plano_fundo.calcular_luz_plano(r, intersecPlanoFundo, luz1, expoente_especular);
          cor_plano_fundo = cor(66,66,66)*intensidade_luz_plano_fundo;          
        }; 


        pintar(std::cout, cor_plano_chao);
        pintar(std::cout, cor_plano_fundo);
        pintar(std::cout, cor_pixel);
      }
    }

}

