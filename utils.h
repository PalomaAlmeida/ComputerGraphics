#include <iostream>
#include "luz_ambiente.h"
#include "objeto.h"
#include "plano.h"
#include "camera.h"
#include "esfera.h"
#include "cilindro.h"
#include "cone.h"
#include "malha.h"
#include "cubo.h"
#include "raio.h"

static void deletar_objeto(Objeto* objeto){
    for(auto i= Objeto::objetos.begin(); i != Objeto::objetos.end(); i++){
        if(*i == objeto){
            Objeto::objetos.erase(i);
            break;
        }
    }
}

static void rotacionar_objetoX(Objeto* objeto, double angulo){
  for(auto i = Objeto::objetos.begin(); i!= Objeto::objetos.end(); i++){
    if(*i == objeto){
      objeto->rotacao_x(angulo);
    }
  }
}

static void rotacionar_objetoY(Objeto* objeto, double angulo){
  for(auto i = Objeto::objetos.begin(); i!= Objeto::objetos.end(); i++){
    if(*i == objeto){
      objeto->rotacao_y(angulo);
    }
  }
}

static void rotacionar_objetoZ(Objeto* objeto, double angulo){
  for(auto i = Objeto::objetos.begin(); i!= Objeto::objetos.end(); i++){
    if(*i == objeto){
      objeto->rotacao_z(angulo);
    }
  }
}

static void escalonar_objeto(Objeto* objeto, double x, double y, double z){
  for(auto i = Objeto::objetos.begin(); i!= Objeto::objetos.end(); i++){
    if(*i == objeto){
      objeto->escalar(x, y, z);
    }
  }
}

static void transladar_objeto(Objeto* objeto, int x, int y, int z){
  for(auto i = Objeto::objetos.begin(); i!= Objeto::objetos.end(); i++){
    if(*i == objeto){
      objeto->translacao(vetor(x, y, z));
    }
  }
}

static void cisalhamento_objeto(Objeto* objeto, double angulo, int opcao_cis){
  for(auto i = Objeto::objetos.begin(); i!= Objeto::objetos.end(); i++){
    if(*i == objeto){
      if(opcao_cis == 1) {objeto->cisalhamento_xy(angulo);}
      else if(opcao_cis == 2) {objeto->cisalhamento_xz(angulo);}
      else if(opcao_cis == 3) {objeto->cisalhamento_yx(angulo);}
      else if(opcao_cis == 4) {objeto->cisalhamento_yz(angulo);}
      else if(opcao_cis == 5) {objeto->cisalhamento_zx(angulo);}
      else if(opcao_cis == 6) {objeto->cisalhamento_zy(angulo);}
    }
  }
}

static string identificar_objeto(Objeto* objeto){
    if(dynamic_cast<Esfera*>(objeto) != NULL){
        return "Esfera";
    }
    else if(dynamic_cast<Plano*>(objeto) != NULL){
        return "Plano";
    }
    else if(dynamic_cast<Cilindro*>(objeto) != NULL){
        return "Cilindro";
    }
    else if(dynamic_cast<Cone*>(objeto) != NULL){
        return "Cone";
    }
    else if(dynamic_cast<Cubo*>(objeto) != NULL){
        return "Cubo";
    }

    return "Objeto";
}

static Cor calcular_cor_pixel(Objeto* objeto_mais_proximo, double raiz_mais_proxima, Raio& r, int i, int j){
  Cor cor_pixel;
    
  if(!isinf(raiz_mais_proxima)){

    if(objeto_mais_proximo->hasTexture) {
      objeto_mais_proximo->set_current_color(i, j);
    }

    vetor intensidade_luz_ponto = objeto_mais_proximo->calcular_intensidade_luz(r,raiz_mais_proxima);
    cor_pixel =  Cor(1.0,1.0,1.0) * intensidade_luz_ponto;
  }
  else{
    cor_pixel = Cor(0.5,0.5,0.5);
  }
  return cor_pixel;
}

static float* criar_matriz_pixels(int altura_imagem, int largura_imagem, Camera camera){
    float* matrizPixels = new float[largura_imagem * altura_imagem * 3];
    int k = 0;

    for (int j = 0; j < altura_imagem; ++j) {
      for (int i = 0; i < largura_imagem; ++i) { 

        //Raio que sai do olho do observador para o pixel
        Raio r = camera.getRaio(altura_imagem, largura_imagem, i , j);

        //Calcula as raizes, verifica se há interseção entre o raio e as esferas e retorna a cor do pixel
        pair<Objeto*,double> objeto_e_raiz_mais_proximas = Objeto::calcular_objeto_mais_proximo_intersecao(r,1,(double) INFINITY);
        Cor cor_pixel = calcular_cor_pixel(objeto_e_raiz_mais_proximas.first, objeto_e_raiz_mais_proximas.second, r, i, j);

        matrizPixels[k] = cor_pixel.r();
        matrizPixels[k+1] = cor_pixel.g();
        matrizPixels[k+2] = cor_pixel.b();
        k += 3;
      }
    }
    return matrizPixels;
}

static void montarObjetosCenarioArvoreNatal(){
        //Luzes do cenário 
    Luz::luz_ambiente = new LuzAmbiente(vetor(0.3,0.3,0.3));

    Luz::luzes_pontuais.push_back(
      new LuzPontual(ponto(1,1,0), vetor(0.7,0.7,0.7))
    );

    //Objetos do mundo  
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

    //Teto
    Objeto::objetos.push_back( 
      new Plano(ponto(0,1.5,0), vetor(0,-1,0), vetor(0.933,0.933,0.933), vetor(0.686,0.933,0.933), vetor(0.686,0.933,0.933), 1)
    );

    //Parede frontal
    Objeto::objetos.push_back( 
      new Plano(ponto(-2,-1.5,-4), vetor(0,0,1), vetor(0.686,0.933,0.933), vetor(0.686,0.933,0.933), vetor(0.686,0.933,0.933), 1)
    );

    //Cone
    Objeto::objetos.push_back(
      new Cone(ponto(0,-0.6,-2), vetor(0,1,0), 1.5, 0.9, 1, vetor(0,1,0.498), vetor(0,1,0.498), vetor(0,1,0.498))
    );

    //Cilindro
    Objeto::objetos.push_back(
      new Cilindro(ponto(0,-1.5,-2), vetor(0,1,0), 0.9, 0.05, vetor(0.824, 0.706, 0.549), vetor(0.824, 0.706, 0.549), vetor(0.824, 0.706, 0.549), 1)
    );

    //Cubo
    Objeto::objetos.push_back(
      new Cubo(0.4, ponto(0,-1.5,-1.65), vetor(1,0.078,0.576),vetor(1,0.078,0.576),vetor(1,0.078,0.567), 100)
    );

    //Esfera
    Objeto::objetos.push_back(
      new Esfera(ponto(0,0.95,-2), 0.05, vetor(0.854, 0.647, 0.125), vetor(0.854, 0.647, 0.125), vetor(0.854, 0.647, 0.125), 1)
    );
}