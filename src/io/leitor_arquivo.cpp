#include "leitor_arquivo.h"
#include <iostream>

DadosGrafo LeitorArquivo::lerArquivoDados(const string &nome_arquivo)
{
    DadosGrafo dados;
    
    cout << "Lendo arquivo: " << nome_arquivo << endl;
    
    ifstream arquivo(nome_arquivo);
    if (!arquivo.is_open())
    {
        cerr << "Erro ao abrir o arquivo: " << nome_arquivo << endl;
        return dados;
    }

    int id_servico = 1;
    int num_vertices_requeridos, num_arestas_requeridas, num_arcos_requeridos;
    
    lerCabecalho(arquivo, dados);
    lerQuantidadesRequeridas(arquivo, num_vertices_requeridos, num_arestas_requeridas, num_arcos_requeridos);
    inicializarVertices(dados);
    
    lerNosRequeridos(arquivo, dados, num_vertices_requeridos, id_servico);
    lerArestasRequeridas(arquivo, dados, num_arestas_requeridas, id_servico);
    lerArestasNaoRequeridas(arquivo, dados);
    lerArcosRequeridos(arquivo, dados, num_arcos_requeridos, id_servico);
    lerArcosNaoRequeridos(arquivo, dados);
    
    arquivo.close();
    
    cout << "Leitura do arquivo concluída." << endl;
    cout << "Total de serviços: " << dados.servicos.size() << endl;
    
    return dados;
}

void LeitorArquivo::lerCabecalho(ifstream &arquivo, DadosGrafo &dados)
{
    string linha;
    
    // Nome da instância
    getline(arquivo, linha);
    dados.nome = linha.substr(linha.find_first_of(":") + 1);
    dados.nome = dados.nome.substr(dados.nome.find_first_not_of(" \t"));
    cout << "Nome da instância: " << dados.nome << endl;

    // Valor ótimo
    getline(arquivo, linha);
    dados.valor_otimo = stoi(linha.substr(linha.find_first_of(":") + 1));

    // Número de veículos
    getline(arquivo, linha);
    dados.num_veiculos = stoi(linha.substr(linha.find_first_of(":") + 1));

    // Capacidade
    getline(arquivo, linha);
    dados.capacidade = stoi(linha.substr(linha.find_first_of(":") + 1));
    cout << "Capacidade: " << dados.capacidade << endl;

    // Depósito
    getline(arquivo, linha);
    dados.deposito = stoi(linha.substr(linha.find_first_of(":") + 1));
    cout << "Depósito: " << dados.deposito << endl;

    // Número de nós
    getline(arquivo, linha);
    dados.num_vertices = stoi(linha.substr(linha.find_first_of(":") + 1));
    cout << "Número de vértices: " << dados.num_vertices << endl;

    // Pular número de arestas e arcos (não usados diretamente)
    getline(arquivo, linha); // num_arestas
    getline(arquivo, linha); // num_arcos
}

void LeitorArquivo::lerQuantidadesRequeridas(ifstream &arquivo, int &num_vertices_req, 
                                            int &num_arestas_req, int &num_arcos_req)
{
    string linha;
    
    getline(arquivo, linha);
    num_vertices_req = stoi(linha.substr(linha.find_first_of(":") + 1));
    
    getline(arquivo, linha);
    num_arestas_req = stoi(linha.substr(linha.find_first_of(":") + 1));
    
    getline(arquivo, linha);
    num_arcos_req = stoi(linha.substr(linha.find_first_of(":") + 1));
    
    cout << "Requeridos - Nós: " << num_vertices_req 
         << ", Arestas: " << num_arestas_req 
         << ", Arcos: " << num_arcos_req << endl;
}

void LeitorArquivo::inicializarVertices(DadosGrafo &dados)
{
    for (int i = 1; i <= dados.num_vertices; i++)
    {
        dados.vertices.insert(i);
    }
}

void LeitorArquivo::lerNosRequeridos(ifstream &arquivo, DadosGrafo &dados, 
                                    int num_vertices_requeridos, int &id_servico)
{
    pularAteSecao(arquivo, "ReN.");
    cout << "Lendo nós requeridos..." << endl;
    
    for (int i = 0; i < num_vertices_requeridos; i++)
    {
        string linha;
        getline(arquivo, linha);
        stringstream ss(linha);
        string nome_no;
        int demanda, custo_servico;

        ss >> nome_no >> demanda >> custo_servico;

        int node = stoi(nome_no.substr(1)); // Remove o 'N'
        dados.vertices_requeridos.insert(node);

        // Criar serviço para o nó
        Servico servico;
        servico.id = id_servico++;
        servico.tipo = 'N';
        servico.origem = node;
        servico.destino = node;
        servico.demanda = demanda;
        servico.custo_servico = custo_servico;
        servico.custo_transporte = 0;

        dados.servicos.push_back(servico);
        cout << "Serviço " << servico.id << ": Nó " << node 
             << ", Demanda " << demanda << ", Custo " << custo_servico << endl;
    }
}

void LeitorArquivo::lerArestasRequeridas(ifstream &arquivo, DadosGrafo &dados, 
                                        int num_arestas_requeridas, int &id_servico)
{
    pularAteSecao(arquivo, "ReE.");
    cout << "Lendo arestas requeridas..." << endl;
    
    for (int i = 0; i < num_arestas_requeridas; i++)
    {
        string linha;
        getline(arquivo, linha);
        stringstream ss(linha);
        string edge_name;
        int u, v;
        double custo_travessia, demanda, custo_servico;

        ss >> edge_name >> u >> v >> custo_travessia >> demanda >> custo_servico;

        dados.arestas.push_back({u, v});
        dados.arestas_requeridas.insert({u, v});
        dados.adj[u].push_back({v, custo_travessia});
        dados.adj[v].push_back({u, custo_travessia});

        // Criar serviço para a aresta
        Servico servico;
        servico.id = id_servico++;
        servico.tipo = 'E';
        servico.origem = u;
        servico.destino = v;
        servico.demanda = demanda;
        servico.custo_servico = custo_servico;
        servico.custo_transporte = custo_travessia;

        dados.servicos.push_back(servico);
        cout << "Serviço " << servico.id << ": Aresta (" << u << "," << v 
             << "), Demanda " << demanda << ", Custo " << custo_servico << endl;
    }
}

void LeitorArquivo::lerArestasNaoRequeridas(ifstream &arquivo, DadosGrafo &dados)
{
    pularAteSecao(arquivo, "EDGE");
    cout << "Lendo arestas não requeridas..." << endl;
    
    string linha;
    getline(arquivo, linha); // Pular linha em branco
    
    while (getline(arquivo, linha) && !linha.empty() && linha.find("ReA.") == string::npos)
    {
        if (linha.find_first_not_of(" \t\n\r") == string::npos)
        {
            continue;
        }

        stringstream ss(linha);
        string edge_name;
        int u, v;
        double custo_travessia;

        ss >> edge_name >> u >> v >> custo_travessia;

        dados.arestas.push_back({u, v});
        dados.adj[u].push_back({v, custo_travessia});
        dados.adj[v].push_back({u, custo_travessia});
    }

    // Se encontrou ReA, precisa retroceder uma linha no arquivo
    if (!linha.empty() && linha.find("ReA.") != string::npos)
    {
        arquivo.seekg(-linha.length() - 1, ios_base::cur);
    }
}

void LeitorArquivo::lerArcosRequeridos(ifstream &arquivo, DadosGrafo &dados, 
                                      int num_arcos_requeridos, int &id_servico)
{
    pularAteSecao(arquivo, "ReA.");
    cout << "Lendo arcos requeridos..." << endl;
    
    for (int i = 0; i < num_arcos_requeridos; i++)
    {
        string linha;
        getline(arquivo, linha);
        stringstream ss(linha);
        string arc_name;
        int u, v;
        double custo_travessia, demanda, custo_servico;

        ss >> arc_name >> u >> v >> custo_travessia >> demanda >> custo_servico;

        dados.arcos.push_back({u, v});
        dados.arcos_requeridos.insert({u, v});
        dados.adj[u].push_back({v, custo_travessia});

        // Criar serviço para o arco
        Servico servico;
        servico.id = id_servico++;
        servico.tipo = 'A';
        servico.origem = u;
        servico.destino = v;
        servico.demanda = demanda;
        servico.custo_servico = custo_servico;
        servico.custo_transporte = custo_travessia;

        dados.servicos.push_back(servico);
        cout << "Serviço " << servico.id << ": Arco (" << u << "," << v 
             << "), Demanda " << demanda << ", Custo " << custo_servico << endl;
    }
}

void LeitorArquivo::lerArcosNaoRequeridos(ifstream &arquivo, DadosGrafo &dados)
{
    pularAteSecao(arquivo, "ARC");
    cout << "Lendo arcos não requeridos..." << endl;
    
    string linha;
    getline(arquivo, linha); // Pular linha em branco
    
    while (getline(arquivo, linha) && !linha.empty())
    {
        if (linha.find_first_not_of(" \t\n\r") == string::npos)
        {
            continue;
        }

        stringstream ss(linha);
        string arc_name;
        int u, v;
        double custo_travessia;

        if (!(ss >> arc_name >> u >> v >> custo_travessia))
        {
            continue; // Linha mal formatada, pular
        }

        dados.arcos.push_back({u, v});
        dados.adj[u].push_back({v, custo_travessia});
    }
}

void LeitorArquivo::pularAteSecao(ifstream &arquivo, const string &secao)
{
    string linha;
    while (getline(arquivo, linha) && linha.find(secao) == string::npos)
        ;
}