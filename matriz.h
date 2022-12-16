#ifndef MATRIZ_H
#define MATRIZ_H
#include "vetor.h"
#include <vector>
#include <cmath>
#include <iostream>

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


        void print(){
            for(int i=0; i < row; i++){
                for(int j=0; j < col; j++)
                    std::cout << values[i][j] << ' ';
                std::cout << "\n";
            }
        }

        vetor matriz_para_vetor(){
            vetor v;
            v.x(this->values[0][0]);
            v.y(this->values[1][0]);
            v.z(this->values[2][0]);

            return v;
        }

        static Matriz vetor_para_matriz(vetor v, bool ponto){
            Matriz m(4, 1);
            m.values[0][0] = v.x();
            m.values[1][0] = v.y();
            m.values[2][0] = v.z();
            m.values[3][0] = ponto;
            return m;
        }

        static Matriz identidade(int size){
            Matriz m(size, size);
            for(int i = 0; i < size; i++) {
                for(int j = 0; j < size; j++) {
                    if(i == j) 
                        m.values[i][j] = 1.;
                    else 
                        m.values[i][j] = 0.;
                }
            }
            return m;
        }

        static Matriz matriz_rotacao_x(double angle){
            Matriz m(4,4);
            m.values[0] = {1., 0., 0. , 0.};
            m.values[1] = {0., cos(angle), -sin(angle), 0.};
            m.values[2] = {0., sin(angle), cos(angle), 0.};
            m.values[3] = {0., 0., 0., 1.};

            return m;
        }

        static Matriz matriz_rotacao_y(double angle){
            Matriz m(4,4);
            m.values[0] = {cos(angle), 0., sin(angle) , 0.};
            m.values[1] = {0., 1., 0., 0.};
            m.values[2] = {-sin(angle), 0., cos(angle), 0.};
            m.values[3] = {0., 0., 0., 1.};

            return m;
        }

        static Matriz matriz_rotacao_z(double angle){
            Matriz m(4,4);
            m.values[0] = {cos(angle), -sin(angle), 0., 0.};
            m.values[1] = {sin(angle), cos(angle), 0., 0.};
            m.values[2] = {0., 0., 1., 0.};
            m.values[3] = {0., 0., 0., 1.};

            return m;
        }

        static Matriz matriz_translacao(vetor v){
            Matriz m(4,4);
            m.values[0] = {1., 0., 0., v[0]};
            m.values[1] = {0., 1., 0., v[1]};
            m.values[2] = {0., 0., 1., v[2]};
            m.values[3] = {0., 0., 0., 1.};

            return m;
        }

        static Matriz matriz_de_escala(double x, double y, double z){
            Matriz m(4,4);
            m.values[0] = {x, 0., 0., 0.};
            m.values[1] = {0., y, 0., 0.};
            m.values[2] = {0., 0., z, 0.};
            m.values[3] = {0., 0., 0., 1.};

            return m;
        }

        static Matriz matriz_cisalhamento_xy(double angle){
            Matriz m(4,4);
            m.values[0] = {1., 0., 0., 0.};
            m.values[1] = {tan(angle), 1., 0., 0.};
            m.values[2] = {0., 0., 1., 0.};
            m.values[3] = {0., 0., 0., 1.};

            return m;
        }

        static Matriz matriz_cisalhamento_xz(double angle){
            Matriz m(4,4);
            m.values[0] = {1., 0., 0., 0.};
            m.values[1] = {0., 1., 0., 0.};
            m.values[2] = {tan(angle), 0., 1., 0.};
            m.values[3] = {0., 0., 0., 1.};

            return m;
        }

        static Matriz matriz_cisalhamento_yx(double angle){
            Matriz m(4,4);
            m.values[0] = {1., tan(angle), 0., 0.};
            m.values[1] = {0., 1., 0., 0.};
            m.values[2] = {0., 0., 1., 0.};
            m.values[3] = {0., 0., 0., 1.};

            return m;
        }
        static Matriz matriz_cisalhamento_yz(double angle){
            Matriz m(4,4);
            m.values[0] = {1., 0., 0., 0.};
            m.values[1] = {0., 1., 0., 0.};
            m.values[2] = {0., tan(angle), 1., 0.};
            m.values[3] = {0., 0., 0., 1.};

            return m;
        }
        static Matriz matriz_cisalhamento_zx(double angle){
            Matriz m(4,4);
            m.values[0] = {1., 0., tan(angle), 0.};
            m.values[1] = {0., 1., 0., 0.};
            m.values[2] = {0., 0., 1., 0.};
            m.values[3] = {0., 0., 0., 1.};

            return m;
        }

        static Matriz matriz_cisalhamento_zy(double angle){
            Matriz m(4,4);
            m.values[0] = {1., 0.,  0., 0.};
            m.values[1] = {0., 1., tan(angle), 0.};
            m.values[2] = {0., 0., 1., 0.};
            m.values[3] = {0., 0., 0., 1.};

            return m;
        }

        static Matriz matriz_reflexao_xy(){
            Matriz m(4,4);
            m.values[0] = {1., 0., 0., 0.};
            m.values[1] = {0., 1., 0., 0.};
            m.values[2] = {0., 0., -1., 0.};
            m.values[3] = {0., 0., 0., 1.};

            return m;
        }
        static Matriz matriz_reflexao_yz(){
            Matriz m(4,4);
            m.values[0] = {-1., 0., 0., 0.};
            m.values[1] = {0., 1., 0., 0.};
            m.values[2] = {0., 0., 1., 0.};
            m.values[3] = {0., 0., 0., 1.};

            return m;
        }
        static Matriz matriz_reflexao_xz(){
            Matriz m(4,4);
            m.values[0] = {1., 0., 0., 0.};
            m.values[1] = {0., -1., 0., 0.};
            m.values[2] = {0., 0., 1., 0.};
            m.values[3] = {0., 0., 0., 1.};

            return m;
        }

        static Matriz householder(vetor n){
            Matriz normal = vetor_para_matriz(n, 1);
            Matriz m = identidade(4) - normal * (-normal) * 2;
            m.values[0][3] = m.values[1][3] = m.values[2][3] = m.values[3][0] = m.values[3][1] = m.values[3][2] = 0.;
            m.values[3][3] = 1.;
            return m;
        }

        static Matriz reflexao_na_matriz(vetor p, vetor n){
            return matriz_translacao(p) * householder(n/n.comprimento()) * matriz_translacao(-p);
        }

        static Matriz matriz_mundo_para_camera(vetor e, vetor at, vetor up){
            vetor k = (e - at);
            vetor k_c = k / k.comprimento();
            vetor v_up_k = (up - e) * k_c;
            vetor i = v_up_k / v_up_k.comprimento();
            vetor j = (k * i);

            Matriz m(4,4);
            m.values[0] = {i.x(), i.y(), i.z(), -(produto_vetor(i,e)) }; 
            m.values[1] = {j.x(), j.y(), j.z(), -(produto_vetor(j,e)) };
            m.values[2] = {k_c.x(), k_c.y(), k_c.z(), -(produto_vetor(k,e))};
            m.values[3] = {0., 0., 0., 1.};
            return m;
        }

        static Matriz matriz_camera_para_mundo(vetor e, vetor at, vetor up){
            vetor v_up = up - e;
            vetor k = (e - at);
            vetor k_c = k / k.comprimento();
            vetor i = (v_up * k);
            vetor i_c = i / i.comprimento();
            vetor j = (k * i);

            Matriz m(4,4);
            m.values[0] = {i_c.x(), j.x(), k_c.x(), e.x()};
            m.values[1] = {i_c.y(), j.y(), k_c.y(), e.y()};
            m.values[2] = {i_c.z(), j.z(), k_c.z(), e.z()};
            m.values[3] = {0., 0., 0., 1.};
            return m;
        }

        std::vector<std::vector<double>> get_values();
        void set_values(std::vector<std::vector<double>> v);


        int get_row();
        void set_row(int row);

        int get_col();
        void set_col(int col);

};

#endif 