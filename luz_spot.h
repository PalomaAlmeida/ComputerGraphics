#ifndef LUZ_SPOT_H
#define LUZ_SPOT_H

#include "vetor.h"
#include <iostream>
#include "raio.h"
#include <math.h>
#include "luz.h"

class LuzSpot: public Luz{
    public:
        vetor direcao;
        int angulo_corte;
        int expoente_luz;
        
    public:
        LuzSpot(){}
        LuzSpot(const ponto& posicao, vetor intensidade) {
            Luz::intensidade_luz = intensidade;
            Luz::posicao_luz = posicao;
        }
        LuzSpot(const ponto& posicao, vetor intensidade, float ang_corte, int expoente_luz, vetor dir): angulo_corte(ang_corte), expoente_luz(expoente_luz), direcao(dir){
            Luz::intensidade_luz = intensidade;
            Luz::posicao_luz = posicao;
        }

        double calcular_intensidade_luz_spot(vetor L) {
            double angulo_corte_radiano = cos((angulo_corte* (M_PI/180)));

            vetor D = -(direcao / direcao.comprimento());
            double cos_a = produto_vetor(D, L/L.comprimento());
            
            if(cos_a >= angulo_corte_radiano){
                return pow(cos_a,expoente_luz);
            }
            else{
                return 0; 
            }
        }
};


#endif
