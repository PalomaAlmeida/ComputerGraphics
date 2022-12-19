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
#include <vector>
#include "malha.h"
#include "cubo.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "utils.h"

using namespace std;

vector<Objeto*> Objeto::objetos = vector<Objeto*>();

auto posicao_luz1 = ponto(-1,1.4,-0.2);
vector<Luz*> Luz::luzes_pontuais = vector<Luz*>();

Luz* Luz::luz_ambiente = new Luz(vetor(0.3,0.3,0.3));
//auto intensidade_luz = vetor(0.7,0.7,0.7);
//auto intensidade_luz_ambiente = vetor(0.3,0.3,0.3);
// luz_pontual luz1(posicao_luz1, intensidade_luz);

// vector<luz_pontual> luzes;

const double ASPECT_RATIO = 1.0/1.0;

// Qtd pixels (divisão dos quadrados da "tela de mosquito")
const int ALTURA_IMAGEM = 500;
const int LARGURA_IMAGEM = (ALTURA_IMAGEM/ASPECT_RATIO);

int main() {    
    float matrizCores[LARGURA_IMAGEM * ALTURA_IMAGEM * 3];
    
    // Origem (olho do observador)
    auto origem = ponto(0, 0, 0);

    //Vfov (define um "zoom" para a camera) em graus
    double vfov = 90;

    //Camera
    Camera camera = Camera(origem,ponto(0,0,-1),ponto(0,1,0),vfov,2,ASPECT_RATIO);
    
    //Adiciona os objetos ao cenário
    montarObjetosCenarioArvoreNatal();

    //Cria a matriz inicial de pixels
    memcpy(
      matrizCores,
      criar_matriz_pixels(ALTURA_IMAGEM,LARGURA_IMAGEM,camera),
      sizeof(matrizCores)
    );
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

    //Chão
    Objeto::objetos.push_back( 
      new Plano(ponto(0,-1.5,0), vetor(0,1,0), "textures/wood_texture.jpg")
    );

    //Parede lateral direita
    Objeto::objetos.push_back( 
      new Plano(ponto(2,-1.5,0), vetor(-1,0,0), vetor(0.686,0.933,0.933), vetor(0.686,0.933,0.933), vetor(0.686,0.933,0.933), 1)
    );

    //Parede lateral esquerda
    Objeto::objetos.push_back( 
      new Plano(ponto(-2,-1.5,0), vetor(1,0,0), vetor(0.686,0.933,0.933), vetor(0.686,0.933,0.933), vetor(0.686,0.933,0.933), 1)
    );

    //Parede frontal
    Objeto::objetos.push_back( 
      new Plano(ponto(-2,-1.5,-4), vetor(0,0,1), vetor(0.686,0.933,0.933), vetor(0.686,0.933,0.933), vetor(0.686,0.933,0.933), 1)
    );

    //Teto
    Objeto::objetos.push_back( 
      new Plano(ponto(0,1.5,0), vetor(0,-1,0), vetor(0.933,0.933,0.933), vetor(0.686,0.933,0.933), vetor(0.686,0.933,0.933), 1)
    );

    //Cone
    Objeto::objetos.push_back(
      new Cone(ponto(0,-0.6,-2), vetor(0,1,0), 1.5, 0.9, 1, vetor(0,1,0.498), vetor(0,1,0.498), vetor(0,1,0.498))
    );

    //Cilindro
    Objeto::objetos.push_back(
      new Cilindro(ponto(0,-1.5,-2), vetor(0,1,0), 0.9, 0.05, vetor(0.824, 0.706, 0.549), vetor(0.824, 0.706, 0.549), vetor(0.824, 0.706, 0.549), 1)
    );

    // //Cubo
    // Cubo* cubo = new Cubo(0.4, ponto(0,-1.5,-1.65), vetor(1,0.078,0.576),vetor(1,0.078,0.576),vetor(1,0.078,0.567), 100);

    // for(Malha* face: cubo->faces_cubo){
    //   Objeto::objetos.push_back(face);
    // }

    //Esfera
    Objeto::objetos.push_back(
      new Esfera(ponto(0,0.95,-2), 0.05, vetor(0.854, 0.647, 0.125), vetor(0.854, 0.647, 0.125), vetor(0.854, 0.647, 0.125), 1)
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
        Cor cor_pixel = calcular_cor_pixel(objeto_e_raiz_mais_proximas.first, objeto_e_raiz_mais_proximas.second, r, i, j);

        pintar(std::cout, cor_pixel);
      }
    }

}

