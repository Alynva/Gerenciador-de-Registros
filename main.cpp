#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "clear_console.cpp"
#include "pause_console_with_message.cpp"
#include "to_string.cpp"

#define TAM_CHAVE 3
#define TAM_NUMERO_DDD 2
#define TAM_NUMERO_PREFIXO 5
#define TAM_NUMERO_SUFIXO 4
#define TAM_DATA_DIA 2
#define TAM_DATA_MES 2
#define TAM_DATA_ANO 4
#define TAM_EMAIL 30
#define TAM_NOME 48

using namespace std;

int main() {
	
	FILE *lf;
	lf = fopen("arquivo.bin", "a+b"); // Abre o arquivo, caso exista, ou cria um vazio
	
	if(lf == NULL) { // Verifica a ocorrência de erro ao abrir o arquivo
		printf("Erro na abertura do arquivo.\n");
		return 2;
	}
	rewind(lf); // Move o ponteiro de leitura/escrita para o começo do arquivo

	string cabecalho; // Variável utilizada para escrever o texto do cabeçalho
	int tam_cabecalho = 50; // Variável para guardar o tamanho em bytes do cabeçalho
	int n_registros = 0;
	fscanf(lf, "Número de registros: %d", &n_registros); // Lê do arquivo o total de registros atual

	int opcao = 0;
	do { // Loop para mostrar o menu
		system(CLEAR);

		printf("Menu:\n" \
				"\t-1 - Sair\n" \
				"\t 0 - Zerar arquivo\n" \
				"\t 1 - Inserir novo registro\n");

		printf("\nDigite o opção: ");
		scanf("%d", &opcao);
		cout << endl;

		switch (opcao) {
			case -1:
				break;
			case 0:

				pause("zerar o arquivo"); // Confirma a ação com o usuário

				lf = freopen("arquivo.bin", "w+b", lf); // Reabre o arquivo em w+ para zerá-lo
				
				if(lf == NULL) { // Verifica erros
					printf("Erro na criaçao do arquivo.\n");
					return 1;
				}

				n_registros = 0; // Zera o contador de registros
				cabecalho = "Número de registros: ";
				cabecalho.append(to_string(n_registros));
				cabecalho.append(tam_cabecalho - strlen(cabecalho.c_str()), ' '); // Preenche a string com espaço até dar o tamanho do cabeçalho
				fprintf(lf, "%s", cabecalho.c_str()); // Imprime o cabeçalho

				fflush(lf); // Força a atualização do arquivo
				break;
			case 1:
				{ // '{}' necessário para criar um escopo e poder declarar variáveis nele
					lf = freopen("arquivo.bin", "a+b", lf); // Reabre o arquivo em a+ para adicionar um registro ao fim (append)
					
					if(lf == NULL) { // Verifica erros
						printf("Erro na abertura do arquivo.\n");
						return 2;
					}

					string chave = "";
					while (strlen(chave.c_str()) != TAM_CHAVE) { // Pede a chave do registro enquanto a inserida não tiver um comprimento de 3 caracteres
						cout << "Insira a chave para o registro (XXX): ";
						cin >> chave;
					}

					string numero_ddd = "", numero_prefixo = "", numero_sufixo = "";
					while (strlen(numero_ddd.c_str()) != TAM_NUMERO_DDD || strlen(numero_prefixo.c_str()) != TAM_NUMERO_PREFIXO || strlen(numero_sufixo.c_str()) != TAM_NUMERO_SUFIXO) { // Pede o número (DDD, prefixo e sufixo) enquanto não tiverem o comprimento 2, 5 e 4, respectivamente, de caracteres
						cout << "Insira o número de telefone (XX XXXXX XXXX): ";
						cin >> numero_ddd >> numero_prefixo >> numero_sufixo;
					}

					string data_dia = "", data_mes = "", data_ano = "";
					while (strlen(data_dia.c_str()) != TAM_DATA_DIA || strlen(data_mes.c_str()) != TAM_DATA_MES || strlen(data_ano.c_str()) != TAM_DATA_ANO) { // Pede a dara (dia, mês e ano) enquanto nao tiverem o comprimento 2, 2, e 4, respectivamente, de caracteres
						cout << "Insira a data de nascimento (XX XX XXXX): ";
						cin >> data_dia >> data_mes >> data_ano;
					}

					string email = "";
					do {
						cout << "Insira o e-mail: ";
						cin >> email;
					} while (strlen(email.c_str()) > TAM_EMAIL); // Pede o email novamente caso o comprimento exceda 30 caracteres
					email.append(TAM_EMAIL - strlen(email.c_str()), ' '); // Preenche o restante com espaços

					string nome = "";
					do {
						cout << "Insira o nome: ";
						getline(cin >> ws, nome);
					} while (strlen(nome.c_str()) > TAM_NOME); /// Pede o nome novamente caso o comprimento exceda 48 caracteres
					nome.append(TAM_NOME - strlen(nome.c_str()), ' '); // Preenche o restante com espaços

					cout << "\nDados reconhecidos:" << endl;
					cout << "Chave: " << chave << endl;
					cout << "Telefone: (" << numero_ddd << ") " << numero_prefixo << "-" << numero_sufixo << endl;
					cout << "Data de nascimento: " << data_dia << "/" << data_mes << "/" << data_ano << endl;
					cout << "E-mail: " << email << endl;
					cout << "Nome: " << nome << endl << endl;

					pause("gravar o registro"); // Confirma a ação com o usuário

					// Grava os dados
					fprintf(lf, "%s", chave.c_str());
					fprintf(lf, "%s%s%s", numero_ddd.c_str(), numero_prefixo.c_str(), numero_sufixo.c_str());
					fprintf(lf, "%s%s%s", data_dia.c_str(), data_mes.c_str(), data_ano.c_str());
					fprintf(lf, "%s", email.c_str());
					fprintf(lf, "%s", nome.c_str());

					// Reabre o arquivo e atualiza o cabeçalho
					lf = freopen("arquivo.bin", "r+b", lf);
					rewind(lf);
					cabecalho = "Número de registros: ";
					cabecalho.append(to_string(++n_registros));
					cabecalho.append(50 - strlen(cabecalho.c_str()), ' ');
					fprintf(lf, "%s", cabecalho.c_str());

					fflush(lf); // Força a atualização do arquivo
				}
				break;

			default:
				cout << "Opção inválida." << endl;
				pause();
				break;
		}

	} while (opcao != -1);

	fclose(lf);
	
	return 0;
}
