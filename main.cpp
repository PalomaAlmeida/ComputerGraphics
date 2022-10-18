#include <iostream>
#include "esfera.h"
#include "cor.h"
#include "vetor.h"
#include "raio.h"
#include "plano.h"
#include "luz_pontual.h"
#include "cilindro.h"
#include "cone.h"
#include <cmath>
#include <list>

using namespace std;

list<Objeto*> Objeto::objetos = list<Objeto*>();

auto posicao_luz1 = ponto(0,0.6,-0.3);
auto intensidade_luz = vetor(0.7,0.7,0.7);
auto intensidade_luz_ambiente = vetor(0.3,0.3,0.3);
luz_pontual luz1(posicao_luz1, intensidade_luz);

list<luz_pontual> luzes;

Cor calcular_cor_pixel(Objeto* objeto_mais_proximo, double raiz_mais_proxima, Raio& r, vetor luz_ambiente){
  Cor cor_pixel;
    
  if(!isinf(raiz_mais_proxima)){
    vetor intensidade_luz_ponto = objeto_mais_proximo->calcular_intensidade_luz(r,raiz_mais_proxima, luz1, luz_ambiente);
    cor_pixel =  Cor(255,255,255) * intensidade_luz_ponto;
  }
  else{
    cor_pixel = Cor(100,100,100);
  }

  return cor_pixel;
}

int main() {
  
    // Qtd pixels (divisão dos quadrados da "tela de mosquito")

    const int largura_imagem = 500;
    const int altura_imagem = 500;
    cout << "P3\n" << largura_imagem << ' ' << altura_imagem << "\n255\n";

    // Tamanho do canvas

    auto hJanela = 0.6;
    auto wJanela = 0.6;
    auto dJanela = 0.3;

    auto Dx = wJanela / (largura_imagem);
    auto Dy = hJanela / (altura_imagem); 

    // Origem (olho do observador)
    auto origem = ponto(0, 0, 0);

    double raio_esfera1 = 0.4;
    auto centro_esfera1 = ponto(0,0,-1);  
    auto centro_topo_cil = centro_esfera1 + (vetor(-1/sqrt(3), 1/sqrt(3), -1/sqrt(3))*(raio_esfera1*3));

    Objeto::objetos.push_back( 
      new Esfera(centro_esfera1, raio_esfera1, vetor(0.7,0.2,0.2), vetor(0.7,0.2,0.2), vetor(0.7,0.2,0.2), 10)
    );
    
    Objeto::objetos.push_back( 
      new Cilindro(centro_esfera1, vetor(-1/sqrt(3), 1/sqrt(3), -1/sqrt(3)), raio_esfera1*3, raio_esfera1/3, vetor(0.2,0.3,0.8),vetor(0.2,0.3,0.8),vetor(0.2,0.3,0.8), 10)
      );
    
    Objeto::objetos.push_back( 
      new Plano(ponto(0,-0.4,0), vetor(0,1,0), vetor(0.2,0.7,0.2), vetor(0,0,0), vetor(0.2,0.7,0.2), 1)
    );

    Objeto::objetos.push_back( 
      new Plano(ponto(0,0,-2), vetor(0,0,1), vetor(0.3,0.3,0.7), vetor(0,0,0),  vetor(0.3,0.3,0.7), 1)
    );

    Objeto::objetos.push_back( 
      new Cone(centro_topo_cil, vetor(-1/sqrt(3), 1/sqrt(3), -1/sqrt(3)), raio_esfera1/3 ,raio_esfera1*1.5, 10, vetor(0.8,0.3,0.2), vetor(0.8,0.3,0.2), vetor(0.8,0.3,0.2))
    );
    
    for (int j = 0; j < altura_imagem; ++j) {
      for (int i = 0; i < largura_imagem; ++i) { 

        //Coordenadas do centro do quadrado da "tela de mosquito" (pixel)
        auto x = -(wJanela / 2) + Dx/2 + i*Dx;
        auto y = (hJanela/2) - Dy/2 - j*Dy;

        //Raio que sai da origem para o pixel
        Raio r(origem, ponto(x,y,-dJanela));

        //Calcula as raizes, verifica se há interseção entre o raio e as esferas e retorna a cor do pixel

        pair<Objeto*,double> objeto_e_raiz_mais_proximas = Objeto::calcular_objeto_mais_proximo_intersecao(r,1,(double) INFINITY);
        Cor cor_pixel = calcular_cor_pixel(objeto_e_raiz_mais_proximas.first, objeto_e_raiz_mais_proximas.second, r, intensidade_luz_ambiente);

        pintar(std::cout, cor_pixel);
      }
    }

}

