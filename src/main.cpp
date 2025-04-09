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
    // estrutura grafo
    int num_vertices;
    set<int> vertices;
    vector<pair<int, int>> arestas;          // Arestas
    vector<pair<int, int>> arcos;            // Arestas direcionadas
    map<pair<int, int>, double> custos;      // Custo para cada aresta/arco
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

        // Nome da instância
        getline(arquivo, linha);
        nome = linha.substr(linha.find_first_of(":") + 1);
        nome = nome.substr(nome.find_first_not_of(" \t"));

        getline(arquivo, linha); // Optimal value
        getline(arquivo, linha); // #Vehicles
        getline(arquivo, linha); // Capacity

        // Depósito
        getline(arquivo, linha);
        deposito = stoi(linha.substr(linha.find_first_of(":") + 1));

        // Número de nós
        getline(arquivo, linha);
        num_vertices = stoi(linha.substr(linha.find_first_of(":") + 1));

        // Número de arestas e arcos
        int num_arestas, num_arcos;
        getline(arquivo, linha);
        num_arestas = stoi(linha.substr(linha.find_first_of(":") + 1));

        getline(arquivo, linha);
        num_arcos = stoi(linha.substr(linha.find_first_of(":") + 1));

        // Quantidade de elementos requeridos
        int num_vertices_requeridos, num_arestas_requeridas, num_arcos_requeridos;

        getline(arquivo, linha);
        num_vertices_requeridos = stoi(linha.substr(linha.find_first_of(":") + 1));

        getline(arquivo, linha);
        num_arestas_requeridas = stoi(linha.substr(linha.find_first_of(":") + 1));

        getline(arquivo, linha);
        num_arcos_requeridos = stoi(linha.substr(linha.find_first_of(":") + 1));

        for (int i = 1; i <= num_vertices; i++)
        {
            vertices.insert(i);
        }

        // Ler até encontrar "ReN."
        while (getline(arquivo, linha) && linha.find("ReN.") == string::npos)
            ;

        // Ler nós requeridos
        for (int i = 0; i < num_vertices_requeridos; i++)
        {
            getline(arquivo, linha);
            stringstream ss(linha);
            string nome_no;
            int demanda, custo_servico;

            ss >> nome_no >> demanda >> custo_servico;

            int node = stoi(nome_no.substr(1)); // Remove o 'N'
            vertices_requeridos.insert(node);
        }

        // Ler até encontrar "ReE."
        while (getline(arquivo, linha) && linha.find("ReE.") == string::npos)
            ;

        // Ler arestas requeridas
        for (int i = 0; i < num_arestas_requeridas; i++)
        {
            getline(arquivo, linha);
            stringstream ss(linha);
            string edge_name;
            int u, v;
            double custo_travessia, demanda, custo_servico;

            ss >> edge_name >> u >> v >> custo_travessia >> demanda >> custo_servico;

            arestas.push_back({u, v});
            custos[{u, v}] = custo_travessia;
            arestas_requeridas.insert({u, v});
            adj[u].push_back({v, custo_travessia});
            adj[v].push_back({u, custo_travessia});
        }

        // Pular até "EDGE"
        while (getline(arquivo, linha) && linha.find("EDGE") == string::npos)
            ;

        // Ler arestas não requeridas
        for (int i = 0; i < num_arestas - num_arestas_requeridas; i++)
        {
            getline(arquivo, linha);
            if (linha.empty())
                break;

            stringstream ss(linha);
            string edge_name;
            int u, v;
            double custo_travessia;

            ss >> edge_name >> u >> v >> custo_travessia;

            arestas.push_back({u, v});
            custos[{u, v}] = custo_travessia;
            adj[u].push_back({v, custo_travessia});
            adj[v].push_back({u, custo_travessia});
        }

        // Pular até "ReA."
        while (getline(arquivo, linha) && linha.find("ReA.") == string::npos)
            ;

        // Arcos requeridos
        for (int i = 0; i < num_arcos_requeridos; i++)
        {
            getline(arquivo, linha);
            stringstream ss(linha);
            string arc_name;
            int u, v;
            double custo_travessia, demanda, custo_servico;

            ss >> arc_name >> u >> v >> custo_travessia >> demanda >> custo_servico;

            arcos.push_back({u, v});
            custos[{u, v}] = custo_travessia;
            arcos_requeridos.insert({u, v});
            adj[u].push_back({v, custo_travessia});
        }

        // Pular até "ARC"
        while (getline(arquivo, linha) && linha.find("ARC") == string::npos)
            ;

        // Arcos não requeridos
        for (int i = 0; i < num_arcos - num_arcos_requeridos; i++)
        {
            getline(arquivo, linha);
            if (linha.empty())
                break;

            stringstream ss(linha);
            string arc_name;
            int u, v;
            double custo_travessia;

            ss >> arc_name >> u >> v >> custo_travessia;

            arcos.push_back({u, v});
            custos[{u, v}] = custo_travessia;
            adj[u].push_back({v, custo_travessia});
        }

        arquivo.close();

        // Print de verificação
        cout << "Dados carregados com sucesso de " << nome_arquivo << endl;
        cout << "Instância: " << nome << endl;
        cout << "Depósito: " << deposito << endl;
        cout << "Nós: " << num_vertices << ", Arestas: " << arestas.size() << ", Arcos: " << arcos.size() << endl;
        cout << "Requeridos - Nós: " << vertices_requeridos.size()
             << ", Arestas: " << arestas_requeridas.size()
             << ", Arcos: " << arcos_requeridos.size() << endl;

        // Print matriz de custos
        cout << "\nMatriz de custos (u -> v = custo):\n";
        for (const auto &entry : custos)
        {
            int u = entry.first.first;
            int v = entry.first.second;
            double custo = entry.second;
            cout << u << " -> " << v << " = " << custo << endl;
        }

        // Print lista de adjacência
        cout << "\nLista de adjacência:\n";
        for (const auto &[u, vizinhos] : adj)
        {
            cout << u << " -> ";
            for (const auto &[v, custo] : vizinhos)
            {
                cout << "(" << v << ", " << custo << ") ";
            }
            cout << endl;
        }
    }
};

int main()
{
    Grafo grafo;
    grafo.lerArquivoDados("../../data/BHW1.dat");

    return 0;
}