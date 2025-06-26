#include "Gerenciador.h"
#include <fstream>
#include <iostream>
#include <math.h>

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
    cout<<"(0) Sair;"<<endl<<endl;

    char resp;
    cin >> resp;
    switch (resp) {
        case 'a': {

            char id_no = get_id_entrada();
            vector<char> fecho_transitivo_direto = grafo->fecho_transitivo_direto(id_no);
            
            if(!fecho_transitivo_direto.empty()){
                cout << "Fecho transitivo direto de " << id_no << " :" << endl;
                grafo->imprime_fecho(fecho_transitivo_direto);
            }
            
            if(pergunta_imprimir_arquivo("fecho_trans_dir.txt")) {
                cout<<"Metodo de impressao em arquivo nao implementado"<<endl<<endl;
            }


            break;
        }

        case 'b':{

            char id_no = get_id_entrada();
            vector<char> fecho_transitivo_indireto = grafo->fecho_transitivo_indireto(id_no);
            cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

            if(pergunta_imprimir_arquivo("fecho_trans_indir.txt")) {
                salvar_lista(fecho_transitivo_indireto, "fecho_trans_indir.txt");
            }

;
            break;
        }

        case 'c': {

            char id_no_1 = get_id_entrada();
            char id_no_2 = get_id_entrada();
            vector<char> caminho_minimo_dijkstra = grafo->caminho_minimo_dijkstra(id_no_1,id_no_2);
            cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

            if(pergunta_imprimir_arquivo("caminho_minimo_dijkstra.txt")) {
                cout<<"Metodo de impressao em arquivo nao implementado"<<endl;
            }


            break;
        }

        case 'd': {

            char id_no_1 = get_id_entrada();
            char id_no_2 = get_id_entrada();
            vector<char> caminho_minimo_floyd = grafo->caminho_minimo_floyd(id_no_1,id_no_2);
            cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

            if(pergunta_imprimir_arquivo("caminho_minimo_floyd.txt")) {
                cout<<"Metodo de impressao em arquivo nao implementado"<<endl;
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
                imprimir_grafo(arvore_geradora_minima_prim);

                if(pergunta_imprimir_arquivo("agm_prim.txt")) {
                    salvar_grafo(arvore_geradora_minima_prim, "agm_prim.txt");
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
                imprimir_grafo(arvore_geradora_minima_kruskal);

                if(pergunta_imprimir_arquivo("agm_kruskal.txt")) {
                    salvar_grafo(arvore_geradora_minima_kruskal, "agm_kruskal.txt");
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
            imprimir_grafo(arvore_caminhamento_profundidade);

            if(pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt")) {
                salvar_grafo(arvore_caminhamento_profundidade, "arvore_caminhamento_profundidade.txt");
            }

            delete arvore_caminhamento_profundidade;
            break;
        }

        case 'h': {
            vector<char> articulacao = grafo->vertices_de_articulacao();
            cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

            if(pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt")) {
                salvar_lista(articulacao, "vertices_de_articulacao.txt");
            }

            break;
        }
        case 'i': {
            
            vector<char> articulacao = grafo->vertices_de_articulacao();
            cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

            if(pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt")) {
                salvar_lista(articulacao, "vertices_de_articulacao.txt");
            }

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

// Função auxiliar para ordenar as arestas pelo id do nó alvo
bool comparar_arestas(Aresta* a, Aresta* b) {
    return a->id_no_alvo < b->id_no_alvo;
}

void Gerenciador::salvar_grafo(Grafo* grafo, string nome_arquivo) {
    // pra ficar no inicio dos arquivos
    ofstream arquivo("0_"+nome_arquivo);

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

    // ordena as arestas de cada nó pelo id do nó alvo
    for (No* no : nos_ordenados) {
        vector<Aresta*> arestas_ordenadas = no->arestas;
        sort(arestas_ordenadas.begin(), arestas_ordenadas.end(), comparar_arestas);
        
        for (Aresta* aresta : arestas_ordenadas) {
            
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
}

void Gerenciador::imprimir_grafo(Grafo* grafo) {

    if(grafo != nullptr) {
        cout << "\n----------------------------------------" << endl;
        cout << "Grafo:" << endl;
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
            vector<Aresta*> arestas_ordenadas = no->arestas;
            sort(arestas_ordenadas.begin(), arestas_ordenadas.end(), comparar_arestas);
            
            for (Aresta* aresta : arestas_ordenadas) {
                
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
    }
}

void Gerenciador::salvar_lista(vector<char> lista, string nome_arquivo) {
    // pra ficar no inicio dos arquivos
    ofstream arquivo("0_"+nome_arquivo);

    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo: " << nome_arquivo << endl;
        return;
    }

    arquivo << lista.size() << "\n";

    for(char c : lista) {
        arquivo << c;
    }

    cout << "Lista salva em " << nome_arquivo << endl;
    arquivo.close();
}

void Gerenciador::imprimir_lista(vector<char> lista) {

    cout << lista.size() << "\n";

    for(char c : lista) {
        cout << c;
    }

}