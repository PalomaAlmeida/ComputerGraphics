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

//Razão entre a altura e largura da tela
const double ASPECT_RATIO = 1.0/1.0;

// Qtd pixels (divisão dos quadrados da "tela de mosquito")
const int ALTURA_IMAGEM = 500;
const int LARGURA_IMAGEM = (ALTURA_IMAGEM/ASPECT_RATIO);

int main() {

    float matrizCores[LARGURA_IMAGEM * ALTURA_IMAGEM * 3];

    SDLEngine sdlEngine{ "Árvore de Natal"
                    ,LARGURA_IMAGEM, ALTURA_IMAGEM
                    ,LARGURA_IMAGEM, ALTURA_IMAGEM
                  };

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

    sdlEngine.atualizarCanvas(matrizCores);
    sdlEngine.atualizarJanela();    

    //Verifica continuamente sobre eventos na tela
    int opcao_cenario;
    bool quit_scenario = false;
    SDL_Event e;

    cout << "Bem vindo ao cenário!" << "\n";

    while(!quit_scenario){

      cout << "\n" << "Selecione uma das opções abaixo" << "\n";
      cout << "1 - Modificar câmera" << "\n";
      cout << "2 - Modificar objeto" << "\n";
      cout << "3 - Modificar fonte de luz" << "\n";
      cout << "4 - Sair" << "\n";
      cin >> opcao_cenario;

      switch(opcao_cenario){
        case 1:
        
          break;
        case 2:
          bool event_quit = false;
          cout << "Clique em um objeto na tela para modificá-lo" << "\n";

          while (!event_quit){ 
      
            //Atualizar a tela com a matriz de pixels se houver alguma mudança nos objetos do cenário

            while( SDL_PollEvent( &e ) ){

              if( e.type == SDL_QUIT ){
                event_quit = true;
              }

              else if( e.type == SDL_MOUSEBUTTONDOWN){
                int coord_x = e.button.x;
                int coord_y = e.button.y;
                int opcao_acao = 0;
                int opcao_transformacao = 0;
                int angulo = 0;
                double x, y, z = 0;

                Raio r = camera.getRaio(ALTURA_IMAGEM, LARGURA_IMAGEM, coord_x, coord_y);
                pair<Objeto*,double> objeto_e_raiz_mais_proximas = Objeto::calcular_objeto_mais_proximo_intersecao(r,1,(double) INFINITY);

                if(objeto_e_raiz_mais_proximas.first == NULL){
                  cout << "Nenhum objeto foi selecionado." << "\n";
                  continue;
                }
                else{    
                  cout << "\n" << "Você selecionou um objeto do tipo: " << identificar_objeto(objeto_e_raiz_mais_proximas.first) << "\n\n";
                  cout << "Selecione uma das ações abaixo: " << "\n";
                  cout << "1 - Deletar" << "\n";
                  cout << "2 - Escalonar" << "\n";
                  cout << "3 - Rotação" << "\n";
                  cout << "4 - Translação" << "\n";
                  cout << "5 - Cisalhamento" << "\n";
                  cout << "6 - Voltar" << "\n";
                  cin  >> opcao_acao;

                  switch(opcao_acao){
                    case 1:
                      deletar_objeto(objeto_e_raiz_mais_proximas.first);
                      cout << "Deletado com sucesso!" << "\n";
                      break;
                      
                    case 2:
                      cout << "Digite o tamanho de x, y e z desejado para o escalonamento" << "\n";
                      cin >> x >> y >> z;
                        
                      //escalonar_objeto(objeto_e_raiz_mais_proximas.first, x, y, z);
                      cout << "Escalonado com sucesso!" << "\n";
                      break;
                      
                    case 3:
                      cout << "Selecione o eixo de rotação abaixo: " << "\n";
                      cout << "1 - Rotacionar no eixo x" << "\n";
                      cout << "2 - Rotacionar no eixo y" << "\n";
                      cout << "3 - Rotacionar no eixo z" << "\n";
                      cin  >> opcao_transformacao;

                      cout << "Digite o angulo de rotacao" << "\n";
                      cin  >> angulo;
                      
                      //No caso de nao entrar em nenhuma opção, a função nao deve fazer nada
                      //rotacionar_objeto(objeto_e_raiz_mais_proximas.first, angulo, eixo);
                      cout << "Rotacionado com sucesso!" << "\n";
                      break;
                    case 4:
                        
                      cout << "Digite o local de translacao: " << "\n";
                      cout << "x: ";
                      cin  >> x;

                      cout << "y: ";
                      cin  >> y;

                      cout << "z: ";
                      cin  >> z;
                        
                      //TODO incluir chamada a função de translacao
                      cout << "Transladado com sucesso!" << "\n";
                      break;

                    case 5:
                        
                      cout << "Selecione o tipo de cisalhamento: " << "\n";
                      cout << "1 - Cisalhamento XY" << "\n";
                      cout << "2 - Cisalhamento XZ" << "\n";
                      cout << "3 - Cisalhamento YX" << "\n";
                      cout << "4 - Cisalhamento YZ" << "\n";
                      cout << "5 - Cisalhamento ZX" << "\n";
                      cout << "6 - Cisalhamento ZY" << "\n";
                      cin  >> opcao_transformacao;

                      cout << "Digite o angulo de rotacao" << "\n";
                      cin  >> angulo;
                        
                      //No caso de nao entrar em nenhuma opção, a função nao deve fazer nada
                      //cisalhamento_objeto(objeto_e_raiz_mais_proximas.first, angulo, opcao_cis);
                      cout << "Cisalhado com sucesso!" << "\n";
                      
                      break;
                    default:
                      break;
                  }  

                  event_quit = true;  
                }
              }
              
            }
            
          }

          memcpy(
            matrizCores,
            criar_matriz_pixels(ALTURA_IMAGEM,LARGURA_IMAGEM,camera),
            sizeof(matrizCores)
          );

          sdlEngine.atualizarCanvas(matrizCores);
          sdlEngine.atualizarJanela();
        
          break;
        case 3:
          break;
        
        default:
          quit_scenario = true;
          break;
      }
    }
}

