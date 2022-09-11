#include <iostream>
#include "cor.h"
#include "vetor.h"
#include "raio.h"


bool intersecciona_esfera(const ponto& centro_esfera, double raio_esfera, const raio& r){
  vetor p = r.origem() - centro_esfera;

  auto a = produto_vetor(r.direcao(),r.direcao());
  auto b = 2.0 * produto_vetor(p, r.direcao());
  auto c = produto_vetor(p, p) - raio_esfera*raio_esfera;

  auto descriminante = b*b - 4*a*c;
  return (descriminante > 0);
}


int main() {
  
    const int largura_imagem = 512;
    const int altura_imagem = 512;
    std::cout << "P3\n" << largura_imagem << ' ' << altura_imagem << "\n255\n";

    // Camera

    auto hJanela = 1.0;
    auto wJanela = 1.0;
    auto dJanela = 1.0;

    auto origem = ponto(0, 0, 0);
  
    auto Dx = wJanela / (largura_imagem);
    auto Dy = hJanela / (altura_imagem);   
  
    for (int j = 0; j < altura_imagem; ++j) {
      for (int i = 0; i < largura_imagem; ++i) { 
        cor cor_pixel;

        //Origem e direção aproximada do centro do quadrado da "tela de mosquito" (pixel)
        auto x = -(wJanela / 2) + Dx/2 + i*Dx;
        auto y = (hJanela/2) - Dy/2 - j*Dy;

        raio r(origem, ponto(x,y,-dJanela));
        
        double raio_esfera = 0.3;
        
        if(intersecciona_esfera(-(ponto(0,0,dJanela)+ponto(0,0,raio_esfera)), raio_esfera, r)){
          cor_pixel = cor(255,0,0);
        }
        else{
          cor_pixel = cor(100,100,100);
        }
        pintar(std::cout, cor_pixel);
      }
    }

}

