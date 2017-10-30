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

#define TAM_BLOCK 512
#define TAM_CABECALHO 50
#define TAM_REGISTRO 100

using namespace std;

int main() {
	
	FILE *lf;
	lf = fopen("arquivo.bin", "a+b"); // Abre o arquivo, caso exista, ou cria um vazio
	
	if(lf == NULL) { // Verifica a ocorrência de erro ao abrir o arquivo
		printf("Erro na abertura do arquivo.\n");
		return 2;
	}
	rewind(lf); // Move o ponteiro de leitura/escrita para o começo do arquivo

	int buffer_index = 0;
	char buffer[TAM_BLOCK];
	fread(buffer, sizeof(char), sizeof(buffer), lf);

	string cabecalho; // Variável utilizada para escrever o texto do cabeçalho
	int n_registros = 0;
	sscanf(buffer, "Numero de registros: %d", &n_registros); // Lê do arquivo o total de registros atual

	int opcao = 0;
	do { // Loop para mostrar o menu
		system(CLEAR);

		printf("Menu:\n" \
				"\t-1 - Sair\n" \
				"\t 0 - Zerar arquivo\n" \
				"\t 1 - Inserir novo registro\n" \
				"\t 2 - Busca por registro\n" \
				"\t 3 - Remover registro");

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
				
				buffer_index = 0;
				for (int i = 0; i < TAM_BLOCK; i++)
					buffer[i] = ' ';

				cabecalho = "Numero de registros: ";
				cabecalho.append(to_string(n_registros));
				cabecalho.append(TAM_CABECALHO - strlen(cabecalho.c_str()), ' '); // Preenche a string com espaço até dar o tamanho do cabeçalho
				sprintf(buffer, "%s", cabecalho.c_str()); // Imprime o cabeçalho

				fwrite(buffer, sizeof(char), sizeof(buffer), lf);
				fflush(lf); // Força a atualização do arquivo
				break;
			case 1:
				{ // '{}' necessário para criar um escopo e poder declarar variáveis nele
					lf = freopen("arquivo.bin", "r+b", lf);
					
					if(lf == NULL) { // Verifica erros
						printf("Erro na abertura do arquivo.\n");
						return 2;
					}

					buffer_index = (n_registros < 4) ? 0 : (n_registros + 1) / 5;
					fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
					int total_read = fread(buffer, sizeof(char), sizeof(buffer), lf);
					for (unsigned int i = total_read; i < sizeof(buffer); i++)
						buffer[i] = ' ';

					cout << "Numero do bloco:" << endl << buffer_index << endl << endl;
					cout << "Bloco:" << endl << buffer << endl << endl;

					string chave = "";
					while (strlen(chave.c_str()) != TAM_CHAVE) { // Pede a chave do registro enquanto a inserida não tiver um comprimento de 3 caracteres
						cout << "Insira a chave para o registro (XXX): ";
						cin >> chave;
					}

					string numero_ddd = "", numero_prefixo = "", numero_sufixo = "";
					while (strlen(numero_ddd.c_str()) != TAM_NUMERO_DDD || strlen(numero_prefixo.c_str()) != TAM_NUMERO_PREFIXO || strlen(numero_sufixo.c_str()) != TAM_NUMERO_SUFIXO) { // Pede o Numero (DDD, prefixo e sufixo) enquanto não tiverem o comprimento 2, 5 e 4, respectivamente, de caracteres
						cout << "Insira o Numero de telefone (XX XXXXX XXXX): ";
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
					cout << "Chave:\t\t\t" << chave << endl;
					cout << "Telefone:\t\t(" << numero_ddd << ") " << numero_prefixo << "-" << numero_sufixo << endl;
					cout << "Data de nascimento:\t" << data_dia << "/" << data_mes << "/" << data_ano << endl;
					cout << "E-mail:\t\t\t" << email << endl;
					cout << "Nome:\t\t\t" << nome << endl << endl;

					pause("gravar o registro"); // Confirma a ação com o usuário

					// Grava os dados
					if (n_registros < 4) {
						cout <<  (TAM_CABECALHO + n_registros * TAM_REGISTRO) << endl;
						snprintf(
							buffer + (TAM_CABECALHO + n_registros * TAM_REGISTRO),
							TAM_CHAVE+1,
							"%s",
							chave.c_str()
						);
						snprintf(
							buffer + (TAM_CABECALHO + n_registros * TAM_REGISTRO) + TAM_CHAVE,
							(TAM_NUMERO_DDD + TAM_NUMERO_PREFIXO + TAM_NUMERO_SUFIXO)+1,
							"%s%s%s",
							numero_ddd.c_str(),
							numero_prefixo.c_str(),
							numero_sufixo.c_str()
						);
						snprintf(
							buffer + (TAM_CABECALHO + n_registros * TAM_REGISTRO) + TAM_CHAVE + (TAM_NUMERO_DDD + TAM_NUMERO_PREFIXO + TAM_NUMERO_SUFIXO),
							(TAM_DATA_DIA + TAM_DATA_MES + TAM_DATA_ANO)+1,
							"%s%s%s",
							data_dia.c_str(),
							data_mes.c_str(),
							data_ano.c_str()
						);
						snprintf(
							buffer + (TAM_CABECALHO + n_registros * TAM_REGISTRO) + TAM_CHAVE + (TAM_NUMERO_DDD + TAM_NUMERO_PREFIXO + TAM_NUMERO_SUFIXO) + (TAM_DATA_DIA + TAM_DATA_MES + TAM_DATA_ANO),
							TAM_EMAIL+1,
							"%s",
							email.c_str()
						);
						snprintf(
							buffer + (TAM_CABECALHO + n_registros * TAM_REGISTRO) + TAM_CHAVE + (TAM_NUMERO_DDD + TAM_NUMERO_PREFIXO + TAM_NUMERO_SUFIXO) + (TAM_DATA_DIA + TAM_DATA_MES + TAM_DATA_ANO) + TAM_EMAIL,
							TAM_NOME+1,
							"%s",
							nome.c_str()
						);

					} else {

						snprintf(
							buffer + ((n_registros + 1) % 5) * TAM_REGISTRO,
							TAM_CHAVE+1,
							"%s",
							chave.c_str()
						);
						snprintf(
							buffer + ((n_registros + 1) % 5) * TAM_REGISTRO + TAM_CHAVE,
							(TAM_NUMERO_DDD + TAM_NUMERO_PREFIXO + TAM_NUMERO_SUFIXO)+1,
							"%s%s%s",
							numero_ddd.c_str(),
							numero_prefixo.c_str(),
							numero_sufixo.c_str()
						);
						snprintf(
							buffer + ((n_registros + 1) % 5) * TAM_REGISTRO + TAM_CHAVE + (TAM_NUMERO_DDD + TAM_NUMERO_PREFIXO + TAM_NUMERO_SUFIXO),
							(TAM_DATA_DIA + TAM_DATA_MES + TAM_DATA_ANO)+1,
							"%s%s%s",
							data_dia.c_str(),
							data_mes.c_str(),
							data_ano.c_str()
						);
						snprintf(
							buffer + ((n_registros + 1) % 5) * TAM_REGISTRO + TAM_CHAVE + (TAM_NUMERO_DDD + TAM_NUMERO_PREFIXO + TAM_NUMERO_SUFIXO) + (TAM_DATA_DIA + TAM_DATA_MES + TAM_DATA_ANO),
							TAM_EMAIL+1,
							"%s",
							email.c_str()
						);
						snprintf(
							buffer + ((n_registros + 1) % 5) * TAM_REGISTRO + TAM_CHAVE + (TAM_NUMERO_DDD + TAM_NUMERO_PREFIXO + TAM_NUMERO_SUFIXO) + (TAM_DATA_DIA + TAM_DATA_MES + TAM_DATA_ANO) + TAM_EMAIL,
							TAM_NOME+1,
							"%s",
							nome.c_str()
						);

					}

					++n_registros;

					fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
					fwrite(buffer, sizeof(char), sizeof(buffer), lf);

					// Reabre o arquivo e atualiza o cabeçalho
					lf = freopen("arquivo.bin", "r+b", lf);
					rewind(lf);
					cabecalho = "Numero de registros: ";
					cabecalho.append(to_string(n_registros));
					cabecalho.append(TAM_CABECALHO - strlen(cabecalho.c_str()), ' ');
					fprintf(lf, "%s", cabecalho.c_str());

					fflush(lf); // Força a atualização do arquivo
				}
				break;

			case 2:
				{
					string chave = "";
					do {
						cout << "Insira a chave para ser buscada (XXX): ";
						cin >> chave;
					} while (strlen(chave.c_str()) != 3);

					lf = freopen("arquivo.bin", "r+b", lf);
					
					if(lf == NULL) { // Verifica erros
						printf("Erro na abertura do arquivo.\n");
						return 2;
					}

					int buffer_index = -1;
					bool achou = false;
					char test_chave[3] = "";
							fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
							fread(buffer, sizeof(char), sizeof(buffer), lf);

					string found_chave = "",
						found_numero_ddd = "",
						found_numero_prefixo = "",
						found_numero_sufixo = "",
						found_data_dia = "",
						found_data_mes = "",
						found_data_ano = "",
						found_email = "",
						found_nome = "";

					do {
						buffer_index++;
						fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
						fread(buffer, sizeof(char), sizeof(buffer), lf);

						int seek_buffer = 0;
						int max_registros = 5;
						if (buffer_index == 0) {
							seek_buffer = TAM_CABECALHO;
							max_registros = 4;
						}

						if (n_registros / 5 + 1 == 1) {
							max_registros = n_registros;
						} else if (buffer_index < n_registros / 5 - 1) {
							max_registros = 5;
						} else {
							max_registros = (n_registros % 5) + 1;
						}

						for (int i = 0, registro_index = 0; i < max_registros; i++, registro_index++) {
							fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
							fread(buffer, sizeof(char), sizeof(buffer), lf);

							string pattern = "%*";
							pattern.append(to_string(seek_buffer + registro_index * TAM_REGISTRO));
							pattern.append("[^\n]%");
							pattern.append(to_string(TAM_CHAVE));
							pattern.append("[^\n]");

							sscanf(buffer, pattern.c_str(), test_chave);

							if (!strcmp(test_chave, chave.c_str())) {
								int j = seek_buffer + registro_index * TAM_REGISTRO;
								for (int k = 0; k < TAM_CHAVE; j++, k++) {
									found_chave.append(to_string(buffer[j]));
								}
								for (int k = 0; k < TAM_NUMERO_DDD; j++, k++) {
									found_numero_ddd.append(to_string(buffer[j]));
								}
								for (int k = 0; k < TAM_NUMERO_PREFIXO; j++, k++) {
									found_numero_prefixo.append(to_string(buffer[j]));
								}
								for (int k = 0; k < TAM_NUMERO_SUFIXO; j++, k++) {
									found_numero_sufixo.append(to_string(buffer[j]));
								}
								for (int k = 0; k < TAM_DATA_DIA; j++, k++) {
									found_data_dia.append(to_string(buffer[j]));
								}
								for (int k = 0; k < TAM_DATA_MES; j++, k++) {
									found_data_mes.append(to_string(buffer[j]));
								}
								for (int k = 0; k < TAM_DATA_ANO; j++, k++) {
									found_data_ano.append(to_string(buffer[j]));
								}
								for (int k = 0; k < TAM_EMAIL; j++, k++) {
									found_email.append(to_string(buffer[j]));
								}
								for (int k = 0; k < TAM_NOME; j++, k++) {
									found_nome.append(to_string(buffer[j]));
								}

								achou = true;
								break;
							}
						}

					} while (!achou && buffer_index < n_registros / 5);

					cout << endl;

					if (!achou) {
						cout << "Não foi encontrado nenhum registro com esta chave." << endl;
					} else {
						cout << "A chave foi encontrada! Aqui estão os dados:" << endl << endl;
						cout << "Chave:\t\t\t" << found_chave << endl;
						cout << "Telefone:\t\t(" << found_numero_ddd << ") " << found_numero_prefixo << "-" << found_numero_sufixo << endl;
						cout << "Data de nascimento:\t" << found_data_dia << "/" << found_data_mes << "/" << found_data_ano << endl;
						cout << "E-mail:\t\t\t" << found_email << endl;
						cout << "Nome:\t\t\t" << found_nome << endl;
					}

					cout << endl;

					pause();
				}
				break;

			case 3:
				{
					string chave = "";
					do {
						cout << "Insira a chave para ser excluída (XXX): ";
						cin >> chave;
					} while (strlen(chave.c_str()) != 3);

					lf = freopen("arquivo.bin", "r+b", lf);
					
					if(lf == NULL) { // Verifica erros
						printf("Erro na abertura do arquivo.\n");
						return 2;
					}

					int buffer_index = -1,
						seek_buffer = 0,
						registro_index = 0;
					bool achou = false;
					char test_chave[3] = "";

					do {
						buffer_index++;

						seek_buffer = 0;
						int max_registros = 5;
						if (buffer_index == 0) {
							seek_buffer = TAM_CABECALHO;
							max_registros = 4;
						}

						if (n_registros / 5 + 1 == 1) {
							max_registros = n_registros;
						} else if (buffer_index < n_registros / 5 - 1) {
							max_registros = 5;
						} else {
							max_registros = (n_registros % 5) + 1;
						}

						registro_index = 0;
						for (int i = 0; i < max_registros; i++, registro_index++) {
							fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
							fread(buffer, sizeof(char), sizeof(buffer), lf);

							string pattern = "%*";
							pattern.append(to_string(seek_buffer + registro_index * TAM_REGISTRO));
							pattern.append("[^\n]%");
							pattern.append(to_string(TAM_CHAVE));
							pattern.append("[^\n]");

							sscanf(buffer, pattern.c_str(), test_chave);

							if (!strcmp(test_chave, chave.c_str())) {
								achou = true;
								break;
							}
						}

					} while (!achou && buffer_index < n_registros / 5);

					cout << endl;

					if (!achou) {
						cout << "Não foi encontrado nenhum registro com esta chave." << endl;
					} else {
						
						fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
						fread(buffer, sizeof(char), sizeof(buffer), lf);

						int j = seek_buffer + registro_index * TAM_REGISTRO;
						for (int k = 0; k < TAM_CHAVE; j++, k++) {
							buffer[j] = '*';
						}
						
						fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
						fwrite(buffer, sizeof(char), sizeof(buffer), lf);

						fflush(lf);
						
						cout << "O registro foi excluído com sucesso." << endl;
					}

					pause();
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
