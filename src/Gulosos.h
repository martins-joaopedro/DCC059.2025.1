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

    static map<float, vector<int>> sols;
    static map<float, vector<double>> tempos;

    // GULOSO
    void reset_domain(Grafo* grafo);
    bool independent_set(const vector<char> &D, Grafo* grafo);
    No* get_node(char id, Grafo* grafo);
    vector<char> get_neighbors(char id_no, Grafo* grafo);
    bool dominant_set(const vector<char> &D, Grafo* grafo);
    void reorganize(vector<pair<char, int>> &vertice_grau_ordenado, Grafo* grafo);
    vector<char> greedy_heuristic(Grafo* grafo, ofstream& file);

    // RANDOMIZADO ADAPTATIVO
    bool check_validity(vector<char> S, Grafo* grafo, ofstream& file);
    void updates_domain(map<char, No*>& mapa_nos, vector<char> S);
    vector<char> updates_LC(map<char, No*>& mapa_nos, vector<char> LC, vector<char> S);
    vector<char> randomized_heuristic(vector<char> LC, map<char, No*>& mapa_nos, int k, ofstream& file);
    vector<char> randomized_adaptative_greedy(Grafo* grafo, float alfa, ofstream& file);
    
    // REATIVO
    void updates_probability(vector<float>& P, vector<float>& M, int m, int solBest_size, ofstream& file);
    void updates_means(vector<float>& M, vector<int>& sum_sols, vector<int>& count, vector<char>& s, int index_alpha);
    void imprime_prob(vector<float>& alphas, vector<float>& P, vector<float>& M,vector<int>& sum_sols, vector<int>& count, int m, ofstream& file);
    int choose_alpha(vector<float>& P);
    vector<char> randomized_adaptative_reactive_greedy(Grafo* grafo, vector<float>& alphas, int m, int nIter, int bloco, ofstream& file);
    
    static void run_randomized_adaptive_greedy(Grafo* grafo, ofstream& file);
    static vector<char> run_randomized_adaptative_reactive_greedy(Grafo* grafo, ofstream& file);
    static vector<char> run_greedy(Grafo* grafo, ofstream& file);
    static void print_means_randomized_greedy();
};


#endif
