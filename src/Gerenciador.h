#ifndef GERENCIADOR_H
#define GERENCIADOR_H

#include <iostream>
#include "Grafo.h"
#include <algorithm>

using namespace std;
class Gerenciador {
public:
    static void comandos(Grafo* grafo);
    static char get_id_entrada();
    static vector<char> get_conjunto_ids(Grafo* grafo, int tam);
    static bool pergunta_imprimir_arquivo(string nome_arquivo);
    static void salvar_grafo(Grafo* grafo, string nome_arquivo);
    static void imprimir_grafo(Grafo* grafo);
    static void salvar_lista_nos(vector<char> lista, string nome_arquivo);
    static void imprimir_lista_nos(vector<char> lista);
    static void imprimir_lista_adj(Grafo* grafo);
    static void salvar_lista_adj(Grafo* grafo, string nome_arquivo);
    static void salvar_letraH(Grafo* grafo, string nome_arquivo);
    static void run_tests(Grafo* grafo, ofstream& file);
};


#endif //GERENCIADOR_H
