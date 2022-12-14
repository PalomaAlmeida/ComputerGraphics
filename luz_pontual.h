#ifndef LUZ_PONTUAL_H
#define LUZ_PONTUAL_H

#include "vetor.h"
#include "luz.h"

class LuzPontual: public Luz{
    public:
        LuzPontual(){}
        LuzPontual(const ponto& posicao, vetor intensidade){
            Luz::posicao_luz = posicao;
            Luz::intensidade_luz = intensidade;
        }
};


#endif
