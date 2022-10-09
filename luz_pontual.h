#ifndef LUZ_H
#define LUZ_H

#include "vetor.h"

class luz_pontual{
    public:
        ponto posicao;
        double intensidade;
        
    public:
        luz_pontual(){}
        luz_pontual(const ponto& posicao, double intensidade): posicao(posicao), intensidade(intensidade){}

        ponto posicao_ponto() const { return posicao; }
        double intensidade_luz() const { return intensidade; }
};


#endif