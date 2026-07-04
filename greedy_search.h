#ifndef GREEDY_SEARCH_H
#define GREEDY_SEARCH_H

#include <vector>
#include <queue>
#include <map>
#include <fstream>
#include <type_traits>
#include "create_map.h"
#include "heuristics.h"

using namespace std;

template <typename heuristic>
void buscaGulosa(vector<vector<int>> matrix, pair<int,int> start, pair<int,int> finish, heuristic compare, ofstream& output) {
    // fila de prioridade: sempre remove o nó com menor f(n) = h(n).
    priority_queue<pair<double, pair<int,int>>, vector<pair<double, pair<int,int>>>, greater<pair<double, pair<int,int>>>> fila;

    vector<vector<int>> custos = matrix; // cópia dos custos antes de marcar visitados
    map<pair<int,int>, pair<int,int>> caminho; // mapa para reconstruir o caminho   
    int custo = 0, gerados = 1, visitados = 0;
    bool achou = false;

    caminho[start] = {-1, -1};
    fila.push({compare(start, finish), start});

    while(!fila.empty()) {
        pair<double, pair<int,int>> elemento = fila.top(); fila.pop();
        double distancia_estimada = elemento.first;
        pair<int,int> coordenada_atual = elemento.second;

        if(matrix[coordenada_atual.first][coordenada_atual.second] == 1) continue;
        visitados++;
        matrix[coordenada_atual.first][coordenada_atual.second] = 1;

        if(coordenada_atual == finish) { achou = true; break; }

        // norte
        if(coordenada_atual.first - 1 > -1 && matrix[coordenada_atual.first-1][coordenada_atual.second] > 1) {
            pair<int,int> next = {coordenada_atual.first-1, coordenada_atual.second};
            gerados++;
            caminho[next] = coordenada_atual;
            fila.push({compare(next, finish), next});
        }
        // leste
        if(coordenada_atual.second + 1 < 15 && matrix[coordenada_atual.first][coordenada_atual.second+1] > 1) {
            pair<int,int> next = {coordenada_atual.first, coordenada_atual.second+1};
            gerados++;
            caminho[next] = coordenada_atual;
            fila.push({compare(next, finish), next});
        }
        // sul
        if(coordenada_atual.first + 1 < 15 && matrix[coordenada_atual.first+1][coordenada_atual.second] > 1) {
            pair<int,int> next = {coordenada_atual.first+1, coordenada_atual.second};
            gerados++;
            caminho[next] = coordenada_atual;
            fila.push({compare(next, finish), next});
        }
        // oeste
        if(coordenada_atual.second - 1 > -1 && matrix[coordenada_atual.first][coordenada_atual.second-1] > 1) {
            pair<int,int> next = {coordenada_atual.first, coordenada_atual.second-1};
            gerados++;
            caminho[next] = coordenada_atual;
            fila.push({compare(next, finish), next});
        }
    }

    if(achou) {
        pair<int,int> coordenada_atual = finish;
        while(coordenada_atual != make_pair(-1, -1)) {
            custo += custos[coordenada_atual.first][coordenada_atual.second];
            coordenada_atual = caminho[coordenada_atual];
        }
    }

    writeMap(matrix, output);
    output << "Busca Gulosa" << endl;
    if constexpr (is_same_v<decltype(compare(start, finish)), double>) {
        output << "Distancia Euclidiana" << endl;
    } else {
        output << "Distancia Manhattan" << endl;
    }
    output << "Estados gerados: "   << gerados   << endl;
    output << "Estados visitados: " << visitados << endl;
    output << "Custo do caminho: " << custo << endl << endl;
}

#endif // GREEDY_SEARCH_H