#ifndef LUZ_AMBIENTE_H
#define LUZ_AMBIENTE_H

#include "vetor.h"
#include "luz.h"


class LuzAmbiente: public Luz{
    public:
        LuzAmbiente(){}
        LuzAmbiente(vetor intensidade){
            Luz::intensidade_luz = intensidade;
        }
};

#endif