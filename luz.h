#ifndef LUZ_H
#define LUZ_H

#include "vetor.h"
#include <vector>

using namespace std;

class Luz{
    public:
        vetor posicao_luz;
        vetor intensidade_luz;
        static vector<Luz*> luzes;
        static Luz* luz_ambiente;

    public:
        Luz(){}
        Luz(vetor intensidade): intensidade_luz(intensidade), posicao_luz(vetor(0,0,0)){}
        Luz(vetor posicao, vetor intensidade): posicao_luz(posicao), intensidade_luz(intensidade){}
        virtual ~Luz(){}

};

#endif