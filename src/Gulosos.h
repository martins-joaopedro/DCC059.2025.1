#ifndef GULOSOS_H
#define GULOSOS_H

#include <iostream>
#include <vector>
#include <map>

#include "Grafo.h"
#include "No.h"
#include "Aresta.h"

using namespace std;
class Gulosos {
public:

    bool check_validity(vector<char> S, Grafo* grafo);
    void updates_domain(map<char, No*>& mapa_nos, vector<char> S);
    vector<char> updates_LC(map<char, No*>& mapa_nos, vector<char> LC, vector<char> S);
    vector<char> randomized_heuristic(vector<char> LC, map<char, No*>& mapa_nos, int k);
    vector<char> randomized_adaptative_greedy(Grafo* grafo, float alfa);
    
    
    void updates_probability(vector<float>& P, vector<float>& M, int m, int solBest_size);
    void updates_means(vector<float>& M, vector<int>& sum_sols, vector<int>& count, vector<char>& s, int index_alpha);
    int choose_alpha(vector<float>& P);
    vector<char> randomized_adaptative_reactive_greedy(Grafo* grafo, vector<float>& alphas, int m, int nIter, int bloco);
    
    void imprime_prob(vector<float>& alphas, vector<float>& P, vector<float>& M,vector<int>& sum_sols, vector<int>& count, int m);
    
    vector<char> heuristica_gulosa(Grafo* grafo);
    void reorganiza(vector<pair<char, int>> &vertice_grau_ordenado);

    void run_randomized_adaptive_greedy(Grafo* grafo);
    void run_randomized_adaptative_reactive_greedy(Grafo* grafo);
    
};


#endif
