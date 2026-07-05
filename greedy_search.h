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
void buscaGulosa(vector<vector<int>> matrix, pair<int,int> start, pair<int,int> finish, heuristic compare, ofstream& output,
                 int& saida_gerados, int& saida_visitados, int& saida_custo, bool& saida_achou) {
    // fila de prioridade: sempre remove o nó com menor f(n) = h(n).
    // modificado para que o pai acompanhe o estado em cada inserção na fila
    priority_queue<pair<double, pair<pair<int,int>, pair<int,int>>>, vector<pair<double, pair<pair<int,int>, pair<int,int>>>>, greater<pair<double, pair<pair<int,int>, pair<int,int>>>>> fila;

    vector<vector<int>> custos = matrix; // cópia dos custos antes de marcar visitados
    map<pair<int,int>, pair<int,int>> caminho; // mapa para reconstruir o caminho   
    vector<vector<bool>> gerados_map(15, vector<bool>(15, false)); // controla os estados já inseridos na fila, afim de evitar inserções repetidas que era oq acontecia antes
    
    int custo = 0, gerados = 1, visitados = 0;
    bool achou = false;

    fila.push({static_cast<double>(compare(start, finish)), {start, {-1, -1}}});
    gerados_map[start.first][start.second] = true;

    while(!fila.empty()) {
        pair<double, pair<pair<int,int>, pair<int,int>>> elemento = fila.top(); fila.pop();
        pair<int,int> coordenada_atual = elemento.second.first;
        pair<int,int> pai              = elemento.second.second;

        if(matrix[coordenada_atual.first][coordenada_atual.second] == 1) continue;
        // registra o predecessor somente quando o nó realmente entra em expansão
        caminho[coordenada_atual] = pai;
        visitados++;
        matrix[coordenada_atual.first][coordenada_atual.second] = 1;

        if(coordenada_atual == finish) { achou = true; break; }

        // norte
        if(coordenada_atual.first - 1 > -1 && matrix[coordenada_atual.first-1][coordenada_atual.second] > 1) {
            pair<int,int> next = {coordenada_atual.first-1, coordenada_atual.second};
            if(!gerados_map[next.first][next.second]) {
                gerados_map[next.first][next.second] = true;
                gerados++;
                fila.push({static_cast<double>(compare(next, finish)), {next, coordenada_atual}});
            }
        }
        // leste
        if(coordenada_atual.second + 1 < 15 && matrix[coordenada_atual.first][coordenada_atual.second+1] > 1) {
            pair<int,int> next = {coordenada_atual.first, coordenada_atual.second+1};
            if(!gerados_map[next.first][next.second]) {
                gerados_map[next.first][next.second] = true;
                gerados++;
                fila.push({static_cast<double>(compare(next, finish)), {next, coordenada_atual}});
            }
        }
        // sul
        if(coordenada_atual.first + 1 < 15 && matrix[coordenada_atual.first+1][coordenada_atual.second] > 1) {
            pair<int,int> next = {coordenada_atual.first+1, coordenada_atual.second};
            if(!gerados_map[next.first][next.second]) {
                gerados_map[next.first][next.second] = true;
                gerados++;
                fila.push({static_cast<double>(compare(next, finish)), {next, coordenada_atual}});
            }
        }
        // oeste
        if(coordenada_atual.second - 1 > -1 && matrix[coordenada_atual.first][coordenada_atual.second-1] > 1) {
            pair<int,int> next = {coordenada_atual.first, coordenada_atual.second-1};
            if(!gerados_map[next.first][next.second]) {
                gerados_map[next.first][next.second] = true;
                gerados++;
                fila.push({static_cast<double>(compare(next, finish)), {next, coordenada_atual}});
            }
        }
    }

    if(achou) {
        pair<int,int> coordenada_atual = finish;
        while(coordenada_atual != make_pair(-1, -1)) {
            custo += custos[coordenada_atual.first][coordenada_atual.second];
            coordenada_atual = caminho[coordenada_atual];
        }
    }

    saida_gerados = gerados;
    saida_visitados = visitados;
    saida_custo = custo;
    saida_achou = achou;

    writeMap(matrix, output);
    output << "Busca Gulosa" << endl;
    if constexpr (is_same_v<decltype(compare(start, finish)), double>) {
        output << "Distancia Euclidiana" << endl;
    } else {
        output << "Distancia Manhattan" << endl;
    }
    output << "Estados gerados: "   << gerados   << endl;
    output << "Estados visitados: " << visitados << endl;
    output << "Custo do caminho: " << custo << endl;
}

#endif // GREEDY_SEARCH_H