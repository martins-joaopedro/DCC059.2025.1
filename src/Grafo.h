//
// Created by Rafael on 28/05/2025.
//

#ifndef GRAFO_H
#define GRAFO_H

#include "No.h"
#include <iostream>
#include <vector>
#include <map>

using namespace std;
class Grafo
{
public:
    Grafo();
    ~Grafo();

    vector<char> fecho_transitivo_direto(char id_no);                                                                      // a
    vector<char> fecho_transitivo_indireto(char id_no);                                                                    // b
    vector<char> caminho_minimo_dijkstra(char id_no_a, char id_no_b);                                                      // c
    vector<char> caminho_minimo_floyd(char id_no_a, char id_no_b);                                                         // d
    Grafo *arvore_geradora_minima_prim(vector<char> ids_nos);                                                              // e
    Grafo *arvore_geradora_minima_kruskal(vector<char> ids_nos);                                                           // f
    Grafo *arvore_caminhamento_profundidade(int id_no);                                                                    // g
    Grafo *caminho_profundidade(No *no, Grafo *grafo, map<char, bool> &visitados, char pai, map<char, int> &profundidade); //

    bool insere_aresta(tuple<int, char, char> &aresta_info, vector<No *> &lista);
    void aux_fecho_transitivo_direto(map<char, bool> &C, char descendente);
    bool execoes_caminho_minimo(char id_no_a, char id_no_b);
    vector<vector<char>> cria_matriz_floyd(vector<vector<int>> &distancia, bool teste);
    map<char, int> calcular_excentricidades();
    const map<char, int> &get_excentricidades();
    map<char, int> armazena_excentricidades;
    void calcula_caracteristicas();

    int ordem;
    bool in_direcionado;
    bool in_ponderado_aresta;
    bool in_ponderado_vertice;
    vector<No *> lista_adj;
    int raio;
    int diametro;
    vector<char> centro;
    vector<char> periferia;
    bool excentricidades_validas = false;
};

#endif // GRAFO_H
