#ifndef A_SEARCH_H
#define A_SEARCH_H

#include <vector>
#include <queue>
#include <map>
#include <fstream>
#include <type_traits>
#include "create_map.h"
#include "heuristics.h"

using namespace std;

template <typename heuristic>
void aStar(vector<vector<int>> matrix, pair<int,int> start, pair<int,int> finish, heuristic compare, ofstream& output) {
    // fila de prioridade: sempre remove o nó com menor f(n) = g(n) + h(n).
    priority_queue<pair<double, pair<int,int>>, vector<pair<double, pair<int,int>>>, greater<pair<double, pair<int,int>>>> fila;

    vector<vector<int>> custos = matrix; // cópia dos custos antes de marcar visitado
    map<pair<int,int>, pair<int,int>> caminho; // mapa para armazena o predecessor de cada posição para reconstrução do caminho.
    map<pair<int,int>, double> g; // guarda o custo acumulado g(n) para cada nó já descoberto.
    

    int custo = 0, gerados = 1, visitados = 0;
    bool achou = false;

    caminho[start] = {-1, -1};
    g[start] = custos[start.first][start.second];
    fila.push({g[start] + compare(start, finish), start});

    while(!fila.empty()) {
        pair<double, pair<int,int>> no_atual = fila.top();  // pega o nó com o menor custo total combinado (f)
        fila.pop();
        double custo_total = no_atual.first;
        pair<int,int> posicao_atual = no_atual.second;

        if(matrix[posicao_atual.first][posicao_atual.second] == 1) continue;
        visitados++;
        matrix[posicao_atual.first][posicao_atual.second] = 1;

        if(posicao_atual == finish) { achou = true; break; }

        // norte
        if(posicao_atual.first - 1 > -1 && matrix[posicao_atual.first-1][posicao_atual.second] > 1) {
            pair<int,int> next = {posicao_atual.first-1, posicao_atual.second};
            double new_g = g[posicao_atual] + custos[next.first][next.second];
            if(!g.count(next) || new_g < g[next]) {
                g[next] = new_g;
                caminho[next] = posicao_atual;
                gerados++;
                fila.push({new_g + compare(next, finish), next});
            }
        }
        // leste
        if(posicao_atual.second + 1 < 15 && matrix[posicao_atual.first][posicao_atual.second+1] > 1) {
            pair<int,int> next = {posicao_atual.first, posicao_atual.second+1};
            double new_g = g[posicao_atual] + custos[next.first][next.second];
            if(!g.count(next) || new_g < g[next]) {
                g[next] = new_g;
                caminho[next] = posicao_atual;
                gerados++;
                fila.push({new_g + compare(next, finish), next});
            }
        }
        // sul
        if(posicao_atual.first + 1 < 15 && matrix[posicao_atual.first+1][posicao_atual.second] > 1) {
            pair<int,int> next = {posicao_atual.first+1, posicao_atual.second};
            double new_g = g[posicao_atual] + custos[next.first][next.second];
            if(!g.count(next) || new_g < g[next]) {
                g[next] = new_g;
                caminho[next] = posicao_atual;
                gerados++;
                fila.push({new_g + compare(next, finish), next});
            }
        }
        // oeste
        if(posicao_atual.second - 1 > -1 && matrix[posicao_atual.first][posicao_atual.second-1] > 1) {
            pair<int,int> next = {posicao_atual.first, posicao_atual.second-1};
            double new_g = g[posicao_atual] + custos[next.first][next.second];
            if(!g.count(next) || new_g < g[next]) {
                g[next] = new_g;
                caminho[next] = posicao_atual;
                gerados++;
                fila.push({new_g + compare(next, finish), next});
            }
        }
    }

    if(achou) custo = (int)g[finish];

    writeMap(matrix, output);
    output << "A*" << endl;
    if constexpr (is_same_v<decltype(compare(start, finish)), double>) {
        output << "Distancia Euclidiana" << endl;
    } else {
        output << "Distancia Manhattan" << endl;
    }
    output << "Estados gerados: "   << gerados   << endl;
    output << "Estados visitados: " << visitados << endl;
    output << "Custo do caminho: " << custo << endl << endl; 
}

template <typename heuristic>
void weightedAStar(vector<vector<int>> matrix, pair<int,int> start, pair<int,int> finish, heuristic compare, double weight, ofstream& output) {
    // fila de prioridade: sempre remove o nó com menor f(n) = g(n) + h(n).
    priority_queue<pair<double, pair<int,int>>, vector<pair<double, pair<int,int>>>, greater<pair<double, pair<int,int>>>> fila;

    vector<vector<int>> custos = matrix; // cópia dos custos antes de marcar visitado
    map<pair<int,int>, pair<int,int>> caminho; // mapa para armazena o predecessor de cada posição para reconstrução do caminho.
    map<pair<int,int>, double> g; // guarda o custo acumulado g(n) para cada nó já descoberto.
    

    int custo = 0, gerados = 1, visitados = 0;
    bool achou = false;

    caminho[start] = {-1, -1};
    g[start] = custos[start.first][start.second];
    fila.push({g[start] + compare(start, finish)*weight, start});

    while(!fila.empty()) {
        pair<double, pair<int,int>> no_atual = fila.top();  // pega o nó com o menor custo total combinado (f)
        fila.pop();
        double custo_total = no_atual.first;
        pair<int,int> posicao_atual = no_atual.second;

        if(matrix[posicao_atual.first][posicao_atual.second] == 1) continue;
        visitados++;
        matrix[posicao_atual.first][posicao_atual.second] = 1;

        if(posicao_atual == finish) { achou = true; break; }

        // norte
        if(posicao_atual.first - 1 > -1 && matrix[posicao_atual.first-1][posicao_atual.second] > 1) {
            pair<int,int> next = {posicao_atual.first-1, posicao_atual.second};
            double new_g = g[posicao_atual] + custos[next.first][next.second];
            if(!g.count(next) || new_g < g[next]) {
                g[next] = new_g;
                caminho[next] = posicao_atual;
                gerados++;
                fila.push({new_g + compare(next, finish)*weight, next});
            }
        }
        // leste
        if(posicao_atual.second + 1 < 15 && matrix[posicao_atual.first][posicao_atual.second+1] > 1) {
            pair<int,int> next = {posicao_atual.first, posicao_atual.second+1};
            double new_g = g[posicao_atual] + custos[next.first][next.second];
            if(!g.count(next) || new_g < g[next]) {
                g[next] = new_g;
                caminho[next] = posicao_atual;
                gerados++;
                fila.push({new_g + compare(next, finish)*weight, next});
            }
        }
        // sul
        if(posicao_atual.first + 1 < 15 && matrix[posicao_atual.first+1][posicao_atual.second] > 1) {
            pair<int,int> next = {posicao_atual.first+1, posicao_atual.second};
            double new_g = g[posicao_atual] + custos[next.first][next.second];
            if(!g.count(next) || new_g < g[next]) {
                g[next] = new_g;
                caminho[next] = posicao_atual;
                gerados++;
                fila.push({new_g + compare(next, finish)*weight, next});
            }
        }
        // oeste
        if(posicao_atual.second - 1 > -1 && matrix[posicao_atual.first][posicao_atual.second-1] > 1) {
            pair<int,int> next = {posicao_atual.first, posicao_atual.second-1};
            double new_g = g[posicao_atual] + custos[next.first][next.second];
            if(!g.count(next) || new_g < g[next]) {
                g[next] = new_g;
                caminho[next] = posicao_atual;
                gerados++;
                fila.push({new_g + compare(next, finish)*weight, next});
            }
        }
    }

    if(achou) custo = (int)g[finish];

    writeMap(matrix, output);
    output << "Weighted A*" << endl;
    output << "Peso: " << weight << endl;
    if constexpr (is_same_v<decltype(compare(start, finish)), double>) {
        output << "Distancia Euclidiana" << endl;
    } else {
        output << "Distancia Manhattan" << endl;
    }
    output << "Estados gerados: "   << gerados   << endl;
    output << "Estados visitados: " << visitados << endl;
    output << "Custo do caminho: " << custo << endl << endl; 
}

#endif // A_SEARCH_H