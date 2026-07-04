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

int distMan(pair<int, int> current, pair<int, int> destination) {
    int cat1 = abs(current.first - destination.first);
    int cat2 = abs(current.second - destination.second);
    return cat1 + cat2 ;
}


#endif // HEURISTICS_H