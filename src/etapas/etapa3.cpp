#include "etapa3.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <limits>
#include <iomanip>

namespace fs = std::filesystem;

EstatisticasOtimizacao::EstatisticasOtimizacao()
{
    iteracoes_2opt = 0;
    melhorias_encontradas = 0;
    custo_inicial = 0;
    custo_final = 0;
    melhoria_total = 0;
    percentual_melhoria = 0.0;
    tempo_otimizacao = 0;
}

SolucaoOtimizada::SolucaoOtimizada()
{
    // Construtores das estruturas base já são chamados automaticamente
}

void executarEtapa3()
{
    cout << "Iniciando Etapa 3 - Otimização com 2-opt" << endl;

    // INÍCIO DO CLOCK PARA A ETAPA 3
    clock_t inicio_etapa3 = clock();

    string diretorio_dados = "./dados/";
    string diretorio_saida_etapa2 = "./solucao/";
    string diretorio_saida_etapa3 = "./solucao_otimizada/";

    // Criar o diretório de saída se não existir
    if (!fs::exists(diretorio_saida_etapa3))
    {
        fs::create_directories(diretorio_saida_etapa3);
        cout << "Diretório de saída criado: " << diretorio_saida_etapa3 << endl;
    }

    if (!fs::exists(diretorio_dados))
    {
        cerr << "Erro: Diretório de dados não encontrado: " << diretorio_dados << endl;
        return;
    }

    cout << "Processando instâncias em: " << diretorio_dados << endl;
    cout << "Salvando soluções otimizadas em: " << diretorio_saida_etapa3 << endl;

    int total_instancias = 0;
    int instancias_processadas = 0;
    int instancias_sucesso = 0;
    vector<pair<string, EstatisticasOtimizacao>> todas_estatisticas;

    // Contar o número total de instâncias para processar
    for (const auto &entrada : fs::directory_iterator(diretorio_dados))
    {
        if (entrada.is_regular_file() && entrada.path().extension() == ".dat")
        {
            if (entrada.path().filename().string().substr(0, 4) != "sol-")
            {
                total_instancias++;
            }
        }
    }

    if (total_instancias == 0)
    {
        cerr << "Nenhum arquivo .dat encontrado no diretório: " << diretorio_dados << endl;
        return;
    }

    cout << "Total de instâncias a processar: " << total_instancias << endl;

    // Processar cada arquivo .dat no diretório
    for (const auto &entrada : fs::directory_iterator(diretorio_dados))
    {
        if (entrada.is_regular_file() && entrada.path().extension() == ".dat")
        {
            string caminho_arquivo = entrada.path().string();
            string nome_arquivo = entrada.path().filename().string();

            // Pular arquivos de solução
            if (nome_arquivo.substr(0, 4) == "sol-")
            {
                continue;
            }

            instancias_processadas++;
            cout << "\n===================================================" << endl;
            cout << "Processando instância " << instancias_processadas << "/" << total_instancias
                 << ": " << nome_arquivo << endl;
            cout << "===================================================" << endl;

            try
            {
                // Ler o grafo
                Grafo grafo;
                grafo.lerArquivoDados(caminho_arquivo);

                // Gerar solução inicial (Etapa 2)
                cout << "Gerando solução inicial..." << endl;
                Solucao solucao_inicial = solucaoInicial(grafo, inicio_etapa3);

                // Otimizar solução com 2-opt (Etapa 3)
                cout << "Aplicando otimização 2-opt..." << endl;
                SolucaoOtimizada solucao_otimizada = otimizarSolucao(grafo, solucao_inicial);

                // Salvar solução otimizada
                salvarSolucaoOtimizada(nome_arquivo, solucao_otimizada, diretorio_saida_etapa3, grafo);

                // Armazenar estatísticas
                todas_estatisticas.push_back({nome_arquivo, solucao_otimizada.estatisticas});

                // Mostrar resumo
                cout << "\n✓ Resumo da otimização para " << nome_arquivo << ":" << endl;
                cout << "Custo inicial: " << solucao_otimizada.estatisticas.custo_inicial << endl;
                cout << "Custo otimizado: " << solucao_otimizada.estatisticas.custo_final << endl;
                cout << "Melhoria: " << solucao_otimizada.estatisticas.melhoria_total
                     << " (" << fixed << setprecision(2)
                     << solucao_otimizada.estatisticas.percentual_melhoria << "%)" << endl;
                cout << "Iterações 2-opt: " << solucao_otimizada.estatisticas.iteracoes_2opt << endl;
                cout << "Melhorias encontradas: " << solucao_otimizada.estatisticas.melhorias_encontradas << endl;

                instancias_sucesso++;
            }
            catch (const exception &e)
            {
                cerr << "✗ Erro ao processar instância " << nome_arquivo << ": " << e.what() << endl;
            }

            cout << "---------------------------------------------------" << endl;
            cout << "Progresso: " << instancias_processadas << "/" << total_instancias
                 << " (" << (instancias_processadas * 100 / total_instancias) << "%)" << endl;
        }
    }

    // Exportar estatísticas consolidadas
    if (!todas_estatisticas.empty())
    {
        exportarEstatisticasOtimizacao(todas_estatisticas);
    }

    // Calcular tempo total da etapa 3
    clock_t tempo_total_etapa3 = clock() - inicio_etapa3;

    cout << "\n=== ETAPA 3 CONCLUÍDA ===" << endl;
    cout << "Total processado: " << instancias_sucesso << "/" << total_instancias << endl;
    cout << "Tempo total da Etapa 3: " << tempo_total_etapa3 << " clocks ("
         << static_cast<double>(tempo_total_etapa3) / CLOCKS_PER_SEC << " segundos)" << endl;
    cout << "Soluções otimizadas salvas em: " << diretorio_saida_etapa3 << endl;
    cout << "Estatísticas exportadas para: resultados_otimizacao_2opt.csv" << endl;
}

SolucaoOtimizada otimizarSolucao(const Grafo &grafo, const Solucao &solucao_inicial)
{
    SolucaoOtimizada resultado;
    resultado.solucao_inicial = solucao_inicial;
    resultado.solucao_otimizada = solucao_inicial; // Começar com a solução inicial

    clock_t inicio_otimizacao = clock();

    resultado.estatisticas.custo_inicial = solucao_inicial.custo_total;
    resultado.estatisticas.custo_final = solucao_inicial.custo_total;

    cout << "Iniciando otimização 2-opt para " << solucao_inicial.rotas.size() << " rotas..." << endl;

    // Aplicar 2-opt em cada rota individualmente
    for (size_t i = 0; i < resultado.solucao_otimizada.rotas.size(); i++)
    {
        cout << "Otimizando rota " << (i + 1) << "/" << resultado.solucao_otimizada.rotas.size() << "..." << endl;

        Rota rota_original = resultado.solucao_otimizada.rotas[i];
        Rota rota_otimizada = aplicar2Opt(grafo, rota_original, resultado.estatisticas);

        // Atualizar a rota se houve melhoria
        if (rota_otimizada.custo_total < rota_original.custo_total)
        {
            resultado.solucao_otimizada.rotas[i] = rota_otimizada;
            cout << "  Rota " << (i + 1) << " melhorada: "
                 << rota_original.custo_total << " -> " << rota_otimizada.custo_total
                 << " (melhoria: " << (rota_original.custo_total - rota_otimizada.custo_total) << ")" << endl;
        }
        else
        {
            cout << "  Rota " << (i + 1) << " não foi melhorada" << endl;
        }
    }

    // Recalcular custo total da solução
    resultado.solucao_otimizada.custo_total = 0;
    for (const auto &rota : resultado.solucao_otimizada.rotas)
    {
        resultado.solucao_otimizada.custo_total += rota.custo_total;
    }

    // Finalizar estatísticas
    resultado.estatisticas.custo_final = resultado.solucao_otimizada.custo_total;
    resultado.estatisticas.melhoria_total = resultado.estatisticas.custo_inicial - resultado.estatisticas.custo_final;

    if (resultado.estatisticas.custo_inicial > 0)
    {
        resultado.estatisticas.percentual_melhoria =
            (static_cast<double>(resultado.estatisticas.melhoria_total) / resultado.estatisticas.custo_inicial) * 100.0;
    }

    resultado.estatisticas.tempo_otimizacao = clock() - inicio_otimizacao;

    cout << "Otimização concluída!" << endl;
    cout << "Melhoria total: " << resultado.estatisticas.melhoria_total
         << " (" << fixed << setprecision(2) << resultado.estatisticas.percentual_melhoria << "%)" << endl;

    return resultado;
}

Rota aplicar2Opt(const Grafo &grafo, const Rota &rota_original, EstatisticasOtimizacao &stats)
{
    Rota melhor_rota = rota_original;
    bool melhorou = true;
    int iteracoes_locais = 0;

    // Só aplicar 2-opt se a rota tiver pelo menos 4 serviços (para poder fazer trocas)
    if (rota_original.nos.size() < 4)
    {
        return melhor_rota;
    }

    while (melhorou && iteracoes_locais < 1000) // Limitar iterações para evitar loops infinitos
    {
        melhorou = false;
        iteracoes_locais++;
        stats.iteracoes_2opt++;

        // Tentar todas as combinações de 2-opt
        for (size_t i = 0; i < melhor_rota.nos.size() - 1; i++)
        {
            for (size_t j = i + 2; j < melhor_rota.nos.size(); j++)
            {
                // Criar nova rota com movimento 2-opt
                Rota nova_rota = melhor_rota;

                // Reverter o segmento entre i+1 e j
                reverse(nova_rota.nos.begin() + i + 1, nova_rota.nos.begin() + j + 1);

                // Recalcular custo da nova rota
                double novo_custo = calcularCustoRota(grafo, nova_rota);
                nova_rota.custo_total = static_cast<int>(novo_custo);

                // Verificar se a nova rota é válida e melhor
                if (verificarValidadeRota(grafo, nova_rota) && nova_rota.custo_total < melhor_rota.custo_total)
                {
                    melhor_rota = nova_rota;
                    melhorou = true;
                    stats.melhorias_encontradas++;
                    break;
                }
            }
            if (melhorou)
                break; // Reiniciar busca com a nova rota
        }
    }

    return melhor_rota;
}

double calcularCustoRota(const Grafo &grafo, const Rota &rota)
{
    if (rota.nos.empty())
    {
        return 0.0;
    }

    double custo_total = 0.0;
    int deposito = grafo.getDeposito();
    int no_atual = deposito;

    // Calcular custo da rota completa
    for (const auto &[id_servico, no_entrada] : rota.nos)
    {
        // Custo para chegar ao serviço
        custo_total += grafo.getDistancia(no_atual, no_entrada);

        // Custo do serviço
        const Servico &servico = grafo.getServicos()[id_servico - 1];
        custo_total += servico.custo_servico;

        // Determinar nó de saída após executar o serviço
        if (servico.tipo == 'N')
        {
            no_atual = servico.origem;
        }
        else if (servico.tipo == 'A')
        {
            no_atual = servico.destino;
        }
        else // 'E'
        {
            no_atual = (no_entrada == servico.origem) ? servico.destino : servico.origem;
        }
    }

    // Custo de retorno ao depósito
    custo_total += grafo.getDistancia(no_atual, deposito);

    return custo_total;
}

bool verificarValidadeRota(const Grafo &grafo, const Rota &rota)
{
    // Verificar se a demanda não excede a capacidade
    int demanda_total = 0;
    for (const auto &[id_servico, no_entrada] : rota.nos)
    {
        const Servico &servico = grafo.getServicos()[id_servico - 1];
        demanda_total += servico.demanda;
    }

    return demanda_total <= grafo.getCapacidade();
}

void salvarSolucaoOtimizada(const string &nome_arquivo, const SolucaoOtimizada &solucao_otim,
                            const string &diretorio_saida, const Grafo &grafo)
{
    string nome_solucao = "sol-" + nome_arquivo;
    string caminho_solucao = diretorio_saida + nome_solucao;

    ofstream arquivo(caminho_solucao);

    if (!arquivo.is_open())
    {
        throw runtime_error("Erro ao criar o arquivo de solução otimizada: " + caminho_solucao);
    }

    const Solucao &solucao = solucao_otim.solucao_otimizada;

    int deposito = grafo.getDeposito();

    // Escrever cabeçalho da solução
    arquivo << solucao.custo_total << endl;
    arquivo << solucao.num_rotas << endl;
    arquivo << solucao.tempo_execucao + solucao_otim.estatisticas.tempo_otimizacao << endl; // Tempo total
    arquivo << clock() << endl;

    // Escrever cada rota
    for (size_t i = 0; i < solucao.rotas.size(); i++)
    {
        const Rota &rota = solucao.rotas[i];

        // Formato correto: índice_do_depósito(sempre 0) dia(sempre 1) id_rota demanda custo total_visitas
        arquivo << " 0 1 " << (i + 1) << " " << rota.demanda_total << " " << rota.custo_total << " " << (rota.nos.size() + 1) << " ";

        // Primeiro nó é sempre o depósito
        arquivo << "(D,0," << deposito << "," << deposito << ") ";

        // Escrever cada serviço na rota
        for (const auto &no : rota.nos)
        {
            int id_servico = no.first;
            int id_no = no.second;

            if (id_servico == 0)
            {
                // Se é o depósito
                arquivo << "(D,0," << deposito << "," << deposito << ") ";
            }
            else
            {
                // Se é um serviço
                const Servico &servico = grafo.getServicos()[id_servico - 1];

                if (servico.tipo == 'N')
                {
                    // Nó requerido
                    arquivo << "(S " << id_servico << "," << id_no << "," << id_no << ") ";
                }
                else
                {
                    // Aresta ou arco requerido
                    int origem = servico.origem;
                    int destino = servico.destino;

                    if (id_no == origem)
                    {
                        arquivo << "(S " << id_servico << "," << origem << "," << destino << ") ";
                    }
                    else
                    {
                        arquivo << "(S " << id_servico << "," << destino << "," << origem << ") ";
                    }
                }
            }
        }

        // Último nó é sempre o depósito
        arquivo << "(D,0," << deposito << "," << deposito << ")" << endl;
    }

    arquivo.close();

    cout << "Solução otimizada salva em: " << caminho_solucao << endl;
}

void exportarEstatisticasOtimizacao(const vector<pair<string, EstatisticasOtimizacao>> &todas_estatisticas)
{
    ofstream arquivo("resultados_otimizacao_2opt.csv");

    if (!arquivo.is_open())
    {
        throw runtime_error("Não foi possível criar o arquivo: resultados_otimizacao_2opt.csv");
    }

    // Cabeçalho
    arquivo << "Instancia,CustoInicial,CustoFinal,MelhoriaAbsoluta,MelhoriaPercentual,Iteracoes2opt,MelhoriasEncontradas,TempoOtimizacao\n";

    // Dados de cada instância
    for (const auto &[nome, stats] : todas_estatisticas)
    {
        arquivo << nome << ","
                << stats.custo_inicial << ","
                << stats.custo_final << ","
                << stats.melhoria_total << ","
                << fixed << setprecision(2) << stats.percentual_melhoria << ","
                << stats.iteracoes_2opt << ","
                << stats.melhorias_encontradas << ","
                << stats.tempo_otimizacao << "\n";
    }

    arquivo.close();
    cout << "Estatísticas de otimização exportadas para: resultados_otimizacao_2opt.csv" << endl;
}