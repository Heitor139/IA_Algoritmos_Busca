#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include "create_map.h"
#include "heuristics.h"
#include "greedy_search.h"
#include "a_search.h"


using namespace std;

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
/*
         subidaEncostaGulosa(matrix, start, finish, distEuclid, output1);
        subidaEncostaGulosa(matrix, start, finish, distMan, output1);
        subidaEncostaMaiorAclive(matrix, start, finish, distEuclid, output1);
        subidaEncostaMaiorAclive(matrix, start, finish, distMan, output1);
        subidaEncostaEstocastico(matrix, start, finish, distEuclid, output1);
        subidaEncostaEstocastico(matrix, start, finish, distMan, output1); 
*/
        buscaGulosa(matrix, start, finish, distEuclid, output1);
        buscaGulosa(matrix, start, finish, distMan, output1);
        aStar(matrix, start, finish, distEuclid, output1);
        aStar(matrix, start, finish, distMan, output1);
    }

    output1.close();

/*
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
 */
    return 0;
}