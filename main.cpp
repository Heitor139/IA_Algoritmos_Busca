#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include "create_map.h"
#include "heuristics.h"
#include "greedy_search.h"
#include "a_search.h"
#include "hill_climbing.h"


using namespace std;

int main() {
    vector<vector<int>> matrix(15, vector<int> (15));
    double obstacles = 0;
    int saida_gerados = 0;
    int saida_visitados = 0;
    int saida_custo = 0;
    bool saida_achou = false;

    ofstream output1("q1_output.txt");

    if(!output1.is_open()) {
        cout << "Falha em abrir o arquivo" << endl;
        return 1;
    }

    random_device rd; 
    mt19937 gen(rd()); 
    uniform_int_distribution<> distr1(3, 6); 
    uniform_int_distribution<> distr2(0, 14);

    for(int i = 0; i < 15; i++) {

        pair<int,int> start;
        pair<int,int> finish;

        do {
            for(int i = 0; i < 15; i++) {
                for(int j = 0; j < 15; j++) {
                    int random_num = distr1(gen); 
                    matrix[i][j] = random_num;
                }
            }

            for(int i = 0; i < obstacles; i++) {
                pair<int, int> block({distr2(gen), distr2(gen)});
                while(matrix[block.first][block.second] == -1) block = make_pair(distr2(gen), distr2(gen));
                matrix[block.first][block.second] = -1;
            }

            start = make_pair(distr2(gen), distr2(gen));
            finish = make_pair(distr2(gen), distr2(gen));
            while(matrix[start.first][start.second] == -1) start = make_pair(distr2(gen), distr2(gen));
            while(matrix[finish.first][finish.second] == -1 || start == finish) finish = make_pair(distr2(gen), distr2(gen));
        } while(!isSolvable(matrix, start, finish));

        writeMap(matrix, output1);
        output1 << "start: (" << start.first << ", " << start.second << ")   finish: (" << finish.first << ", " << finish.second << ")" << endl << endl;

        subidaEncostaGulosa(matrix, start, finish, distEuclid, output1);
        subidaEncostaGulosa(matrix, start, finish, distMan, output1);
        
        subidaEncostaMaiorAclive(matrix, start, finish, distEuclid, output1, saida_gerados, saida_visitados, saida_custo, saida_achou);
        subidaEncostaMaiorAclive(matrix, start, finish, distMan, output1, saida_gerados, saida_visitados, saida_custo, saida_achou);
        
        subidaEncostaEstocastico(matrix, start, finish, distEuclid, output1); 
        subidaEncostaEstocastico(matrix, start, finish, distMan, output1); 

    }

    output1.close();


    ofstream output2("q2_output.txt");
    if(!output2.is_open()) {
        cout << "Falha em abrir o arquivo" << endl;
        return 1;
    }

    for(int i = 0; i < 20; i++) {

        pair<int,int> start;
        pair<int,int> finish;

        do {
            for(int i = 0; i < 15; i++) {
                for(int j = 0; j < 15; j++) {
                    int random_num = distr1(gen); 
                    matrix[i][j] = random_num;
                }
            }

            for(int i = 0; i < obstacles; i++) {
                pair<int, int> block({distr2(gen), distr2(gen)});
                while(matrix[block.first][block.second] == -1) block = make_pair(distr2(gen), distr2(gen));
                matrix[block.first][block.second] = -1;
            }

            start = make_pair(distr2(gen), distr2(gen));
            finish = make_pair(distr2(gen), distr2(gen));
            while(matrix[start.first][start.second] == -1) start = make_pair(distr2(gen), distr2(gen));
            while(matrix[finish.first][finish.second] == -1 || start == finish) finish = make_pair(distr2(gen), distr2(gen));
        } while(!isSolvable(matrix, start, finish));

        writeMap(matrix, output2);
        output2 << "start: (" << start.first << ", " << start.second << ")   finish: (" << finish.first << ", " << finish.second << ")" << endl << endl;
        buscaGulosa(matrix, start, finish, distEuclid, output2, saida_gerados, saida_visitados, saida_custo, saida_achou);
        buscaGulosa(matrix, start, finish, distMan, output2, saida_gerados, saida_visitados, saida_custo, saida_achou);
       
        aStar(matrix, start, finish, distEuclid, output2, saida_gerados, saida_visitados, saida_custo, saida_achou);
        aStar(matrix, start, finish, distMan, output2, saida_gerados, saida_visitados, saida_custo, saida_achou);
    }

    output2.close();

    obstacles = round(225*0.20);

    ofstream output3("q3_output.txt");
    if(!output3.is_open()) {
        cout << "Falha em abrir o arquivo" << endl;
        return 1;
    }


    for(int i = 0; i < 20; i++) {

        pair<int,int> start;
        pair<int,int> finish;

        do {
            for(int i = 0; i < 15; i++) {
                for(int j = 0; j < 15; j++) {
                    int random_num = distr1(gen); 
                    matrix[i][j] = random_num;
                }
            }

            for(int i = 0; i < obstacles; i++) {
                pair<int, int> block({distr2(gen), distr2(gen)});
                while(matrix[block.first][block.second] == -1) block = make_pair(distr2(gen), distr2(gen));
                matrix[block.first][block.second] = -1;
            }

            start = make_pair(distr2(gen), distr2(gen));
            finish = make_pair(distr2(gen), distr2(gen));
            while(matrix[start.first][start.second] == -1) start = make_pair(distr2(gen), distr2(gen));
            while(matrix[finish.first][finish.second] == -1 || start == finish) finish = make_pair(distr2(gen), distr2(gen));
        } while(!isSolvable(matrix, start, finish));

        writeMap(matrix, output3);
        output3 << "start: (" << start.first << ", " << start.second << ")   finish: (" << finish.first << ", " << finish.second << ")" << endl << endl;
        weightedAStar(matrix, start, finish, distEuclid, 1, output3);
        weightedAStar(matrix, start, finish, distEuclid, 3, output3);
        weightedAStar(matrix, start, finish, distEuclid, 6, output3);
       
        weightedAStar(matrix, start, finish, distMan, 1, output3);
        weightedAStar(matrix, start, finish, distMan, 3, output3);
        weightedAStar(matrix, start, finish, distMan, 6, output3);
    }

    output3.close();

    ofstream output4("q4_output.txt");
    if(!output4.is_open()) {
        cout << "Falha em abrir o arquivo" << endl;
        return 1;
    }


    for(int o = 1; o <= 4; o++) {
        obstacles = round(225*0.1*o);
        int pct_atual = o * 10;
        double total_sde_gerados = 0, total_sde_visitados = 0, total_sde_custo = 0;
        int total_sde_sucessos = 0;
        double total_gulosa_gerados = 0, total_gulosa_visitados = 0, total_gulosa_custo = 0;
        int total_gulosa_sucessos = 0;
        double total_ast_gerados = 0, total_ast_visitados = 0, total_ast_custo = 0;
        int total_ast_sucessos = 0;
        for(int i = 0; i < 20; i++) {

            pair<int,int> start;
            pair<int,int> finish;

            do {
                for(int i = 0; i < 15; i++) {
                    for(int j = 0; j < 15; j++) {
                        int random_num = distr1(gen); 
                        matrix[i][j] = random_num;
                    }
                }

                for(int i = 0; i < obstacles; i++) {
                    pair<int, int> block({distr2(gen), distr2(gen)});
                    while(matrix[block.first][block.second] == -1) block = make_pair(distr2(gen), distr2(gen));
                    matrix[block.first][block.second] = -1;
                }

                start = make_pair(distr2(gen), distr2(gen));
                finish = make_pair(distr2(gen), distr2(gen));
                while(matrix[start.first][start.second] == -1) start = make_pair(distr2(gen), distr2(gen));
                while(matrix[finish.first][finish.second] == -1 || start == finish) finish = make_pair(distr2(gen), distr2(gen));
            } while(!isSolvable(matrix, start, finish));

            writeMap(matrix, output4);
            output4 << "start: (" << start.first << ", " << start.second << ")   finish: (" << finish.first << ", " << finish.second << ")" << "  obstacle percentage: " << (o*10) << "%" << endl << endl;

            subidaEncostaMaiorAclive(matrix, start, finish, distMan, output4, saida_gerados, saida_visitados, saida_custo, saida_achou);
            total_sde_gerados += saida_gerados;
            total_sde_visitados += saida_visitados;
            total_sde_custo += saida_custo;
            if(saida_achou) total_sde_sucessos++;

            buscaGulosa(matrix, start, finish, distMan, output4, saida_gerados, saida_visitados, saida_custo, saida_achou);
            total_gulosa_gerados += saida_gerados;
            total_gulosa_visitados += saida_visitados;
            total_gulosa_custo += saida_custo;
            if(saida_achou) total_gulosa_sucessos++;

            aStar(matrix, start, finish, distMan, output4, saida_gerados, saida_visitados, saida_custo, saida_achou);
            total_ast_gerados += saida_gerados;
            total_ast_visitados += saida_visitados;
            total_ast_custo += saida_custo;
            if(saida_achou) total_ast_sucessos++;
        }

        output4 << "\nDensidade de obstaculos" << pct_atual << "%" << endl;
        output4 << "SdE Maior Aclive - media gerados: " << (total_sde_gerados / 20.0) << ", media visitados: " << (total_sde_visitados / 20.0) << ", media custo: " << (total_sde_custo / 20.0) << ", taxa sucesso: " << (total_sde_sucessos / 20.0) * 100.0 << "%" << endl;
        output4 << "Busca Gulosa - media gerados: " << (total_gulosa_gerados / 20.0) << ", media visitados: " << (total_gulosa_visitados / 20.0) << ", media custo: " << (total_gulosa_custo / 20.0) << ", taxa sucesso: " << (total_gulosa_sucessos / 20.0) * 100.0 << "%" << endl;
        output4 << "A* - media gerados: " << (total_ast_gerados / 20.0) << ", media visitados: " << (total_ast_visitados / 20.0) << ", media custo: " << (total_ast_custo / 20.0) << ", taxa sucesso: " << (total_ast_sucessos / 20.0) * 100.0 << "%" << endl << endl;
    }

    return 0;
}