#ifndef HEURISTICS_H
#define HEURISTICS_H

#include <utility>
#include <cmath>

using namespace std;

double distEuclid(pair<int, int> current, pair<int, int> destination) {
    double cat1 = abs(current.first - destination.first);
    double cat2 = abs(current.second - destination.second);
    return sqrt(cat1*cat1 + cat2*cat2) ;
}

double distMan(pair<int, int> current, pair<int, int> destination) {
    int cat1 = abs(current.first - destination.first);
    int cat2 = abs(current.second - destination.second);
    return cat1 + cat2 ;
}

// heurísticas com fator de multiplicação para a questão 3 
double distEuclid3(pair<int,int> current, pair<int,int> destination) { return 3.0 * distEuclid(current,destination); }
double distEuclid6(pair<int,int> current, pair<int,int> destination) { return 6.0 * distEuclid(current,destination); }
double distMan3   (pair<int,int> current, pair<int,int> destination) { return 3.0 * distMan(current,destination); }
double distMan6   (pair<int,int> current, pair<int,int> destination) { return 6.0 * distMan(current,destination); }


#endif // HEURISTICS_H