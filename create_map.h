#ifndef CREATE_MAP_H
#define CREATE_MAP_H

#include <queue>
#include <iostream>
#include <vector>
#include <fstream>


using namespace std;

inline void showMap(vector<vector<int>>& matrix) {

    cout << "   | ";
    for(int i = 0; i < 15; i++) {
        cout << i << " |";
        if(i < 9) cout << " ";
    }
    cout << endl;

    for(int i = 0; i < 15; i++) {
        if(i < 10) {
            cout << i << "  | ";
        } else {
            cout << i << " | ";
        }
        for(int j = 0; j < 15; j++) {
            if(matrix[i][j] < 0) cout << matrix[i][j] << "| ";
            else cout << matrix[i][j] << " | ";
        }
        cout << endl;
    }
    cout << endl;
}

inline void writeMap(vector<vector<int>>& matrix, ofstream& output) {
    output << "   | ";
    for(int i = 0; i < 15; i++) {
        output << i << " |";
        if(i < 9) output << " ";
    }
    output << endl;

    for(int i = 0; i < 15; i++) {
        if(i < 10) {
            output << i << "  | ";
        } else {
            output << i << " | ";
        }
        for(int j = 0; j < 15; j++) {
            if(matrix[i][j] < 0) output << matrix[i][j] << "| ";
            else output << matrix[i][j] << " | ";
        }
        output << endl;
    }
    output << endl;
}

inline int isSolvable(vector<vector<int>> matrix, pair<int, int> start, pair<int, int> finish) {
    queue<pair<int,int>> fila;
    fila.push(start);
    while(!fila.empty()) {
        if(fila.front() == finish) return 1;
        matrix[fila.front().first][fila.front().second] = 1;

        if(fila.front().first - 1 > -1 && matrix[fila.front().first-1][fila.front().second] > 1) {
            matrix[fila.front().first-1][fila.front().second] = 0;
            fila.push({fila.front().first-1, fila.front().second});
        }
        if(fila.front().second + 1 < 15 && matrix[fila.front().first][fila.front().second+1] > 1) {
            matrix[fila.front().first][fila.front().second+1] = 0;
            fila.push({fila.front().first, fila.front().second+1});
        }
        if(fila.front().first + 1 < 15  && matrix[fila.front().first+1][fila.front().second] > 1) {
            matrix[fila.front().first+1][fila.front().second] = 0;
            fila.push({fila.front().first+1, fila.front().second});
        }
        if(fila.front().second - 1 > -1 && matrix[fila.front().first][fila.front().second-1] > 1) {
            matrix[fila.front().first][fila.front().second-1] = 0;
            fila.push({fila.front().first, fila.front().second-1});
        }
        

        fila.pop();
    }

    return 0;
}

#endif //CREATE_MAP_H