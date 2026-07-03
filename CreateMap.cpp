#include <vector>
#include <queue>
#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <utility>
#include <type_traits>

using namespace std;

void showMap(vector<vector<int>>& matrix) {

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

void writeMap(vector<vector<int>>& matrix, ofstream& output) {
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

int isSolvable(vector<vector<int>> matrix, pair<int, int> start, pair<int, int> finish) {
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

double distEuclid(pair<int, int> current, pair<int, int> destination) {
    double cat1 = abs(current.first - destination.first);
    double cat2 = abs(current.second - destination.second);
    return sqrt(cat1*cat1 + cat2*cat2);
}

int distMan(pair<int, int> current, pair<int, int> destination) {
    int cat1 = abs(current.first - destination.first);
    int cat2 = abs(current.second - destination.second);
    return cat1 + cat2;
}

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
void subidaEncostaMaiorAclive(vector<vector<int>> matrix, pair<int, int> start, pair<int, int> finish, heuristic compare, ofstream& output) {
    queue<pair<int,int>> fila;
    int custo = 0, gerados = 1, visitados = 0;
    fila.push(start);
    while(!fila.empty()) {
        double melhor = 100;
        int melhorId = -1;
        vector<pair<int,int>> vizinhos(4);
        custo += matrix[fila.front().first][fila.front().second];
        visitados += 1;
        matrix[fila.front().first][fila.front().second] = 1;
        if(fila.front() == finish) break;

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

        fila.push(vizinhos[melhorId]);
        

        fila.pop();
    }

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

int main() {
    vector<vector<int>> matrix(15, vector<int> (15));
    double obstacles = 0;

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
        subidaEncostaMaiorAclive(matrix, start, finish, distEuclid, output1);
        subidaEncostaMaiorAclive(matrix, start, finish, distMan, output1);
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
        
    }

    output2.close();

    obstacles = round(225*(20/100));

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
        
    }

    output3.close();

    obstacles = 10;

    ofstream output4("q4_output.txt");
    if(!output4.is_open()) {
        cout << "Falha em abrir o arquivo" << endl;
        return 1;
    }


    for(int o = 0; o < 4; o++) {
        obstacles = round(225*(obstacles+(10*o)/100));
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
            output4 << "start: (" << start.first << ", " << start.second << ")   finish: (" << finish.first << ", " << finish.second << ")" << endl << endl;
        
        }
    }

    return 0;
}