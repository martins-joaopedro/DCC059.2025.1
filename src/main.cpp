#include "Gerenciador.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

int main(int argc, char *argv[]) {
    
    for(int i=1; i<argc; i++) {
        string path = argv[i];
        cout << path << endl;
        
        Grafo* grafo = Grafo::ler_grafo(path);
        Gerenciador::imprimir_grafo(grafo);
        Gerenciador::comandos(grafo);

        delete grafo;
    }
    
    return 0;
}