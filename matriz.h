#ifndef MATRIZ_H
#define MATRIZ_H
#include "vetor.h"
#include <vector>
#include <cmath>
#include <ostream>

class Matriz{
    private:
        int row, col;
        std::vector<std::vector<double>> values;
    public:
        Matriz(){}
        Matriz(int linha, int coluna) : row(linha), col(coluna) { this->values.resize(row, std::vector<double>(col)); }

        Matriz operator * (Matriz m){
            if(this->col != m.row) return Matriz(1, 1);
            Matriz A(this->row, m.col);    

            for(int i = 0; i < this->row; i++)
                for(int j = 0; j < m.col; j++)
                    for(int k = 0; k < m.row; k++)
                        A.values[i][j] += this->values[i][k]*m.values[k][j];
            return A;
        }

        Matriz operator * (double e){
            Matriz A(this-> row, this->col);

            for(int i=0; i< this->col; i++)
                for(int j=0; j< this->row; j++)
                    A.values[i][j] = this->values[i][j]*e;
            return A;
        }

        Matriz operator - (Matriz m){
            if(this->row != m.row || this->col != m.col) return Matriz(1, 1);
            Matriz A(m.row, m.col);
           
            for(int i = 0; i < this->col; i++)
                for(int j = 0; j < this->row; j++)
                    A.values[i][j] = this->values[i][j] - m.values[i][j];
            return A;
        }

        Matriz operator - (){
            Matriz A(this->row, this->col);
            
            for(int i = 0; i < this->col; i++)
                for(int j = 0; j < this->row; j++)
                    A.values[i][j] = -this->values[i][j];
            return A;
        }

        Matriz operator ~ (){
            Matriz A(this->col, this->row);  
            
            for(int i = 0; i < this->col; i++)
                for(int j = 0; j < this->row; j++)
                   A.values[i][j] = this->values[j][i];
            return A; 
        }


        void print();

        vetor Matriz_para_vetor();
        static Matriz vetor_para_Matriz(vetor v, bool ponto = true);

        static Matriz identity(int size);

        static Matriz matriz_rotacao_x(double angle);
        static Matriz matriz_rotacao_y(double angle);
        static Matriz matriz_rotacao_z(double angle);
        static Matriz matriz_translacao(vetor v);
        static Matriz matriz_de_escala(double x, double y, double z);
        static Matriz matriz_shearing_xy(double angle);
        static Matriz matriz_shearing_xz(double angle);
        static Matriz matriz_shearing_yx(double angle);
        static Matriz matriz_shearing_yz(double angle);
        static Matriz matriz_shearing_zx(double angle);
        static Matriz matriz_shearing_zy(double angle);

        static Matriz matriz_reflexao_xy();
        static Matriz matriz_reflexao_yz();
        static Matriz matriz_reflexao_xz();

        static Matriz householder(vetor n);
        static Matriz reflexao_na_matriz(vetor p, vetor n);

        static Matriz matriz_mundo_para_camera(vetor e, vetor at, vetor up);
        static Matriz matriz_camera_para_mundo(vetor e, vetor at, vetor up);

        std::vector<std::vector<double>> get_values();
        void set_values(std::vector<std::vector<double>> v);


        int get_row();
        void set_row(int row);

        int get_col();
        void set_col(int col);

        static Matriz matriz_rotacao_x();
};

#endif 