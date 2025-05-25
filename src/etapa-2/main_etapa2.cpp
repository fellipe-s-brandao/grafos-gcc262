#include "../grafo.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>

namespace fs = std::filesystem;
using namespace std;

// Estrutura para representar uma rota
struct Rota
{
    vector<pair<int, int>> nos; // Lista de nós (id_servico, id_no)
    int demanda_total;
    int custo_total;

    Rota()
    {
        demanda_total = 0;
        custo_total = 0;
    }
};

// Estrutura para representar a solução
struct Solucao
{
    vector<Rota> rotas;
    int custo_total;
    int num_rotas;
    clock_t tempo_execucao;

    Solucao()
    {
        custo_total = 0;
        num_rotas = 0;
        tempo_execucao = 0;
    }
};

Solucao solucaoInicial(const Grafo &grafo, clock_t &inicio_execucao)
{
    Solucao solucao;
    clock_t inicio = inicio_execucao;

    cout << "Iniciando construção da solução inicial..." << endl;

    int deposito = grafo.getDeposito();
    const vector<Servico> &servicos = grafo.getServicos();

    cout << "Total de serviços: " << servicos.size() << endl;
    cout << "Capacidade do veículo: " << grafo.getCapacidade() << endl;

    // Lista de todos os serviços não atribuídos
    vector<int> servicos_nao_atribuidos;
    for (int i = 0; i < servicos.size(); i++)
    {
        servicos_nao_atribuidos.push_back(i + 1);
    }

    vector<Rota> rotas;
    int contador_rotas = 0;

    // Criar rotas até que todos os serviços sejam atendidos
    while (!servicos_nao_atribuidos.empty())
    {
        contador_rotas++;
        cout << "Criando rota " << contador_rotas << " (serviços restantes: "
             << servicos_nao_atribuidos.size() << ")" << endl;

        // Evitar loop infinito
        if (contador_rotas > 50)
        {
            cout << "AVISO: Muitas rotas criadas. Parando para evitar loop infinito." << endl;
            break;
        }

        // Encontrar o serviço mais próximo do depósito
        int melhor_servico = -1;
        int melhor_no = -1;
        double menor_custo = numeric_limits<double>::infinity();

        for (int id_servico : servicos_nao_atribuidos)
        {
            const Servico &servico = servicos[id_servico - 1];

            double custo_para_origem = grafo.getDistancia(deposito, servico.origem);
            double custo_para_destino = grafo.getDistancia(deposito, servico.destino);

            double custo;
            int no_entrada;

            if (servico.tipo == 'N')
            {
                custo = custo_para_origem;
                no_entrada = servico.origem;
            }
            else if (servico.tipo == 'A')
            {
                custo = custo_para_origem;
                no_entrada = servico.origem;
            }
            else // 'E'
            {
                if (custo_para_origem <= custo_para_destino)
                {
                    custo = custo_para_origem;
                    no_entrada = servico.origem;
                }
                else
                {
                    custo = custo_para_destino;
                    no_entrada = servico.destino;
                }
            }

            if (custo < menor_custo)
            {
                menor_custo = custo;
                melhor_servico = id_servico;
                melhor_no = no_entrada;
            }
        }

        if (melhor_servico == -1)
        {
            cout << "ERRO: Não foi possível encontrar um serviço válido!" << endl;
            break;
        }

        // Criar nova rota começando com o melhor serviço
        Rota rota;
        const Servico &servico = servicos[melhor_servico - 1];

        rota.nos.push_back({melhor_servico, melhor_no});
        rota.demanda_total = servico.demanda;
        rota.custo_total = grafo.getDistancia(deposito, melhor_no) + servico.custo_servico;

        // Determinar nó atual após executar o serviço
        int no_atual;
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
            no_atual = (melhor_no == servico.origem) ? servico.destino : servico.origem;
        }

        // Remover serviço da lista
        auto it = find(servicos_nao_atribuidos.begin(), servicos_nao_atribuidos.end(), melhor_servico);
        if (it != servicos_nao_atribuidos.end())
        {
            servicos_nao_atribuidos.erase(it);
        }

        // Continuar adicionando serviços à rota atual
        bool adicionou_servico = true;
        int tentativas = 0;

        while (adicionou_servico && !servicos_nao_atribuidos.empty() &&
               rota.demanda_total < grafo.getCapacidade())
        {
            tentativas++;
            if (tentativas > 200)
            { // Evitar loop infinito interno
                cout << "AVISO: Muitas tentativas na rota atual. Finalizando rota." << endl;
                break;
            }

            adicionou_servico = false;
            melhor_servico = -1;
            melhor_no = -1;
            menor_custo = numeric_limits<double>::infinity();

            for (int id_servico : servicos_nao_atribuidos)
            {
                const Servico &servico = servicos[id_servico - 1];

                // Verificar capacidade
                if (rota.demanda_total + servico.demanda > grafo.getCapacidade())
                    continue;

                double custo_para_origem = grafo.getDistancia(no_atual, servico.origem);
                double custo_para_destino = grafo.getDistancia(no_atual, servico.destino);

                double custo;
                int no_entrada;

                if (servico.tipo == 'N')
                {
                    custo = custo_para_origem;
                    no_entrada = servico.origem;
                }
                else if (servico.tipo == 'A')
                {
                    custo = custo_para_origem;
                    no_entrada = servico.origem;
                }
                else // 'E'
                {
                    if (custo_para_origem <= custo_para_destino)
                    {
                        custo = custo_para_origem;
                        no_entrada = servico.origem;
                    }
                    else
                    {
                        custo = custo_para_destino;
                        no_entrada = servico.destino;
                    }
                }

                if (custo < menor_custo)
                {
                    menor_custo = custo;
                    melhor_servico = id_servico;
                    melhor_no = no_entrada;
                }
            }

            if (melhor_servico != -1)
            {
                const Servico &servico = servicos[melhor_servico - 1];

                rota.custo_total += grafo.getDistancia(no_atual, melhor_no);
                rota.nos.push_back({melhor_servico, melhor_no});
                rota.demanda_total += servico.demanda;
                rota.custo_total += servico.custo_servico;

                // Atualizar nó atual
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
                    no_atual = (melhor_no == servico.origem) ? servico.destino : servico.origem;
                }

                // Remover serviço
                auto it = find(servicos_nao_atribuidos.begin(), servicos_nao_atribuidos.end(), melhor_servico);
                if (it != servicos_nao_atribuidos.end())
                {
                    servicos_nao_atribuidos.erase(it);
                }

                adicionou_servico = true;
            }
        }

        // Adicionar custo de retorno ao depósito
        rota.custo_total += grafo.getDistancia(no_atual, deposito);

        rotas.push_back(rota);

        cout << "Rota " << contador_rotas << " criada: " << rota.nos.size()
             << " serviços, demanda " << rota.demanda_total
             << ", custo " << rota.custo_total << endl;
    }

    // Finalizar solução
    solucao.rotas = rotas;
    solucao.num_rotas = rotas.size();
    solucao.custo_total = 0;

    for (const auto &rota : rotas)
    {
        solucao.custo_total += rota.custo_total;
    }

    solucao.tempo_execucao = clock() - inicio;

    cout << "Solução construída com " << solucao.num_rotas << " rotas e custo " << solucao.custo_total << endl;

    return solucao;
}
// Função para salvar a solução em arquivo
void salvarSolucao(const string &nome_arquivo, const Solucao &solucao, const string &diretorio_saida, const Grafo &grafo)
{
    string nome_solucao = "sol-" + nome_arquivo;
    string caminho_solucao = diretorio_saida + nome_solucao;

    ofstream arquivo(caminho_solucao);

    if (!arquivo.is_open())
    {
        cerr << "Erro ao criar o arquivo de solução: " << caminho_solucao << endl;
        return;
    }

    // Escrever cabeçalho da solução
    arquivo << solucao.custo_total << endl;
    arquivo << solucao.num_rotas << endl;
    arquivo << solucao.tempo_execucao << endl;
    arquivo << clock() << endl;

    // Escrever cada rota
    for (int i = 0; i < solucao.rotas.size(); i++)
    {
        const Rota &rota = solucao.rotas[i];

        // Índice do depósito (sempre 0), dia (sempre 1), ID da rota (começando por 1)
        arquivo << " 0 1 " << (i + 1) << " " << rota.demanda_total << " " << rota.custo_total << " " << (rota.nos.size() + 1) << " ";

        // Primeiro nó é sempre o depósito
        arquivo << "(D 0,1,1) ";

        // Escrever cada serviço na rota
        for (const auto &no : rota.nos)
        {
            int id_servico = no.first;
            int id_no = no.second;

            if (id_servico == 0)
            {
                // Se é o depósito
                arquivo << "(D 0,1,1) ";
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
        arquivo << "(D 0,1,1)" << endl;
    }

    arquivo.close();

    cout << "Solução salva em: " << caminho_solucao << endl;
}

int main()
{
    string diretorio_dados = "./dados/";
    string diretorio_saida = "./solucao/";
    clock_t inicio = clock();

    // Criar o diretório de saída se não existir
    if (!fs::exists(diretorio_saida))
    {
        fs::create_directories(diretorio_saida);
        cout << "Diretório de saída criado: " << diretorio_saida << endl;
    }

    cout << "Iniciando processamento de instâncias- Etapa 2" << endl;
    cout << "Buscando arquivos .dat em: " << diretorio_dados << endl;
    cout << "Salvando soluções em: " << diretorio_saida << endl;

    int total_instancias = 0;
    int instancias_processadas = 0;

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

                // Gerar solução inicial
                Solucao solucao = solucaoInicial(grafo, inicio);

                // Salvar solução
                salvarSolucao(nome_arquivo, solucao, diretorio_saida, grafo);

                cout << "\nResumo da solução para " << nome_arquivo << ":" << endl;
                cout << "Custo total: " << solucao.custo_total << endl;
                cout << "Número de rotas: " << solucao.num_rotas << endl;
                cout << "Tempo de execução: " << solucao.tempo_execucao << " clocks" << endl;
            }
            catch (const exception &e)
            {
                cerr << "Erro ao processar instância " << nome_arquivo << ": " << e.what() << endl;
            }

            cout << "---------------------------------------------------" << endl;
            cout << "Progresso: " << instancias_processadas << "/" << total_instancias
                 << " (" << (instancias_processadas * 100 / total_instancias) << "%)" << endl;
        }
    }

    cout << "\nTodas as instâncias foram processadas!" << endl;
    cout << "Total processado: " << instancias_processadas << "/" << total_instancias << endl;
    cout << "Soluções salvas em: " << diretorio_saida << endl;

    return 0;
}