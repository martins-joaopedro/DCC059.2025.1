#include "Gerenciador.h"
#include "Gulosos.h"
#include <fstream>
#include <iostream>
#include <math.h>
#include <set>
#include <ctime>

void Gerenciador::comandos(Grafo* grafo) {
    cout << "\n----------------------------------------" << endl;
    cout << "Comandos disponiveis:" << endl;
    cout<<"Digite uma das opcoes abaixo e pressione enter:"<<endl<<endl;
    cout<<"(a) Fecho transitivo direto de um no;"<<endl;
    cout<<"(b) Fecho transitivo indireto de um no;"<<endl;
    cout<<"(c) Caminho minimo (Djikstra);"<<endl;
    cout<<"(d) Caminho minimo (Floyd);"<<endl;
    cout<<"(e) Arvore Geradora Minima (Algoritmo de Prim);"<<endl;
    cout<<"(f) Arvore Geradora Minima (Algoritmo de Kruskal);"<<endl;
    cout<<"(g) Arvore de caminhamento em profundidade;"<<endl;
    cout<<"(h) Raio, diametro, centro e periferia do grafo;"<<endl;
    cout<<"(i) Algortimo guloso;"<<endl;
    cout<<"(j) Algortimo guloso randomizado adaptativo;"<<endl;
    cout<<"(k) Algortimo guloso randomizado adaptativo reativo;"<<endl;
    cout<<"(0) Sair;"<<endl<<endl;

    char resp;
    cin >> resp;
    switch (resp) {
        case 'a': {

            char id_no = get_id_entrada();
            vector<char> fecho_transitivo_direto = grafo->fecho_transitivo_direto(id_no);
            
            imprimir_lista_nos(fecho_transitivo_direto);

            if(pergunta_imprimir_arquivo("fecho_trans_dir.txt")) 
                salvar_lista_nos(fecho_transitivo_direto, "fecho_trans_dir.txt");
            
            break;
        }

        case 'b':{

            char id_no = get_id_entrada();
            vector<char> fecho_transitivo_indireto = grafo->fecho_transitivo_indireto(id_no);
            
            imprimir_lista_nos(fecho_transitivo_indireto);

            if(pergunta_imprimir_arquivo("fecho_trans_indir.txt"))
                salvar_lista_nos(fecho_transitivo_indireto, "fecho_trans_indir.txt");
    
            break;
        }

        case 'c': {

            char id_no_1 = get_id_entrada();
            char id_no_2 = get_id_entrada();
            vector<char> caminho_minimo_dijkstra = grafo->caminho_minimo_dijkstra(id_no_1,id_no_2);
            


            imprimir_lista_nos(caminho_minimo_dijkstra);

            if(pergunta_imprimir_arquivo("caminho_minimo_dijkstra.txt")) {
                salvar_lista_nos(caminho_minimo_dijkstra, "caminho_minimo_dijkstra.txt");
            }

            break;
        }

        case 'd': {

            char id_no_1 = get_id_entrada();
            char id_no_2 = get_id_entrada();
            vector<char> caminho_minimo_floyd = grafo->caminho_minimo_floyd(id_no_1,id_no_2);
            
            imprimir_lista_nos(caminho_minimo_floyd);

            if(pergunta_imprimir_arquivo("caminho_minimo_floyd.txt")) {
                salvar_lista_nos(caminho_minimo_floyd, "caminho_minimo_floyd.txt");
            }

            break;
        }
        
        case 'e': {

            int tam;
            cout<<"Digite o tamanho do subconjunto: ";
            cin>>tam;

            if(tam > 0 && tam <= grafo->ordem) {

                vector<char> ids = get_conjunto_ids(grafo,tam);
                Grafo* arvore_geradora_minima_prim = grafo->arvore_geradora_minima_prim(ids);

                imprimir_lista_adj(arvore_geradora_minima_prim);
                
                if(pergunta_imprimir_arquivo("agm_prim.txt")) {
                    salvar_lista_adj(arvore_geradora_minima_prim, "agm_prim.txt");
                }

                delete arvore_geradora_minima_prim;

            }else {
                cout<<"Valor invalido"<<endl;
            }

            break;
        }

        case 'f': {

            int tam;
            cout<<"Digite o tamanho do subconjunto: ";
            cin>>tam;

            if(tam > 0 && tam <= grafo->ordem) {

                vector<char> ids = get_conjunto_ids(grafo,tam);
                Grafo* arvore_geradora_minima_kruskal = grafo->arvore_geradora_minima_kruskal(ids);
                
                imprimir_lista_adj(arvore_geradora_minima_kruskal);

                if(pergunta_imprimir_arquivo("agm_kruskal.txt")) {
                    salvar_lista_adj(arvore_geradora_minima_kruskal, "agm_kruskal.txt");
                }

                delete arvore_geradora_minima_kruskal;

            }else {
                cout<<"Valor invalido"<<endl;
            }

            break;
        }

        case 'g': {

            char id_no = get_id_entrada();
            Grafo* arvore_caminhamento_profundidade = grafo->arvore_caminhamento_profundidade(id_no);
            
            imprimir_lista_adj(arvore_caminhamento_profundidade);

            if(pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt")) {
                salvar_lista_adj(arvore_caminhamento_profundidade, "arvore_caminhamento_profundidade.txt");
            }

            delete arvore_caminhamento_profundidade;
            break;
        }

        case 'h': {

            grafo->calcula_caracteristicas();
            
            cout<<"--------RAIO: "<<grafo->raio;            
            cout<<endl<<"--------CENTRO: ";
            for (char v : grafo->centro)
                cout << v << " ";  
            
            cout<<endl<<"--------DIAMETRO: "<<grafo->diametro;            
            cout<<endl<<"--------PERIFERIA: ";
            for (char v : grafo->periferia)
                cout << v << " ";
            cout<<endl<<endl;
            
            if(pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt")) {
                salvar_letraH(grafo, "letraH.txt");
            }

            break;
        }

        case 'i': {
            string output = "output/Guloso.txt";
            ofstream file = ofstream(output);

            file << "\n__________________________________________________" << endl;
            file << "GULOSO" << endl;
            clock_t start_time = clock();
            
            Gulosos::run_greedy(grafo, file);
            
            
            clock_t end_time = clock();
            double elapsed_time = double(end_time - start_time) / CLOCKS_PER_SEC;
            
            file << "Tempo de execucao: " << elapsed_time << " segundos" << endl;
            file.close();

            cout << "\n>>>> Saida em " << output << " <<<<\n";

            break;
        }

        case 'j': {
            string output = "output/Randomizado.txt";
            ofstream file = ofstream(output);

            file << "\n__________________________________________________" << endl;
            file << "GULOSO RANDOMIZADO" << endl;
            clock_t start_time = clock();
            
            Gulosos::run_randomized_adaptive_greedy(grafo, file);
            
            clock_t end_time = clock();
            double elapsed_time = double(end_time - start_time) / CLOCKS_PER_SEC;
            
            file << "Tempo de execucao: " << elapsed_time << " segundos" << endl;
            file.close();

            cout << "\n>>>> Saida em " << output << " <<<<\n";

            break;
        }

        case 'k': {
            string output = "output/Reativo.txt";
            ofstream file = ofstream(output);

            file << "\n__________________________________________________" << endl;
            file << "GULOSO REATIVO" << endl;
            clock_t start_time = clock();
            
            Gulosos::run_randomized_adaptative_reactive_greedy(grafo, file);
            
            clock_t end_time = clock();
            double elapsed_time = double(end_time - start_time) / CLOCKS_PER_SEC;
            
            file << "Tempo de execucao: " << elapsed_time << " segundos" << endl;
            file.close();

            cout << "\n>>>> Saida em " << output << " <<<<\n";

            break;
        }

        case '0': {
            exit(0);
        }

        default: {
            cout<<"Opcao invalida"<<endl;
        }
    }

    comandos(grafo);

}

char Gerenciador::get_id_entrada() {
    cout<<"Digite o id de um no: ";
    char id;
    cin>>id;
    cout<<endl;
    return id;
}

vector<char> Gerenciador::get_conjunto_ids(Grafo *grafo, int tam) {
    vector<char> ids = {};
    while((int)ids.size() < tam) {
        char id_no =get_id_entrada();
        bool existe = false;
        for(No* no: grafo->lista_adj){
            if(no->id == id_no){
                existe = true;
                break;
            }
        }

        if(!existe){
            cout<<"Vertice nao existe"<<endl;
        }else{
            bool repetido = find(ids.begin(), ids.end(),id_no) != ids.end();
            if(repetido) {
                    cout<<"Valor repetido"<<endl;
            }else {
                ids.push_back(id_no);
            }
        }

    }

    return ids;
}


bool Gerenciador::pergunta_imprimir_arquivo(string nome_arquivo) {

    cout<<"Imprimir em arquivo externo? ("<<nome_arquivo<<")"<<endl;
    cout<<"(1) Sim;"<<endl;
    cout<<"(2) Nao."<<endl;
    int resp;
    cin>>resp;
    cout<<endl;

    switch (resp) {
        case 1:
            return true;
        case 2:
            return false;
        default:
            cout<<"Resposta invalida"<<endl;
            return pergunta_imprimir_arquivo(nome_arquivo);
    }
}

// SALVAR EM ARQUIVO
bool comparar_nos(No* a, No* b) {
    return a->id < b->id;
}

void Gerenciador::salvar_grafo(Grafo* grafo, string nome_arquivo) {
    
    if(grafo != nullptr) {
       
        // pra ficar no inicio dos arquivos
        ofstream arquivo("output/"+nome_arquivo);
    
        if (!arquivo.is_open()) {
            cout << "Erro ao abrir o arquivo: " << nome_arquivo << endl;
            return;
        }
    
        arquivo << grafo->in_direcionado << " "
                << grafo->in_ponderado_aresta << " "
                << grafo->in_ponderado_vertice << "\n";
    
        // ajuda pra caso alguma arvore tenha tamanho menor
        int ordem = grafo->lista_adj.size() < grafo->ordem ? grafo->lista_adj.size() : grafo->ordem;
        arquivo << ordem << "\n";
    
        // ordena a lista de nós pelo id
        vector<No*> nos_ordenados = grafo->lista_adj;
        sort(nos_ordenados.begin(), nos_ordenados.end(), comparar_nos);
    
        for (No* no : nos_ordenados) {
            
            string conteudo = string(1, no->id);
            // so adiciona o peso se o grafo for ponderado
            if(grafo->in_ponderado_vertice) {
                conteudo += " " + to_string(no->peso) + "\n";
            } else {
                conteudo += "\n";
            }
            arquivo << conteudo;
        }
    
        for (No* no : nos_ordenados) {
    
            for (Aresta* aresta : no->arestas) {
                
                string conteudo = string(1, no->id) + " " + string(1, aresta->id_no_alvo);
                
                // so adiciona o peso se a aresta for ponderada
                if(grafo->in_ponderado_aresta) {
                    conteudo += " " + to_string(aresta->peso);    
                } 
                if(aresta->retorno) {
                    conteudo += " (retorno)\n"; 
                } else conteudo += "\n";
                
                arquivo << conteudo;
            }
        }
    
        cout << "Grafo salvo em " << nome_arquivo << endl;
        arquivo.close();
    } else cout << "Grafo vazio." << endl;
}

void Gerenciador::imprimir_grafo(Grafo* grafo) {

    if(grafo != nullptr) {
        cout << "\n----------------------------------------" << endl;

        cout << grafo->in_direcionado << " "
            << grafo->in_ponderado_aresta << " "
            << grafo->in_ponderado_vertice << "\n";

        int ordem = grafo->lista_adj.size() < grafo->ordem ? grafo->lista_adj.size() : grafo->ordem;
        cout << ordem << "\n";
    
        // ordena a lista de nós pelo id
        vector<No*> nos_ordenados = grafo->lista_adj;
        sort(nos_ordenados.begin(), nos_ordenados.end(), comparar_nos);
    
        for (No* no : nos_ordenados) {
            
            string conteudo = string(1, no->id);
    
            if(grafo->in_ponderado_vertice) {
                conteudo += " " + to_string(no->peso) + "\n";
            } else {
                conteudo += "\n";
            }
            cout << conteudo;
        }
    
        // ordena as arestas de cada nó pelo id do nó alvo
        for (No* no : nos_ordenados) {
         
            for (Aresta* aresta : no->arestas) {
                
                string conteudo = string(1, no->id) + " " + string(1, aresta->id_no_alvo);
                
                // so adiciona o peso se a aresta for ponderada
                if(grafo->in_ponderado_aresta) {
                    conteudo += " " + to_string(aresta->peso);    
                } 
                if(aresta->retorno) {
                    conteudo += " (retorno)\n"; 
                } else conteudo += "\n";
                
                cout << conteudo;
            }
        }
    } cout << "Grafo vazio." << endl;
    cout << endl;
}

void Gerenciador::salvar_lista_nos(vector<char> lista, string nome_arquivo) {

    if(!lista.empty()) {
        // pra ficar no inicio dos arquivos
        ofstream arquivo("output/" + nome_arquivo);
    
        if (!arquivo.is_open()) {
            cout << "Erro ao abrir o arquivo: " << nome_arquivo << endl;
            return;
        }
    
        arquivo << lista.size() << "\n";
        
        int k = 0;
        for(char c : lista) {
            k++;
            arquivo << c;
            if(k < lista.size())
                arquivo << ", ";
        }
        cout << "Lista salva em " << nome_arquivo << endl;
        arquivo.close();
    } else cout << "Lista vazia." << endl;
}

void Gerenciador::imprimir_lista_nos(vector<char> lista) {

    cout << lista.size() << "\n";

    if (!lista.empty()) {
        int k = 0;
        for(char c : lista) {
            k++;
            cout << c;
            if(k < lista.size())
                cout << ", ";
        }
        cout << endl;
    } else cout << "Lista vazia." << endl;
}


void Gerenciador::salvar_lista_adj(Grafo* grafo, string nome_arquivo) {

    if(grafo != nullptr) {
        ofstream arquivo("output/"+nome_arquivo);

        // ordena a lista de nós pelo id
        vector<No*> nos_ordenados = grafo->lista_adj;
        sort(nos_ordenados.begin(), nos_ordenados.end(), comparar_nos);
    
        // ordena as arestas de cada nó pelo id do nó alvo
        for (No* no : nos_ordenados) {
            arquivo << no->id << ": ";
            int k = 0;
            for (Aresta* aresta : no->arestas) {
                k++;
                arquivo << aresta->id_no_alvo;
                if(k < no->arestas.size())
                    arquivo << " -> ";
            }
            arquivo << endl;
        }
        cout << "Lista de Adjacências salva em " << nome_arquivo << endl;
    } else cout << "Grafo vazio." << endl;
    cout << endl;
}

void Gerenciador::imprimir_lista_adj(Grafo* grafo) {

    if(grafo != nullptr) {
        // ordena a lista de nós pelo id
        vector<No*> nos_ordenados = grafo->lista_adj;
        sort(nos_ordenados.begin(), nos_ordenados.end(), comparar_nos);
    
        // ordena as arestas de cada nó pelo id do nó alvo
        for (No* no : nos_ordenados) {
            cout << no->id << ": ";
            int k = 0;
            for (Aresta* aresta : no->arestas) {
                k++;
                cout << aresta->id_no_alvo;
                if(k < no->arestas.size())
                    cout << " -> ";
            }
            cout << endl;
        }
    } else cout << "Grafo vazio." << endl;
    cout << endl;
}

void Gerenciador::salvar_letraH(Grafo* grafo, string nome_arquivo){
    if(grafo != nullptr) {
        ofstream arquivo("output/"+nome_arquivo);

        arquivo << "Raio: " << grafo->raio << endl << "Centro: ";

        for(char id : grafo->centro){
            arquivo << id << ", ";
        }

        arquivo << endl;
        
        arquivo << "Diametro: " << grafo->diametro << endl << "Periferia: ";

        for(char id : grafo->periferia){
            arquivo << id << ", ";
        }
        arquivo << endl;

        cout << "Dados salvos em " << nome_arquivo << endl;
    } else cout << "Grafo vazio." << endl;
    cout << endl;
}

void Gerenciador::run_tests(Grafo* grafo, ofstream& file) {
    
    auto medir_tempo = [&](auto func, Grafo* g, ofstream& f) {
    double soma_tempos = 0.0;
    for (int i = 0; i < 10; i++) {
        srand(7*i); // semente diferente a cada execução

        // Cria uma cópia limpa do grafo para não vazar estado
        Grafo copia = *g; // se tiver cópia profunda implementada
        Gulosos gulosos;
        gulosos.resetar_dominacao(&copia);

        clock_t start_time = clock();
        func(&copia, f);
        clock_t end_time = clock();

        soma_tempos += double(end_time - start_time) / CLOCKS_PER_SEC;
    }
    return soma_tempos / 10.0;
};

    file << "\n__________________________________________________" << endl;
    file << "GULOSO" << endl;
    double media_guloso = medir_tempo(Gulosos::run_greedy, grafo, file);
    file << "Tempo medio de execucao: " << media_guloso << " segundos" << endl;

    file << "\n__________________________________________________" << endl;
    file << "GULOSO RANDOMIZADO" << endl;
    double media_randomizado = medir_tempo(Gulosos::run_randomized_adaptive_greedy, grafo, file);
    file << "Tempo medio de execucao: " << media_randomizado << " segundos" << endl;

    file << "\n__________________________________________________" << endl;
    file << "GULOSO REATIVO" << endl;
    double media_reativo = medir_tempo(Gulosos::run_randomized_adaptative_reactive_greedy, grafo, file);
    file << "Tempo medio de execucao: " << media_reativo << " segundos" << endl;
}
