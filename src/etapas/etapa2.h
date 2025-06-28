#ifndef ETAPA2_H
#define ETAPA2_H

#include "../core/grafo.h"
#include <vector>
#include <string>
#include <ctime>

using namespace std;

// Estrutura para representar uma rota
struct Rota
{
    vector<pair<int, int>> nos; // Lista de nós (id_servico, id_no)
    int demanda_total;
    int custo_total;

    Rota();
};

// Estrutura para representar a solução
struct Solucao
{
    vector<Rota> rotas;
    int custo_total;
    int num_rotas;
    clock_t tempo_execucao;

    Solucao();
};

// Função principal da Etapa 2
void executarEtapa2();

// Funções auxiliares
Solucao solucaoInicial(const Grafo &grafo, clock_t &inicio_execucao);
void salvarSolucao(const string &nome_arquivo, const Solucao &solucao, const string &diretorio_saida, const Grafo &grafo);

#endif // ETAPA2_H