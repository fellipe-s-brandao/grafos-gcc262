#ifndef ETAPA3_H
#define ETAPA3_H

#include "../core/grafo.h"
#include "etapa2.h" // Para reutilizar estruturas Rota e Solucao
#include <vector>
#include <string>
#include <ctime>

using namespace std;

// Estrutura para armazenar estatísticas da otimização
struct EstatisticasOtimizacao
{
    int iteracoes_2opt;
    int melhorias_encontradas;
    int custo_inicial;
    int custo_final;
    int melhoria_total;
    double percentual_melhoria;
    clock_t tempo_otimizacao;

    EstatisticasOtimizacao();
};

// Estrutura para solução otimizada (herda de Solucao)
struct SolucaoOtimizada
{
    Solucao solucao_inicial;
    Solucao solucao_otimizada;
    EstatisticasOtimizacao estatisticas;

    SolucaoOtimizada();
};

// Função principal da Etapa 3
void executarEtapa3();

// Funções de otimização 2-opt
SolucaoOtimizada otimizarSolucao(const Grafo &grafo, const Solucao &solucao_inicial);
Rota aplicar2Opt(const Grafo &grafo, const Rota &rota_original, EstatisticasOtimizacao &stats);
double calcularCustoRota(const Grafo &grafo, const Rota &rota);
bool verificarValidadeRota(const Grafo &grafo, const Rota &rota);

// Função para salvar solução otimizada
void salvarSolucaoOtimizada(const string &nome_arquivo, const SolucaoOtimizada &solucao_otim,
                            const string &diretorio_saida, const Grafo &grafo);

// Função para exportar estatísticas
void exportarEstatisticasOtimizacao(const vector<pair<string, EstatisticasOtimizacao>> &todas_estatisticas);

#endif // ETAPA3_H