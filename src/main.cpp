#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <algorithm>
#include <limits>
#include <cmath>
#include <string>
#include <iomanip>

using namespace std;

class Grafo
{
private:
    // estrutura grafp
    int num_vertices;
    set<int> vertices;
    vector<pair<int, int>> arestas;          // Arestas
    vector<pair<int, int>> arcos;            // Arestas direcionadas
    map<pair<int, int>, double> custos;      // Custo para cada aresta/arco
    map<pair<int, int>, double> demandas;    // Demanda para cada aresta/arco
    map<int, vector<pair<int, double>>> adj; // Lista de adjacência com pesos

    // obrigatórios
    set<int> vertices_requeridos;
    set<pair<int, int>> arestas_requeridas;
    set<pair<int, int>> arcos_requeridos;

    // deposito inicial
    int deposito;

    string nome;

public: 
    Grafo() : num_vertices(0), deposito(0) {}

    void lerArquivoDados(const string &nome_arquivo)
    {
        ifstream arquivo(nome_arquivo);
        if (!arquivo.is_open())
        {
            cerr << "Erro ao abrir o arquivo: " << nome_arquivo << endl;
            return;
        }

        string linha;
        string campo;

        // Ler o nome da instância
        getline(arquivo, linha);
        sscanf(linha.c_str(), "Name:\t%s", &campo[0]);
        nome = campo;

        cout << linha;

        // Pular linhas desnecessárias
        getline(arquivo, linha); // Optimal value
        getline(arquivo, linha); // #Vehicles  
        getline(arquivo, linha); // #Capacity
    }
    
};

int main()
{
    Grafo grafo;
    grafo.lerArquivoDados("../../data/BHW1.dat");
    
    return 0;
}