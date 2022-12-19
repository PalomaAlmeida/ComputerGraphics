#ifndef LUZ_H
#define LUZ_H

#include "vetor.h"
#include <vector>

using namespace std;

class Luz{
    public:
        vetor posicao_luz;
        vetor intensidade_luz;
        static vector<Luz*> luzes_pontuais;
        static Luz* luz_ambiente;

    public:
        Luz(){}
        Luz(vetor intensidade): intensidade_luz(intensidade), posicao_luz(vetor(0,0,0)){}
        Luz(vetor posicao, vetor intensidade): posicao_luz(posicao), intensidade_luz(intensidade){}

        static void alterar_intensidade_luz(Luz luz, vetor intensidade_luz){
            luz.intensidade_luz = intensidade_luz;
        }
};

#endif