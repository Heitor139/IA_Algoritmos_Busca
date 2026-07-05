#ifndef HILL_CLIMBING_H
#define HILL_CLIMBING_H

#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <type_traits>
#include "create_map.h"
#include "heuristics.h"

using namespace std;

template <typename heuristic>
void subidaEncostaGulosa(vector<vector<int>> matrix, pair<int, int> start, pair<int, int> finish, heuristic compare, ofstream& output) {
    queue<pair<int,int>> fila;
    int custo = 0, gerados = 1, visitados = 0;
    fila.push(start);
    while(!fila.empty()) {
        custo += matrix[fila.front().first][fila.front().second];
        visitados += 1;
        matrix[fila.front().first][fila.front().second] = 1;
        if(fila.front() == finish) break;

        if(fila.front().first - 1 > -1 && compare(fila.front(), finish) > compare({fila.front().first-1, fila.front().second}, finish)  && matrix[fila.front().first-1][fila.front().second] > 1) {
            gerados += 1;
            fila.push({fila.front().first-1, fila.front().second});
        } else if(fila.front().second + 1 < 15 && compare(fila.front(), finish) > compare({fila.front().first, fila.front().second+1}, finish) && matrix[fila.front().first][fila.front().second+1] > 1) {
            gerados += 1;
            fila.push({fila.front().first, fila.front().second+1});
        } else if(fila.front().first + 1 < 15  && compare(fila.front(), finish) > compare({fila.front().first+1, fila.front().second}, finish) && matrix[fila.front().first+1][fila.front().second] > 1) {
            gerados += 1;
            fila.push({fila.front().first+1, fila.front().second});
        } else if(fila.front().second - 1 > -1 && compare(fila.front(), finish) > compare({fila.front().first, fila.front().second-1}, finish) && matrix[fila.front().first][fila.front().second-1] > 1) {
            gerados += 1;
            fila.push({fila.front().first, fila.front().second-1});
        }
        

        fila.pop();
    }

    writeMap(matrix, output);
    output << "Subida de encosta gulosa" << endl;
    if constexpr (is_same_v<decltype(compare(start, finish)), double>) {
        output << "Distancia Euclidiana" << endl;
    } else {
        output << "Distancia Manhattan" << endl;
    }
    output << "Estados gerados: " << gerados << endl;
    output << "Estados visitados: " << visitados << endl;
    output << "Custo do caminho: " << custo << endl << endl; 
}

template <typename heuristic>
void subidaEncostaMaiorAclive(vector<vector<int>> matrix, pair<int, int> start, pair<int, int> finish, heuristic compare, ofstream& output,
                              int& saida_gerados, int& saida_visitados, int& saida_custo, bool& saida_achou) {
    queue<pair<int,int>> fila;
    int custo = 0, gerados = 1, visitados = 0;
    bool achou = false;
    fila.push(start);
    while(!fila.empty()) {
        double melhor = 100;
        int melhorId = -1;
        vector<pair<int,int>> vizinhos(4);
        custo += matrix[fila.front().first][fila.front().second];
        visitados += 1;
        matrix[fila.front().first][fila.front().second] = 1;
        if(fila.front() == finish) {
            achou = true;
            break;
        }

        if(fila.front().first - 1 > -1  && matrix[fila.front().first-1][fila.front().second] > 1) {
            gerados += 1;
            vizinhos[0] = {fila.front().first-1, fila.front().second};
            if(compare(vizinhos[0], finish) < melhor) {
                melhor = compare(vizinhos[0], finish);
                melhorId = 0;
            }
        }
        if(fila.front().second + 1 < 15 && matrix[fila.front().first][fila.front().second+1] > 1) {
            gerados += 1;
            vizinhos[1] = {fila.front().first, fila.front().second+1};
            if(compare(vizinhos[1], finish) < melhor) {
                melhor = compare(vizinhos[1], finish);
                melhorId = 1;
            }
        }
        if(fila.front().first + 1 < 15 && matrix[fila.front().first+1][fila.front().second] > 1) {
            gerados += 1;
            vizinhos[2] = {fila.front().first+1, fila.front().second};
            if(compare(vizinhos[2], finish) < melhor) {
                melhor = compare(vizinhos[2], finish);
                melhorId = 2;
            }
        }
        if(fila.front().second - 1 > -1 && matrix[fila.front().first][fila.front().second-1] > 1) {
            gerados += 1;
            vizinhos[3] = {fila.front().first, fila.front().second-1};
            if(compare(vizinhos[3], finish) < melhor) {
                melhor = compare(vizinhos[3], finish);
                melhorId = 3;
            }
        }

        if(melhorId == -1) break;
        fila.push(vizinhos[melhorId]);
        

        fila.pop();
    }

    saida_gerados = gerados;
    saida_visitados = visitados;
    saida_custo = custo;
    saida_achou = achou;

    writeMap(matrix, output);
    output << "Subida de encosta pelo maior aclive" << endl;
    if constexpr (is_same_v<decltype(compare(start, finish)), double>) {
        output << "Distancia Euclidiana" << endl;
    } else {
        output << "Distancia Manhattan" << endl;
    }
    output << "Estados gerados: " << gerados << endl;
    output << "Estados visitados: " << visitados << endl;
    output << "Custo do caminho: " << custo << endl << endl; 
}

template <typename heuristic>
void subidaEncostaEstocastico(vector<vector<int>> matrix, pair<int, int> start, pair<int, int> finish, heuristic compare, ofstream& output) {
    queue<pair<int,int>> fila;
    int custo = 0, gerados = 1, visitados = 0;
    fila.push(start);
    while(!fila.empty()) {
        vector<pair<int,int>> vizinhos;
        custo += matrix[fila.front().first][fila.front().second];
        visitados += 1;
        matrix[fila.front().first][fila.front().second] = 1;
        if(fila.front() == finish) break;

        if(fila.front().first - 1 > -1 && compare(fila.front(), finish) > compare({fila.front().first-1, fila.front().second}, finish) && matrix[fila.front().first-1][fila.front().second] > 1) {
            gerados += 1;
            vizinhos.push_back({fila.front().first-1, fila.front().second});
        }
        if(fila.front().second + 1 < 15 && compare(fila.front(), finish) > compare({fila.front().first, fila.front().second+1}, finish) && matrix[fila.front().first][fila.front().second+1] > 1) {
            gerados += 1;
            vizinhos.push_back({fila.front().first, fila.front().second+1});
        }
        if(fila.front().first + 1 < 15 && compare(fila.front(), finish) > compare({fila.front().first+1, fila.front().second}, finish) && matrix[fila.front().first+1][fila.front().second] > 1) {
            gerados += 1;
            vizinhos.push_back({fila.front().first+1, fila.front().second});
        }
        if(fila.front().second - 1 > -1 && compare(fila.front(), finish) > compare({fila.front().first, fila.front().second-1}, finish) && matrix[fila.front().first][fila.front().second-1] > 1) {
            gerados += 1;
            vizinhos.push_back({fila.front().first, fila.front().second-1});
        }

        if(vizinhos.empty()) break;

        random_device rand; 
        mt19937 g(rand()); 
        uniform_int_distribution<> distr(0, vizinhos.size()-1); 
        int go = distr(g);

        fila.push(vizinhos[go]);
        

        fila.pop();
    }

    writeMap(matrix, output);
    output << "Subida de encosta estocástica" << endl;
    if constexpr (is_same_v<decltype(compare(start, finish)), double>) {
        output << "Distancia Euclidiana" << endl;
    } else {
        output << "Distancia Manhattan" << endl;
    }
    output << "Estados gerados: " << gerados << endl;
    output << "Estados visitados: " << visitados << endl;
    output << "Custo do caminho: " << custo << endl << endl; 
}



#endif // HILL_CLIMBING_H