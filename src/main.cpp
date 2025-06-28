#include "core/grafo.h"
#include "etapas/etapa1.h"
#include "etapas/etapa2.h"
#include "etapas/etapa3.h"
#include <iostream>
#include <string>

using namespace std;

void mostrarMenu()
{
    cout << "\n=======================================" << endl;
    cout << "    SISTEMA DE ANÁLISE DE GRAFOS" << endl;
    cout << "=======================================" << endl;
    cout << "1. Etapa 1 - Análise de Métricas dos Grafos" << endl;
    cout << "2. Etapa 2 - Geração de Soluções Iniciais" << endl;
    cout << "3. Etapa 3 - Otimização com 2-opt" << endl;
    cout << "9. Informações sobre as Etapas" << endl;
    cout << "0. Sair" << endl;
    cout << "=======================================" << endl;
    cout << "Escolha uma opção: ";
}

void mostrarInformacoes()
{
    cout << "\n=== INFORMAÇÕES DAS ETAPAS ===" << endl;
    cout << "\nETAPA 1 - Análise de Métricas:" << endl;
    cout << "- Calcula densidade, graus, caminho médio, diâmetro" << endl;
    cout << "- Calcula centralidade de intermediação" << endl;
    cout << "- Gera arquivos CSV com resultados" << endl;
    cout << "- Processa todos os arquivos .dat do diretório" << endl;
    
    cout << "\nETAPA 2 - Geração de Soluções:" << endl;
    cout << "- Gera soluções iniciais para problemas de roteamento" << endl;
    cout << "- Usa algoritmo guloso nearest neighbor" << endl;
    cout << "- Respeita capacidade dos veículos" << endl;
    cout << "- Salva soluções em formato específico" << endl;
    
    cout << "\nETAPA 3 - Otimização com 2-opt:" << endl;
    cout << "- Aprimora as soluções da Etapa 2" << endl;
    cout << "- Usa algoritmo de busca local 2-opt" << endl;
    cout << "- Melhora o custo das rotas individualmente" << endl;
    cout << "- Mantém todas as restrições de capacidade" << endl;
    cout << "- Gera estatísticas de melhoria" << endl;
    cout << "\nPressione Enter para continuar...";
    cin.ignore();
    cin.get();
}

int main()
{
    cout << "Bem-vindo ao Sistema de Análise de Grafos - GCC262" << endl;
    
    int opcao;
    bool continuar = true;
    
    while (continuar)
    {
        mostrarMenu();
        cin >> opcao;
        
        switch (opcao)
        {
            case 1:
                cout << "\n=== EXECUTANDO ETAPA 1 ===" << endl;
                try 
                {
                    executarEtapa1();
                }
                catch (const exception& e)
                {
                    cerr << "Erro na Etapa 1: " << e.what() << endl;
                }
                break;
                
            case 2:
                cout << "\n=== EXECUTANDO ETAPA 2 ===" << endl;
                try 
                {
                    executarEtapa2();
                }
                catch (const exception& e)
                {
                    cerr << "Erro na Etapa 2: " << e.what() << endl;
                }
                break;
                
            case 3:
                cout << "\n=== EXECUTANDO ETAPA 3 ===" << endl;
                try 
                {
                    executarEtapa3();
                }
                catch (const exception& e)
                {
                    cerr << "Erro na Etapa 3: " << e.what() << endl;
                }
                break;

            case 9:
                mostrarInformacoes();
                break;
                
            case 0:
                cout << "Encerrando o programa..." << endl;
                continuar = false;
                break;
                
            default:
                cout << "Opção inválida! Digite 9 para mais informações." << endl;
                break;
        }
        
        if (continuar && opcao != 9)
        {
            cout << "\nPressione Enter para continuar...";
            cin.ignore();
            cin.get();
        }
    }
    
    return 0;
}