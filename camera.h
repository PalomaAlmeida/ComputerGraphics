#ifndef CAMERA_H
#define CAMERA_H

#include "vetor.h"
#include "raio.h"
#include <math.h>

class Camera{
    public:
        Camera(
            ponto lookfrom, 
            ponto lookat,
            vetor vup, 
            double vfov,
            double altura_janela, 
            double aspect_ratio
        ): origem(lookfrom){
            
            auto theta = (vfov * M_PI) / 180;
            auto h = tan(theta/2);

            hJanela = altura_janela * h;
            wJanela = hJanela * aspect_ratio;

            w = (lookfrom - lookat) / (lookfrom - lookat).comprimento();
            u = produto_vetorial(vup,w) / produto_vetorial(vup,w).comprimento();
            v = produto_vetorial(w,u);
        }

        Raio getRaio(int altura_imagem, int largura_imagem, int i, int j){

            auto wJanela_new = wJanela * u;
            auto hJanela_new = hJanela * v;

            auto Dx = wJanela_new / (largura_imagem);
            auto Dy = hJanela_new / (altura_imagem); 

            auto x = -(wJanela_new / 2) + Dx/2 + i*Dx;
            auto y = (hJanela_new/2) - Dy/2 - j*Dy;

            return Raio(origem, x + y - w);
        }

    public:
        ponto origem;
        Raio raio_origem;
        double hJanela, wJanela;
    
    private:
        vetor v,u,w;

};

#endif