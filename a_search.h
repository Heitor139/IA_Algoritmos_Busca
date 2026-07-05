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
void aStar(vector<vector<int>> matrix, pair<int,int> start, pair<int,int> finish, heuristic compare, ofstream& output,
        int& saida_gerados, int& saida_visitados, int& saida_custo, bool& saida_achou) {
    // fila de prioridade: sempre remove o nó com menor f(n) = g(n) + h(n).
    priority_queue<pair<double, pair<pair<int,int>, pair<int,int>>>, vector<pair<double, pair<pair<int,int>, pair<int,int>>>>, greater<pair<double, pair<pair<int,int>, pair<int,int>>>>> fila;

    vector<vector<int>> custos = matrix; // cópia dos custos antes de marcar visitado
    map<pair<int,int>, pair<int,int>> caminho; // mapa para armazena o predecessor de cada posição para reconstrução do caminho.
    map<pair<int,int>, double> g; // guarda o custo acumulado g(n) para cada nó já descoberto.
    

    int custo = 0, gerados = 1, visitados = 0;
    bool achou = false;

    g[start] = 0;
    fila.push({g[start] + static_cast<double>(compare(start, finish)), {start, {-1, -1}}});

    while(!fila.empty()) {
        pair<double, pair<pair<int,int>, pair<int,int>>> no_atual = fila.top();  // pega o nó com o menor custo total combinado (f)
        fila.pop();
        pair<int,int> posicao_atual = no_atual.second.first;
        pair<int,int> pai           = no_atual.second.second;

        if(matrix[posicao_atual.first][posicao_atual.second] == 1) continue;
        // registra o predecessor apenas quando o nó realmente entra em expansão, ou seja, quando é removido da fila de prioridade.
        caminho[posicao_atual] = pai;
        visitados++;
        matrix[posicao_atual.first][posicao_atual.second] = 1;

        if(posicao_atual == finish) { achou = true; break; }

        // norte
        if(posicao_atual.first - 1 > -1 && matrix[posicao_atual.first-1][posicao_atual.second] > 1) {
            pair<int,int> next = {posicao_atual.first-1, posicao_atual.second};
            double new_g = g[posicao_atual] + custos[next.first][next.second];
            if(!g.count(next) || new_g < g[next]) {
                g[next] = new_g;
                gerados++;
                fila.push({new_g + static_cast<double>(compare(next, finish)), {next, posicao_atual}});
            }
        }
        // leste
        if(posicao_atual.second + 1 < 15 && matrix[posicao_atual.first][posicao_atual.second+1] > 1) {
            pair<int,int> next = {posicao_atual.first, posicao_atual.second+1};
            double new_g = g[posicao_atual] + custos[next.first][next.second];
            if(!g.count(next) || new_g < g[next]) {
                g[next] = new_g;
                gerados++;
                fila.push({new_g + static_cast<double>(compare(next, finish)), {next, posicao_atual}});
            }
        }
        // sul
        if(posicao_atual.first + 1 < 15 && matrix[posicao_atual.first+1][posicao_atual.second] > 1) {
            pair<int,int> next = {posicao_atual.first+1, posicao_atual.second};
            double new_g = g[posicao_atual] + custos[next.first][next.second];
            if(!g.count(next) || new_g < g[next]) {
                g[next] = new_g;
                gerados++;
                fila.push({new_g + static_cast<double>(compare(next, finish)), {next, posicao_atual}});
            }
        }
        // oeste
        if(posicao_atual.second - 1 > -1 && matrix[posicao_atual.first][posicao_atual.second-1] > 1) {
            pair<int,int> next = {posicao_atual.first, posicao_atual.second-1};
            double new_g = g[posicao_atual] + custos[next.first][next.second];
            if(!g.count(next) || new_g < g[next]) {
                g[next] = new_g;
                gerados++;
                fila.push({new_g + static_cast<double>(compare(next, finish)), {next, posicao_atual}});
            }
        }
    }

    if(achou) custo = (int)g[finish];

    saida_gerados = gerados;
    saida_visitados = visitados;
    saida_custo = custo;
    saida_achou = achou;

    writeMap(matrix, output);
    output << "A*" << endl;
    if constexpr (is_same_v<decltype(compare(start, finish)), double>) {
        output << "Distancia Euclidiana" << endl;
    } else {
        output << "Distancia Manhattan" << endl;
    }
    output << "Estados gerados: "   << gerados   << endl;
    output << "Estados visitados: " << visitados << endl;
    output << "Custo do caminho: " << custo << endl;
}

template <typename heuristic>
void weightedAStar(vector<vector<int>> matrix, pair<int,int> start, pair<int,int> finish, heuristic compare, double weight, ofstream& output) {
    // fila de prioridade: sempre remove o nó com menor f(n) = g(n) + h(n).
    priority_queue<pair<double, pair<pair<int,int>, pair<int,int>>>, vector<pair<double, pair<pair<int,int>, pair<int,int>>>>, greater<pair<double, pair<pair<int,int>, pair<int,int>>>>> fila;

    vector<vector<int>> custos = matrix; // cópia dos custos antes de marcar visitado
    map<pair<int,int>, pair<int,int>> caminho; // mapa para armazena o predecessor de cada posição para reconstrução do caminho.
    map<pair<int,int>, double> g; // guarda o custo acumulado g(n) para cada nó já descoberto.
    

    int custo = 0, gerados = 1, visitados = 0;
    bool achou = false;


    g[start] = 0;
    fila.push({g[start] + static_cast<double>(compare(start, finish))*weight, {start, {-1, -1}}});

    while(!fila.empty()) {
        pair<double, pair<pair<int,int>, pair<int,int>>> no_atual = fila.top();  // pega o nó com o menor custo total combinado (f)
        fila.pop();
        pair<int,int> posicao_atual = no_atual.second.first;
        pair<int,int> pai           = no_atual.second.second;

        if(matrix[posicao_atual.first][posicao_atual.second] == 1) continue;
        caminho[posicao_atual] = pai;
        visitados++;
        matrix[posicao_atual.first][posicao_atual.second] = 1;

        if(posicao_atual == finish) { achou = true; break; }

        // norte
        if(posicao_atual.first - 1 > -1 && matrix[posicao_atual.first-1][posicao_atual.second] > 1) {
            pair<int,int> next = {posicao_atual.first-1, posicao_atual.second};
            double new_g = g[posicao_atual] + custos[next.first][next.second];
            if(!g.count(next) || new_g < g[next]) {
                g[next] = new_g;
                gerados++;
                fila.push({new_g + static_cast<double>(compare(next, finish))*weight, {next, posicao_atual}});
            }
        }
        // leste
        if(posicao_atual.second + 1 < 15 && matrix[posicao_atual.first][posicao_atual.second+1] > 1) {
            pair<int,int> next = {posicao_atual.first, posicao_atual.second+1};
            double new_g = g[posicao_atual] + custos[next.first][next.second];
            if(!g.count(next) || new_g < g[next]) {
                g[next] = new_g;
                gerados++;
                fila.push({new_g + static_cast<double>(compare(next, finish))*weight, {next, posicao_atual}});
            }
        }
        // sul
        if(posicao_atual.first + 1 < 15 && matrix[posicao_atual.first+1][posicao_atual.second] > 1) {
            pair<int,int> next = {posicao_atual.first+1, posicao_atual.second};
            double new_g = g[posicao_atual] + custos[next.first][next.second];
            if(!g.count(next) || new_g < g[next]) {
                g[next] = new_g;
                gerados++;
                fila.push({new_g + static_cast<double>(compare(next, finish))*weight, {next, posicao_atual}});
            }
        }
        // oeste
        if(posicao_atual.second - 1 > -1 && matrix[posicao_atual.first][posicao_atual.second-1] > 1) {
            pair<int,int> next = {posicao_atual.first, posicao_atual.second-1};
            double new_g = g[posicao_atual] + custos[next.first][next.second];
            if(!g.count(next) || new_g < g[next]) {
                g[next] = new_g;
                gerados++;
                fila.push({new_g + static_cast<double>(compare(next, finish))*weight, {next, posicao_atual}});
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
    output << "Custo do caminho: " << custo << endl;
}

#endif // A_SEARCH_H