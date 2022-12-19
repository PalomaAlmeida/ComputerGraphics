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
vector<Luz*> Luz::luzes = vector<Luz*>();

Luz* Luz::luz_ambiente = new Luz();

//Razão entre a altura e largura da tela
const double ASPECT_RATIO = 1.0/1.0;

// Qtd pixels (divisão dos quadrados da "tela de mosquito")
const int LARGURA_IMAGEM = 700;
const int ALTURA_IMAGEM = (LARGURA_IMAGEM/ASPECT_RATIO);

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
    montarObjetosCenarioPraca();

    //Cria a matriz inicial de pixels

    memcpy(
      matrizCores,
      criar_matriz_pixels(ALTURA_IMAGEM,LARGURA_IMAGEM,camera),
      sizeof(matrizCores)
    );

    sdlEngine.atualizarCanvas(matrizCores);
    sdlEngine.atualizarJanela();    

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
        {    
          float look_from_x, look_from_y, look_from_z; 
          float look_at_x, look_at_y, look_at_z;
          float new_vfov;
          float vup_x, vup_y, vup_z;

          cout << "\nOrigem da camera atual (lookfrom): (" << camera.origem.x() << ", " << camera.origem.y() << ", " << camera.origem.z() << ") \n";
          cout << "Destino da camera atual (lookat): (" << camera.destino.x() << ", " << camera.destino.y() << ", " << camera.destino.z() << ") \n";

          cout << "\nSelecione a nova posição da camera (lookfrom)" << "\n";
          cout << "x : " ; cin >> look_from_x;
          cout << "y : " ; cin >> look_from_y;
          cout << "z : " ; cin >> look_from_z;

          cout << "\n Selecione a nova posição de destino (lookat)" << "\n";
          cout << "x : " ; cin >> look_at_x;
          cout << "y : " ; cin >> look_at_y;
          cout << "z : " ; cin >> look_at_z;

          cout << "\nSelecione o novo valor do view up (vup)" << "\n";
          cout << "x : " ; cin >> vup_x;
          cout << "y : " ; cin >> vup_y;
          cout << "z : " ; cin >> vup_z;

          cout << "\nSelecione o valor do vertical field-of-view (vfov) em graus" << "\n";
          cin >> new_vfov;

          camera = Camera(
            ponto(look_from_x,look_from_y,look_from_z),
            ponto(look_at_x,look_at_y,look_at_z),
            vetor(vup_x,vup_y,vup_z),
            new_vfov,
            1,
            ASPECT_RATIO
          );       

          memcpy(
            matrizCores,
            criar_matriz_pixels(ALTURA_IMAGEM,LARGURA_IMAGEM,camera),
            sizeof(matrizCores)
          );

          sdlEngine.atualizarCanvas(matrizCores);
          sdlEngine.atualizarJanela();
          
          break;
        }
        case 2:
        {
          bool event_quit = false;
          cout << "Clique em um objeto na tela para modificá-lo" << "\n";

          while (!event_quit){ 

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
                  cout << "6 - Alterar cor" << "\n";
                  cout << "7 - Voltar" << "\n";
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
                      //transladar_objeto(objeto_e_raiz_mais_proximas.first, x, y, z);
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

                    case 6: 
                      
                      cout << "Digite a intensidade do ka: " << "\n";
                      cin  >> x >> y >> z;
                      //ka(objeto_e_raiz_mais_proximas.first, x, y, z);

                      cout << "Digite a intensidade do kd: " << "\n";
                      cin  >> x >> y >> z;
                      //kd(objeto_e_raiz_mais_proximas.first, x, y, z);

                      cout << "Digite a intensidade do ke: " << "\n";
                      cin  >> x >> y >> z;
                      //ke(objeto_e_raiz_mais_proximas.first, x, y, z);

                      cout << "Cor alterada com sucesso!" << "\n";
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
        }
        case 3:
        {
          int num_lights = 0;
          Luz* luz_selecionada;
          int opcao_luz;
          int opcao_intensidade;

          cout << "Selecione uma das fontes de luz abaixo: " << "\n";

          for(Luz* luz: Luz::luzes){
            cout << num_lights << " - Luz pontual " << "\n";
            num_lights++;
          }
          
          cout << num_lights <<  " - Luz ambiente " << "\n";
          cin >> opcao_luz;
          
          if(opcao_luz < num_lights){
            luz_selecionada = Luz::luzes[opcao_luz];
          } else if (opcao_luz == num_lights){
            luz_selecionada = Luz::luz_ambiente;
          }

          cout << "Selecione uma ação abaixo " << "\n";
          cout << "1 - Apagar luz" << "\n";
          cout << "2 - Modificar intensidade" << "\n";
          cout << "3 - Modificar posicao da luz" << "\n";
          cin >> opcao_intensidade;

          if(opcao_intensidade == 1){
            luz_selecionada->intensidade_luz = vetor(0,0,0);
          }
          else if (opcao_intensidade == 2){
            float int_x, int_y, int_z;
            cout << "Digite o vetor de intensidade da luz (cada valor entre 0 e 1) " << "\n";
            cout << "x: "; cin >> int_x;
            cout << "y: "; cin >> int_y;
            cout << "z: "; cin >> int_z;

            int_x = max(min(int_x,1.0f),0.0f);
            int_y = max(min(int_y,1.0f),0.0f);
            int_z = max(min(int_z,1.0f),0.0f);
            
            luz_selecionada->intensidade_luz = vetor(int_x,int_y,int_z);
            cout << "Luz modificada com sucesso." << "\n";
          }
          else{
            float pos_x, pos_y, pos_z;
            if(dynamic_cast<LuzPontual*>(luz_selecionada) != NULL){
              cout << "\nDigite a nova posicao da luz pontual" << "\n";
              cout << "x: "; cin >> pos_x;
              cout << "y: "; cin >> pos_y;
              cout << "z: "; cin >> pos_z;

              luz_selecionada->posicao_luz = ponto(pos_x,pos_y,pos_z);
            }
            else{
              cout << "\nLuz ambiente não pode ser movida." << "\n";
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
        }
        default:
          quit_scenario = true;
          break;
      }
    }
}

