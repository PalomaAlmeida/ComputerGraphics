#include <iostream>
#include "utils.h"
#include "cor.h"
#include "vetor.h"
#include "raio.h"
#include <vector>
#include "camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "SDL/SDLEngine.h"

using namespace std;

vector<Objeto*> Objeto::objetos = vector<Objeto*>();
vector<Luz*> Luz::luzes_pontuais = vector<Luz*>();

Luz* Luz::luz_ambiente = new Luz();

int main() {
  
    //Razão entre a altura e largura da tela
    const float aspect_ratio = 1.0/1.0 ;
    
    // Qtd pixels (divisão dos quadrados da "tela de mosquito")
    const int largura_imagem = 500;
    const int altura_imagem = (largura_imagem/aspect_ratio);
    float matrizCores[largura_imagem * altura_imagem * 3];

    SDLEngine sdlEngine{ "Árvore de Natal"
                    ,largura_imagem, altura_imagem
                    ,largura_imagem, altura_imagem
                  };

    // Origem (olho do observador)
    auto origem = ponto(0, 0, 0);

    //Vfov (define um "zoom" para a camera) em graus
    double vfov = 90;

    //Camera
    Camera camera = Camera(origem,ponto(1,0,-1),ponto(0,1,0),vfov,2,aspect_ratio);
    
    //Adiciona os objetos ao cenário
    montarObjetosCenarioArvoreNatal();

    //Cria a matriz inicial de pixels

    memcpy(
      matrizCores,
      criar_matriz_pixels(altura_imagem,largura_imagem,camera),
      sizeof(matrizCores)
    );

    //Verifica continuamente sobre eventos na tela
    
    SDL_Event e;
    bool quit = false;
    bool matrizChanged = true;
    
    while (!quit){ 
      
      //Atualizar a tela com a matriz de pixels se houver alguma mudança nos objetos do cenário

      if(matrizChanged){
        sdlEngine.atualizarCanvas(matrizCores);
        sdlEngine.atualizarJanela();
        matrizChanged = false;
      }

      while( SDL_PollEvent( &e ) ){

        if( e.type == SDL_QUIT ){
          quit = true;
        }

        else if( e.type == SDL_MOUSEBUTTONDOWN){
          int x = e.button.x;
          int y = e.button.y;

          Raio r = camera.getRaio(altura_imagem, largura_imagem, x, y);
          pair<Objeto*,double> objeto_e_raiz_mais_proximas = Objeto::calcular_objeto_mais_proximo_intersecao(r,1,(double) INFINITY);
            
          int opcao = 0;
            
          cout << "\n" << "Você selecionou um objeto do tipo: " << identificar_objeto(objeto_e_raiz_mais_proximas.first) << "\n\n";
          cout << "Selecione uma das ações abaixo: " << "\n";
          cout << "1 - Deletar" << "\n";
          cout << "2 - Movimentar" << "\n";
          cout << "3 - Rotacionar" << "\n";
          cout << "4 - Cancelar" << "\n";
          cin  >> opcao;
            
          switch(opcao){
            case 1:
              deletar_objeto(objeto_e_raiz_mais_proximas.first);
      
              memcpy(
                matrizCores,
                criar_matriz_pixels(altura_imagem,largura_imagem,camera),
                sizeof(matrizCores)
              );

              matrizChanged = true;
              break;
            default:
              cout << "\n" << "Ação cancelada." << "\n";
              break;
          }              
        }
      }
    }

}

