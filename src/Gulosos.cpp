// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <vector>
// #include <map>
// #include <set>
// #include <cstdlib>
// #include <ctime>
// #include <algorithm>
// //#include <windows.h>

// #include "Gerenciador.h"
// #include "Gulosos.h"
// //#include "main.cpp" //tirar depois
// using namespace std;

// void initializes_arrays(vector<float>& P, vector<float>& M, vector<int>& count, int m){
//     for(int i=0; i<m; i++){
//         P[i] = 0;
//         M[i] = 0;
//         count[i] = 0;
//     }
// }

// void updates_probability(vector<float>& P, vector<float>& M, int m){
//     vector<float> scores;//transfoma media em pontuação, menores medias tem pontuação maior
//     float sum_scores=0;//soma dos sores de todos os alphas
//     int i;

//     for(i=0; i<m; i++){
//         scores[i] = 1/M[i];
//         sum_scores += scores[i];
//     }

//     for(i=0; i<m; i++){
//         P[i] = scores[i]/sum_scores;
//     }
// }

// void updates_means(vector<float>& M, vector<int>& count, vector<char>& s, int i){
//     // int i = 0;
//     // M[i] = (M[i] * count[i] + s.size()) / (count[i] + 1);
//     M[i] = (M[i] * count[i] + s.size()) / (count[i] + 1);
// }

// //escolhe o alfa com maior probabilidade
// int choses_alpha(vector<float>& P){
//     auto it = max_element(P.begin(), P.end());//acha maior valor de probabilidade
//     int index = -1;

//     if (it != P.end()){
//         index = distance(P.begin(), it);//acha o index do maior valor
//     }
//     return index;
// }

// //algoritmo randomizado adaptativo reativo
// vector<char> randomized_adaptative_reactive_greedy(Grafo* grafo, vector<float>& alphas, int m, int nIter, int bloco){
//     vector<char> solBest, s, LC;
//     int k, i=1;

//     vector<float> P (m,0); //probabilidade de alphas
//     vector<float> M (m,0);//média da qualidade
//     vector<int> count (m,0);//contagem de iterações de cada alpha
//     vector<int> Q;//qualidadde de cada solução

//     //initializes_arrays(P, M, count, m);

//     while(i < nIter){
//         if(i != 0 && i % bloco == 0){//atualiza prob
//             updates_probability(P,M,m);
//         }
//         i++;
//         cout<< "i: "<<i<< endl;
//         int index_alpha = 1;//choses_alpha(P);
//         count[i]++;
//         s = randomized_adaptative_greedy(grafo, alphas[index_alpha]);
        
//         bool isValid = check_validity(s, grafo);

//         if(isValid){
//             updates_means(M,count,s,index_alpha);
//             if(s.size() < solBest.size())
//                 solBest = s;
//         }
//         else cout << "SOLUÇÃO INVALIDA" << endl;


//         // LC = {};//ordenaCandidatos
//         // i++;
//         // s ={};
//         // int alpha = choses_alpha(P);
        
//         // while(true){
//         //     cout<<"guloso!!!!!!!"<<endl;//aqui chama o randomizado
//         //     break;
//         // } 
        
//         // atualizaMedias(M, s, alpha );
        
//         // if (s.val < solBest.val)
//         //     solBest = s;
//     }


//     return solBest;
// }

// void teste(){
//     // int m = 5;
//     // vector<char> s = randomized_adaptative_reactive_greedy()
// }


// vector<char> heuristic(vector<char> LC, map<char, No*>& mapa_nos, int k) {

//     map<char, int> degree;
    
//     if(k == 0) {
//         // calcula o grau de cada nó
//         for(char no : LC)
//             degree[no] = mapa_nos[no]->arestas.size();
        
//     } else  {
//         // calcula o quanto cada grau pode dominar na lista de candidatos
//         for(char no : LC) {
//             int domain = 1; // considera ele mesmo
//             for(Aresta* aresta : mapa_nos[no]->arestas) {
//                 if(!mapa_nos[aresta->id_no_alvo]->dominado)
//                     domain++;
//             }
//             degree[no] = domain;
//         }
//     }

//     // reordena os nós por grau e remonta a LC
//     LC.clear();

//     cout << "\n=> LC: " << k << endl;
//     while (!degree.empty()) {
//         auto max_it = degree.begin();
//         for (auto it = degree.begin(); it != degree.end(); ++it) {
//             if (it->second > max_it->second)
//                 max_it = it;
//         }

//         LC.push_back(max_it->first);
//         cout << "(" << max_it->first << " ; " << max_it->second << ") ";

//         // remove do map pós iteração
//         degree.erase(max_it);
//     } 
//     cout << endl;

//     return LC;
// }

// void updates_domain(map<char, No*>& mapa_nos, vector<char> S) {
//     for (char no : S) {
//         mapa_nos[no]->dominado = true;
//         for(Aresta* aresta : mapa_nos[no]->arestas)
//             mapa_nos[aresta->id_no_alvo]->dominado = true;
//     }
// }

// // lógica adaptativa de atualização da lista LC 
// vector<char> updates_LC(map<char, No*>& mapa_nos, vector<char> LC, vector<char> S) {
    
//     set<char> neighborhood = set<char>();

//     // critério de atualização: removo os nós vizinhos de v da LC
//     for (char no : S) {
//         for (Aresta* aresta : mapa_nos[no]->arestas) {
//             char neighbor = aresta->id_no_alvo;
//             neighborhood.insert(neighbor);
//         }
//     }

//     vector<char> new_LC = vector<char>();

//     for (char no : LC) {
//         // se ele for vizinho de algum nó em S, não o adiciono
//         if(find(neighborhood.begin(), neighborhood.end(), no) != neighborhood.end())
//             continue;
        
//         // nao posso recolocar candidatos de S
//         if(find(S.begin(), S.end(), no) != S.end())
//             continue;

//         new_LC.push_back(no);
//     }

//     return new_LC;
// }

// // algoritmo adaptativo guloso randomizado
// vector<char> randomized_adaptative_greedy(Grafo* grafo, float alfa) {
    
//     bool debug = true;
//     map<char, No*> mapa_nos;
//     vector<char> LC;

//     // monta o mapa de nos para facilitar e a lista de candidatos
//     for (No* no : grafo->lista_adj) {
//         no->dominado = false; // reseta a dominancia para não influenciar em outras itr
//         mapa_nos[no->id] = no;
//         LC.push_back(no->id);
//     }

//     // euristica: inicialmente ordena os candidatos pelo grau do no
//     // depois para garantir dominancia, pelo grau de cobertura, ambos de forma crescente
//     int k = 0;
//     LC = heuristic(LC, mapa_nos, k);
//     vector<char> S = vector<char>(); 

//     while(!LC.empty()) {
        
//         k++;
//         int LCR = max(1, int(alfa * LC.size()));
//         int randomized = rand() % LCR;

//         // seleciona o no e LC <- LC - {v}
//         char no = LC[randomized];
//         LC.erase(LC.begin() + randomized);
        
//         if(debug)
//             cout << "Escolhendo no: " << no  << endl;
        
//         S.push_back(no);

//         // atualiza dominancia dos nós
//         updates_domain(mapa_nos, S);
        
//         // garanto todas as condições para independencia 
//         LC = updates_LC(mapa_nos, LC, S);
        
//         // reorganiza a LC
//         LC = heuristic(LC, mapa_nos, k);

//         if(debug) {
//             cout << "[ S:  " << k << " ]" << endl;
//             for (char no : S)
//                 cout << no << " ";
//             cout << endl;
//         }

//         if(debug) {
//             cout << "[ LC:  " << k << " ]" << endl;
//             for (char no : LC)
//                 cout << no << " ";
//             cout << endl;
//         }
//     }

//     return S;
// }

// bool check_validity(vector<char> S, Grafo* grafo) {

//     set<char> domain = set<char>();
//     set<char> dominated = set<char>();

//     // para cada no de S ver se ele domina todos
//     for(char v : S)
//         for(No* no : grafo->lista_adj) {
//             domain.insert(no->id); // sempre que eu buscar por um no
//             if(v == no->id) {
//                 dominated.insert(no->id); // sempre que eu achar um nó e tudo que ele domina
//                 for(Aresta* aresta : no->arestas)
//                     dominated.insert(aresta->id_no_alvo);
//             }
            
//         }
    
//     // TEST: verifica a independencia
//     // S.push_back('a');

//     // ver se para todos da solução é independente
//     bool cond = true; 
//     for (auto v : S) 
//         for(No* no : grafo->lista_adj)
//             if(v == no->id)
//                 for(Aresta* aresta : no->arestas)
//                     if(find(S.begin(), S.end(), aresta->id_no_alvo) != S.end()) {
//                         cond = false;
//                         cout << "(X) nao eh independente" << endl;
//                         break;
//                     }
                
//     cout << "Vertices: ";
//     for (auto no : domain) cout << no << " ";

//     cout << "\nDominados: ";
//     for (auto no : domain) cout << no << " ";

//     cout << "\nSolucao: ";
//     for (auto no : S) cout << no << " ";
//     cout << "\n";

//     if (domain == dominated && cond) {
//         cout << "===> Solucao VALIDA! |S| = " << S.size() << "\n";
//         return true;
//     }
//     else
//         cout << "===> Solucao INVALIDA\n";
//     return false;
// }

// void run2(Grafo* grafo) {
//     map<float, vector<int>> sols;
//     float alphas[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6};
//     int MAX_IT = 1;
//     srand(time(0));

//     for (auto alpha : alphas) {
//         //setColor(1 + alpha * 10);
//         cout << "Alpha: " << alpha << endl;
//         sols[alpha] = vector<int>();
//         for (int i=0; i<MAX_IT; i++) {
//             cout << "\n[ Iteracao: " << i+1 << " ]" << endl;
//             vector<char> S = randomized_adaptative_greedy(grafo, alpha);
//             bool isValid = check_validity(S, grafo);

//             if(isValid)
//                 sols[alpha].push_back(S.size());
//             else cout << "SOLUÇÃO INVALIDA" << endl;
//         }
//         cout << endl;
//     }

//     for(auto alpha : alphas ) {
//         cout << "[ Alpha: " << alpha << " ] ";
//         for(auto S_size : sols[alpha])
//             cout << S_size << " ";
//         cout << endl;
//     } 

// }