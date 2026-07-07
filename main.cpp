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
    vector<vector<int>> matrix(15, vector<int> (15)); //Inicialização do grid
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

    double total_sde_gul_eu_gerados = 0, total_sde_gul_eu_visitados = 0, total_sde_gul_eu_custo = 0;
    int total_sde_gul_eu_sucessos = 0;
    double total_sde_gul_man_gerados = 0, total_sde_gul_man_visitados = 0, total_sde_gul_man_custo = 0;
    int total_sde_gul_man_sucessos = 0;
    double total_sde_acl_eu_gerados = 0, total_sde_acl_eu_visitados = 0, total_sde_acl_eu_custo = 0;
    int total_sde_acl_eu_sucessos = 0;
    double total_sde_acl_man_gerados = 0, total_sde_acl_man_visitados = 0, total_sde_acl_man_custo = 0;
    int total_sde_acl_man_sucessos = 0;
    double total_sde_est_eu_gerados = 0, total_sde_est_eu_visitados = 0, total_sde_est_eu_custo = 0;
    int total_sde_est_eu_sucessos = 0;
    double total_sde_est_man_gerados = 0, total_sde_est_man_visitados = 0, total_sde_est_man_custo = 0;
    int total_sde_est_man_sucessos = 0;

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

        output1 << "=== Entrada " << i+1 << " ===" << endl;
        writeMap(matrix, output1);
        output1 << "start: (" << start.first << ", " << start.second << ")   finish: (" << finish.first << ", " << finish.second << ")" << endl << endl;

        subidaEncostaGulosa(matrix, start, finish, distEuclid, output1, saida_gerados, saida_visitados, saida_custo, saida_achou);
        total_sde_gul_eu_gerados += saida_gerados; total_sde_gul_eu_visitados += saida_visitados;
        total_sde_gul_eu_custo += saida_custo; if(saida_achou) total_sde_gul_eu_sucessos++;

        subidaEncostaGulosa(matrix, start, finish, distMan, output1, saida_gerados, saida_visitados, saida_custo, saida_achou);
        total_sde_gul_man_gerados += saida_gerados; total_sde_gul_man_visitados += saida_visitados;
        total_sde_gul_man_custo += saida_custo; if(saida_achou) total_sde_gul_man_sucessos++;

        subidaEncostaMaiorAclive(matrix, start, finish, distEuclid, output1, saida_gerados, saida_visitados, saida_custo, saida_achou);
        total_sde_acl_eu_gerados += saida_gerados; total_sde_acl_eu_visitados += saida_visitados;
        total_sde_acl_eu_custo += saida_custo; if(saida_achou) total_sde_acl_eu_sucessos++;

        subidaEncostaMaiorAclive(matrix, start, finish, distMan, output1, saida_gerados, saida_visitados, saida_custo, saida_achou);
        total_sde_acl_man_gerados += saida_gerados; total_sde_acl_man_visitados += saida_visitados;
        total_sde_acl_man_custo += saida_custo; if(saida_achou) total_sde_acl_man_sucessos++;

        subidaEncostaEstocastico(matrix, start, finish, distEuclid, output1, saida_gerados, saida_visitados, saida_custo, saida_achou);
        total_sde_est_eu_gerados += saida_gerados; total_sde_est_eu_visitados += saida_visitados;
        total_sde_est_eu_custo += saida_custo; if(saida_achou) total_sde_est_eu_sucessos++;

        subidaEncostaEstocastico(matrix, start, finish, distMan, output1, saida_gerados, saida_visitados, saida_custo, saida_achou);
        total_sde_est_man_gerados += saida_gerados; total_sde_est_man_visitados += saida_visitados;
        total_sde_est_man_custo += saida_custo; if(saida_achou) total_sde_est_man_sucessos++;

        output1 << endl;
    }

    output1 << "========================================" << endl;
    output1 << "MEDIAS - EXPERIMENTO 1 (15 entradas)" << endl;
    output1 << "========================================" << endl << endl;

    output1 << "SdE Gulosa Deterministica - Euclidiana" << endl;
    output1 << "  Media gerados:   " << total_sde_gul_eu_gerados / 15.0   << endl;
    output1 << "  Media visitados: " << total_sde_gul_eu_visitados / 15.0 << endl;
    output1 << "  Media custo:     " << total_sde_gul_eu_custo / 15.0     << endl;
    output1 << "  Taxa sucesso:    " << (total_sde_gul_eu_sucessos / 15.0) * 100.0 << "%" << endl << endl;

    output1 << "SdE Gulosa Deterministica - Manhattan" << endl;
    output1 << "  Media gerados:   " << total_sde_gul_man_gerados / 15.0   << endl;
    output1 << "  Media visitados: " << total_sde_gul_man_visitados / 15.0 << endl;
    output1 << "  Media custo:     " << total_sde_gul_man_custo / 15.0     << endl;
    output1 << "  Taxa sucesso:    " << (total_sde_gul_man_sucessos / 15.0) * 100.0 << "%" << endl << endl;

    output1 << "SdE Maior Aclive - Euclidiana" << endl;
    output1 << "  Media gerados:   " << total_sde_acl_eu_gerados / 15.0   << endl;
    output1 << "  Media visitados: " << total_sde_acl_eu_visitados / 15.0 << endl;
    output1 << "  Media custo:     " << total_sde_acl_eu_custo / 15.0     << endl;
    output1 << "  Taxa sucesso:    " << (total_sde_acl_eu_sucessos / 15.0) * 100.0 << "%" << endl << endl;

    output1 << "SdE Maior Aclive - Manhattan" << endl;
    output1 << "  Media gerados:   " << total_sde_acl_man_gerados / 15.0   << endl;
    output1 << "  Media visitados: " << total_sde_acl_man_visitados / 15.0 << endl;
    output1 << "  Media custo:     " << total_sde_acl_man_custo / 15.0     << endl;
    output1 << "  Taxa sucesso:    " << (total_sde_acl_man_sucessos / 15.0) * 100.0 << "%" << endl << endl;

    output1 << "SdE Estocastica - Euclidiana" << endl;
    output1 << "  Media gerados:   " << total_sde_est_eu_gerados / 15.0   << endl;
    output1 << "  Media visitados: " << total_sde_est_eu_visitados / 15.0 << endl;
    output1 << "  Media custo:     " << total_sde_est_eu_custo / 15.0     << endl;
    output1 << "  Taxa sucesso:    " << (total_sde_est_eu_sucessos / 15.0) * 100.0 << "%" << endl << endl;

    output1 << "SdE Estocastica - Manhattan" << endl;
    output1 << "  Media gerados:   " << total_sde_est_man_gerados / 15.0   << endl;
    output1 << "  Media visitados: " << total_sde_est_man_visitados / 15.0 << endl;
    output1 << "  Media custo:     " << total_sde_est_man_custo / 15.0     << endl;
    output1 << "  Taxa sucesso:    " << (total_sde_est_man_sucessos / 15.0) * 100.0 << "%" << endl << endl;

    output1.close();

    ofstream output2("q2_output.txt");
    if(!output2.is_open()) {
        cout << "Falha em abrir o arquivo" << endl;
        return 1;
    }

    double total_gulosa_eu_gerados = 0, total_gulosa_eu_visitados = 0, total_gulosa_eu_custo = 0;
    int total_gulosa_eu_sucessos = 0;
    double total_gulosa_man_gerados = 0, total_gulosa_man_visitados = 0, total_gulosa_man_custo = 0;
    int total_gulosa_man_sucessos = 0;
    double total_ast_eu_gerados = 0, total_ast_eu_visitados = 0, total_ast_eu_custo = 0;
    int total_ast_eu_sucessos = 0;
    double total_ast_man_gerados = 0, total_ast_man_visitados = 0, total_ast_man_custo = 0;
    int total_ast_man_sucessos = 0;

    for(int i = 0; i < 20; i++) {
        pair<int,int> start, finish;
        do {
            for(int i = 0; i < 15; i++)
                for(int j = 0; j < 15; j++)
                    matrix[i][j] = distr1(gen);
            start = make_pair(distr2(gen), distr2(gen));
            finish = make_pair(distr2(gen), distr2(gen));
            while(matrix[start.first][start.second] == -1) start = make_pair(distr2(gen), distr2(gen));
            while(matrix[finish.first][finish.second] == -1 || start == finish) finish = make_pair(distr2(gen), distr2(gen));
        } while(!isSolvable(matrix, start, finish));

        output2 << "=== Entrada " << i+1 << " ===" << endl;
        writeMap(matrix, output2);
        output2 << "start: (" << start.first << ", " << start.second << ")   finish: (" << finish.first << ", " << finish.second << ")" << endl << endl;

        buscaGulosa(matrix, start, finish, distEuclid, output2, saida_gerados, saida_visitados, saida_custo, saida_achou);
        total_gulosa_eu_gerados += saida_gerados; total_gulosa_eu_visitados += saida_visitados;
        total_gulosa_eu_custo += saida_custo; if(saida_achou) total_gulosa_eu_sucessos++;

        buscaGulosa(matrix, start, finish, distMan, output2, saida_gerados, saida_visitados, saida_custo, saida_achou);
        total_gulosa_man_gerados += saida_gerados; total_gulosa_man_visitados += saida_visitados;
        total_gulosa_man_custo += saida_custo; if(saida_achou) total_gulosa_man_sucessos++;

        aStar(matrix, start, finish, distEuclid, output2, saida_gerados, saida_visitados, saida_custo, saida_achou);
        total_ast_eu_gerados += saida_gerados; total_ast_eu_visitados += saida_visitados;
        total_ast_eu_custo += saida_custo; if(saida_achou) total_ast_eu_sucessos++;

        aStar(matrix, start, finish, distMan, output2, saida_gerados, saida_visitados, saida_custo, saida_achou);
        total_ast_man_gerados += saida_gerados; total_ast_man_visitados += saida_visitados;
        total_ast_man_custo += saida_custo; if(saida_achou) total_ast_man_sucessos++;

        output2 << endl;
    }

    output2 << "========================================" << endl;
    output2 << "MEDIAS - EXPERIMENTO 2 (20 entradas)" << endl;
    output2 << "========================================" << endl << endl;

    output2 << "Busca Gulosa - Euclidiana" << endl;
    output2 << "  Media gerados:   " << total_gulosa_eu_gerados / 20.0   << endl;
    output2 << "  Media visitados: " << total_gulosa_eu_visitados / 20.0 << endl;
    output2 << "  Media custo:     " << total_gulosa_eu_custo / 20.0     << endl;
    output2 << "  Taxa sucesso:    " << (total_gulosa_eu_sucessos / 20.0) * 100.0 << "%" << endl << endl;

    output2 << "Busca Gulosa - Manhattan" << endl;
    output2 << "  Media gerados:   " << total_gulosa_man_gerados / 20.0   << endl;
    output2 << "  Media visitados: " << total_gulosa_man_visitados / 20.0 << endl;
    output2 << "  Media custo:     " << total_gulosa_man_custo / 20.0     << endl;
    output2 << "  Taxa sucesso:    " << (total_gulosa_man_sucessos / 20.0) * 100.0 << "%" << endl << endl;

    output2 << "A* - Euclidiana" << endl;
    output2 << "  Media gerados:   " << total_ast_eu_gerados / 20.0   << endl;
    output2 << "  Media visitados: " << total_ast_eu_visitados / 20.0 << endl;
    output2 << "  Media custo:     " << total_ast_eu_custo / 20.0     << endl;
    output2 << "  Taxa sucesso:    " << (total_ast_eu_sucessos / 20.0) * 100.0 << "%" << endl << endl;

    output2 << "A* - Manhattan" << endl;
    output2 << "  Media gerados:   " << total_ast_man_gerados / 20.0   << endl;
    output2 << "  Media visitados: " << total_ast_man_visitados / 20.0 << endl;
    output2 << "  Media custo:     " << total_ast_man_custo / 20.0     << endl;
    output2 << "  Taxa sucesso:    " << (total_ast_man_sucessos / 20.0) * 100.0 << "%" << endl << endl;

    output2.close();

    obstacles = round(225*0.20);

    ofstream output3("q3_output.txt");
    if(!output3.is_open()) {
        cout << "Falha em abrir o arquivo" << endl;
        return 1;
    }

    double total_h1_gerados = 0, total_h1_visitados = 0, total_h1_custo = 0;
    int total_h1_sucessos = 0;
    double total_h2_gerados = 0, total_h2_visitados = 0, total_h2_custo = 0;
    int total_h2_sucessos = 0;
    double total_h3_gerados = 0, total_h3_visitados = 0, total_h3_custo = 0;
    int total_h3_sucessos = 0;
    double total_h4_gerados = 0, total_h4_visitados = 0, total_h4_custo = 0;
    int total_h4_sucessos = 0;
    double total_h5_gerados = 0, total_h5_visitados = 0, total_h5_custo = 0;
    int total_h5_sucessos = 0;
    double total_h6_gerados = 0, total_h6_visitados = 0, total_h6_custo = 0;
    int total_h6_sucessos = 0;

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

        output3 << "=== Entrada " << i+1 << " ===" << endl;
        writeMap(matrix, output3);
        output3 << "start: (" << start.first << ", " << start.second << ")   finish: (" << finish.first << ", " << finish.second << ")" << endl << endl;

        weightedAStar(matrix, start, finish, distEuclid, 1, output3, saida_gerados, saida_visitados, saida_custo, saida_achou);
        total_h1_gerados += saida_gerados; total_h1_visitados += saida_visitados;
        total_h1_custo += saida_custo; if(saida_achou) total_h1_sucessos++;

        weightedAStar(matrix, start, finish, distEuclid, 3, output3, saida_gerados, saida_visitados, saida_custo, saida_achou);
        total_h2_gerados += saida_gerados; total_h2_visitados += saida_visitados;
        total_h2_custo += saida_custo; if(saida_achou) total_h2_sucessos++;

        weightedAStar(matrix, start, finish, distEuclid, 6, output3, saida_gerados, saida_visitados, saida_custo, saida_achou);
        total_h3_gerados += saida_gerados; total_h3_visitados += saida_visitados;
        total_h3_custo += saida_custo; if(saida_achou) total_h3_sucessos++;

        weightedAStar(matrix, start, finish, distMan, 1, output3, saida_gerados, saida_visitados, saida_custo, saida_achou);
        total_h4_gerados += saida_gerados; total_h4_visitados += saida_visitados;
        total_h4_custo += saida_custo; if(saida_achou) total_h4_sucessos++;

        weightedAStar(matrix, start, finish, distMan, 3, output3, saida_gerados, saida_visitados, saida_custo, saida_achou);
        total_h5_gerados += saida_gerados; total_h5_visitados += saida_visitados;
        total_h5_custo += saida_custo; if(saida_achou) total_h5_sucessos++;

        weightedAStar(matrix, start, finish, distMan, 6, output3, saida_gerados, saida_visitados, saida_custo, saida_achou);
        total_h6_gerados += saida_gerados; total_h6_visitados += saida_visitados;
        total_h6_custo += saida_custo; if(saida_achou) total_h6_sucessos++;

        output3 << endl;
    }

    output3 << "========================================" << endl;
    output3 << "MEDIAS - EXPERIMENTO 3 (20 entradas, 20% obstaculos)" << endl;
    output3 << "========================================" << endl << endl;

    output3 << "h1 = 1x Euclidiana" << endl;
    output3 << "  Media gerados:   " << total_h1_gerados / 20.0   << endl;
    output3 << "  Media visitados: " << total_h1_visitados / 20.0 << endl;
    output3 << "  Media custo:     " << total_h1_custo / 20.0     << endl;
    output3 << "  Taxa sucesso:    " << (total_h1_sucessos / 20.0) * 100.0 << "%" << endl << endl;

    output3 << "h2 = 3x Euclidiana" << endl;
    output3 << "  Media gerados:   " << total_h2_gerados / 20.0   << endl;
    output3 << "  Media visitados: " << total_h2_visitados / 20.0 << endl;
    output3 << "  Media custo:     " << total_h2_custo / 20.0     << endl;
    output3 << "  Taxa sucesso:    " << (total_h2_sucessos / 20.0) * 100.0 << "%" << endl << endl;

    output3 << "h3 = 6x Euclidiana" << endl;
    output3 << "  Media gerados:   " << total_h3_gerados / 20.0   << endl;
    output3 << "  Media visitados: " << total_h3_visitados / 20.0 << endl;
    output3 << "  Media custo:     " << total_h3_custo / 20.0     << endl;
    output3 << "  Taxa sucesso:    " << (total_h3_sucessos / 20.0) * 100.0 << "%" << endl << endl;

    output3 << "h4 = 1x Manhattan" << endl;
    output3 << "  Media gerados:   " << total_h4_gerados / 20.0   << endl;
    output3 << "  Media visitados: " << total_h4_visitados / 20.0 << endl;
    output3 << "  Media custo:     " << total_h4_custo / 20.0     << endl;
    output3 << "  Taxa sucesso:    " << (total_h4_sucessos / 20.0) * 100.0 << "%" << endl << endl;

    output3 << "h5 = 3x Manhattan" << endl;
    output3 << "  Media gerados:   " << total_h5_gerados / 20.0   << endl;
    output3 << "  Media visitados: " << total_h5_visitados / 20.0 << endl;
    output3 << "  Media custo:     " << total_h5_custo / 20.0     << endl;
    output3 << "  Taxa sucesso:    " << (total_h5_sucessos / 20.0) * 100.0 << "%" << endl << endl;

    output3 << "h6 = 6x Manhattan" << endl;
    output3 << "  Media gerados:   " << total_h6_gerados / 20.0   << endl;
    output3 << "  Media visitados: " << total_h6_visitados / 20.0 << endl;
    output3 << "  Media custo:     " << total_h6_custo / 20.0     << endl;
    output3 << "  Taxa sucesso:    " << (total_h6_sucessos / 20.0) * 100.0 << "%" << endl << endl;

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
            pair<int,int> start, finish;
            do {
                for(int i = 0; i < 15; i++)
                    for(int j = 0; j < 15; j++)
                        matrix[i][j] = distr1(gen);
                for(int i = 0; i < obstacles; i++) {
                    pair<int,int> block({distr2(gen), distr2(gen)});
                    while(matrix[block.first][block.second] == -1) block = make_pair(distr2(gen), distr2(gen));
                    matrix[block.first][block.second] = -1;
                }
                start = make_pair(distr2(gen), distr2(gen));
                finish = make_pair(distr2(gen), distr2(gen));
                while(matrix[start.first][start.second] == -1) start = make_pair(distr2(gen), distr2(gen));
                while(matrix[finish.first][finish.second] == -1 || start == finish) finish = make_pair(distr2(gen), distr2(gen));
            } while(!isSolvable(matrix, start, finish));

            output4 << "=== Entrada " << i+1 << " | Densidade " << pct_atual << "% ===" << endl;
            writeMap(matrix, output4);
            output4 << "start: (" << start.first << ", " << start.second << ")   finish: (" << finish.first << ", " << finish.second << ")" << endl << endl;

            subidaEncostaMaiorAclive(matrix, start, finish, distMan, output4, saida_gerados, saida_visitados, saida_custo, saida_achou);
            total_sde_gerados += saida_gerados; total_sde_visitados += saida_visitados;
            total_sde_custo += saida_custo; if(saida_achou) total_sde_sucessos++;

            buscaGulosa(matrix, start, finish, distMan, output4, saida_gerados, saida_visitados, saida_custo, saida_achou);
            total_gulosa_gerados += saida_gerados; total_gulosa_visitados += saida_visitados;
            total_gulosa_custo += saida_custo; if(saida_achou) total_gulosa_sucessos++;

            aStar(matrix, start, finish, distMan, output4, saida_gerados, saida_visitados, saida_custo, saida_achou);
            total_ast_gerados += saida_gerados; total_ast_visitados += saida_visitados;
            total_ast_custo += saida_custo; if(saida_achou) total_ast_sucessos++;

            output4 << endl;
        }

        output4 << "========================================" << endl;
        output4 << "MEDIAS - EXPERIMENTO 4 - Densidade " << pct_atual << "%" << endl;
        output4 << "========================================" << endl << endl;

        output4 << "SdE Maior Aclive - Manhattan" << endl;
        output4 << "  Media gerados:   " << total_sde_gerados / 20.0   << endl;
        output4 << "  Media visitados: " << total_sde_visitados / 20.0 << endl;
        output4 << "  Media custo:     " << total_sde_custo / 20.0     << endl;
        output4 << "  Taxa sucesso:    " << (total_sde_sucessos / 20.0) * 100.0 << "%" << endl << endl;

        output4 << "Busca Gulosa - Manhattan" << endl;
        output4 << "  Media gerados:   " << total_gulosa_gerados / 20.0   << endl;
        output4 << "  Media visitados: " << total_gulosa_visitados / 20.0 << endl;
        output4 << "  Media custo:     " << total_gulosa_custo / 20.0     << endl;
        output4 << "  Taxa sucesso:    " << (total_gulosa_sucessos / 20.0) * 100.0 << "%" << endl << endl;

        output4 << "A* - Manhattan" << endl;
        output4 << "  Media gerados:   " << total_ast_gerados / 20.0   << endl;
        output4 << "  Media visitados: " << total_ast_visitados / 20.0 << endl;
        output4 << "  Media custo:     " << total_ast_custo / 20.0     << endl;
        output4 << "  Taxa sucesso:    " << (total_ast_sucessos / 20.0) * 100.0 << "%" << endl << endl;
    }

    output4.close();
    return 0;
}