#ifndef CUBO_H
#define CUBO_H

#include <iostream>
#include <vector>
#include "malha.h"

using namespace std;

class Cubo{
    public:
        Cubo(double aresta, ponto centro_cubo, vetor k_d, vetor k_e, vetor k_a, int exp_especular): aresta(aresta), centro_cubo(centro_cubo), k_d(k_d), k_e(k_e), k_a(k_a), exp_especular(exp_especular){
            //Vértices de baixo do cubo
            ponto v1 = ponto(centro_cubo.x()+aresta/2, centro_cubo.y(), centro_cubo.z()+aresta/2);
            ponto v2 = ponto(centro_cubo.x()+aresta/2, centro_cubo.y(), centro_cubo.z()-aresta/2);
            ponto v3 = ponto(centro_cubo.x()-aresta/2, centro_cubo.y(), centro_cubo.z()+aresta/2);
            ponto v4 = ponto(centro_cubo.x()-aresta/2, centro_cubo.y(), centro_cubo.z()-aresta/2);
            
            //Vértices de cima do cubo
            ponto v5 = ponto(centro_cubo.x()+aresta/2, centro_cubo.y()+aresta, centro_cubo.z()+aresta/2);
            ponto v6 = ponto(centro_cubo.x()+aresta/2, centro_cubo.y()+aresta, centro_cubo.z()-aresta/2);
            ponto v7 = ponto(centro_cubo.x()-aresta/2, centro_cubo.y()+aresta, centro_cubo.z()+aresta/2);
            ponto v8 = ponto(centro_cubo.x()-aresta/2, centro_cubo.y()+aresta, centro_cubo.z()-aresta/2);

            //Faces de baixo do cubo
            faces_cubo.push_back(new Malha(v1,v2,v3,k_d,k_e,k_a,exp_especular));
            faces_cubo.push_back(new Malha(v4,v3,v2,k_d,k_e,k_a,exp_especular));

            //Faces de cima do cubo
            faces_cubo.push_back(new Malha(v5,v6,v7,k_d,k_e,k_a,exp_especular));
            faces_cubo.push_back(new Malha(v8,v7,v6,k_d,k_e,k_a,exp_especular));

            //Face lateral da esquerda do cubo
            faces_cubo.push_back(new Malha(v3,v7,v4,k_d,k_e,k_a,exp_especular));
            faces_cubo.push_back(new Malha(v8,v4,v7,k_d,k_e,k_a,exp_especular));

            //Face lateral da direita do cubo
            faces_cubo.push_back(new Malha(v1,v2,v5,k_d,k_e,k_a,exp_especular));
            faces_cubo.push_back(new Malha(v6,v5,v2,k_d,k_e,k_a,exp_especular));

            //Face de trás do cubo
            faces_cubo.push_back(new Malha(v2,v6,v4,k_d,k_e,k_a,exp_especular));
            faces_cubo.push_back(new Malha(v8,v4,v6,k_d,k_e,k_a,exp_especular));

            //Face da frente do cubo
            faces_cubo.push_back(new Malha(v1,v5,v3,k_d,k_e,k_a,exp_especular));
            faces_cubo.push_back(new Malha(v7,v3,v5,k_d,k_e,k_a,exp_especular));
        }

        Cubo(double aresta, ponto centro_cubo, const char* fileName, int exp_especular): aresta(aresta), centro_cubo(centro_cubo){
            //Vértices de baixo do cubo
            ponto v1 = ponto(centro_cubo.x()+aresta/2, centro_cubo.y(), centro_cubo.z()+aresta/2);
            ponto v2 = ponto(centro_cubo.x()+aresta/2, centro_cubo.y(), centro_cubo.z()-aresta/2);
            ponto v3 = ponto(centro_cubo.x()-aresta/2, centro_cubo.y(), centro_cubo.z()+aresta/2);
            ponto v4 = ponto(centro_cubo.x()-aresta/2, centro_cubo.y(), centro_cubo.z()-aresta/2);
            
            //Vértices de cima do cubo
            ponto v5 = ponto(centro_cubo.x()+aresta/2, centro_cubo.y()+aresta, centro_cubo.z()+aresta/2);
            ponto v6 = ponto(centro_cubo.x()+aresta/2, centro_cubo.y()+aresta, centro_cubo.z()-aresta/2);
            ponto v7 = ponto(centro_cubo.x()-aresta/2, centro_cubo.y()+aresta, centro_cubo.z()+aresta/2);
            ponto v8 = ponto(centro_cubo.x()-aresta/2, centro_cubo.y()+aresta, centro_cubo.z()-aresta/2);

            //Faces de baixo do cubo
            faces_cubo.push_back(new Malha(v1,v2,v3,fileName));
            faces_cubo.push_back(new Malha(v4,v3,v2,fileName));

            //Faces de cima do cubo
            faces_cubo.push_back(new Malha(v5,v6,v7,fileName));
            faces_cubo.push_back(new Malha(v8,v7,v6,fileName));

            //Face lateral da esquerda do cubo
            faces_cubo.push_back(new Malha(v3,v7,v4,fileName));
            faces_cubo.push_back(new Malha(v8,v4,v7,fileName));

            //Face lateral da direita do cubo
            faces_cubo.push_back(new Malha(v1,v2,v5,fileName));
            faces_cubo.push_back(new Malha(v6,v5,v2,fileName));

            //Face de trás do cubo
            faces_cubo.push_back(new Malha(v2,v6,v4,fileName));
            faces_cubo.push_back(new Malha(v8,v4,v6,fileName));

            //Face da frente do cubo
            faces_cubo.push_back(new Malha(v1,v5,v3,fileName));
            faces_cubo.push_back(new Malha(v7,v3,v5,fileName));
        }

        Cubo(){}

    public:
        vector<Malha*> faces_cubo;
        double aresta;
        ponto centro_cubo;
        vetor k_d, k_e, k_a;
        int exp_especular;

};

#endif