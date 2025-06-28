#ifndef ESTRUTURAS_GRAFO_H
#define ESTRUTURAS_GRAFO_H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>

using namespace std;

// Estrutura para representar um serviço
struct Servico
{
    int id;
    char tipo; // 'N', 'E', ou 'A'
    int origem;
    int destino;
    int demanda;
    int custo_servico;
    int custo_transporte;
};

// Estrutura para dados do grafo lidos do arquivo
struct DadosGrafo
{
    // Estrutura básica do grafo
    int num_vertices;
    set<int> vertices;
    vector<pair<int, int>> arestas;
    vector<pair<int, int>> arcos;
    map<int, vector<pair<int, double>>> adj;
    
    // Elementos requeridos
    set<int> vertices_requeridos;
    set<pair<int, int>> arestas_requeridas;
    set<pair<int, int>> arcos_requeridos;
    
    // Metadados da instância
    string nome;
    int valor_otimo;
    int num_veiculos;
    int capacidade;
    int deposito;
    
    // Serviços
    vector<Servico> servicos;
    
    // Construtor
    DadosGrafo() : num_vertices(0), valor_otimo(-1), num_veiculos(-1), capacidade(0), deposito(0) {}
};

#endif // ESTRUTURAS_GRAFO_H