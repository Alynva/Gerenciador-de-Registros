#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <algorithm>
#include "clear_console.cpp"
#include "pause_console_with_message.cpp"
#include "to_string.cpp"

#define TAM_CHAVE 3
#define TAM_NUMERO_DDD 2
#define TAM_NUMERO_PREFIXO 5
#define TAM_NUMERO_SUFIXO 4
#define TAM_NUMERO TAM_NUMERO_DDD+TAM_NUMERO_PREFIXO+TAM_NUMERO_SUFIXO
#define TAM_DATA_DIA 2
#define TAM_DATA_MES 2
#define TAM_DATA_ANO 4
#define TAM_DATA TAM_DATA_DIA+TAM_DATA_MES+TAM_DATA_ANO
#define TAM_EMAIL 30
#define TAM_NOME 48

#define TAM_BLOCK 512
#define TAM_CABECALHO 50
#define TAM_REGISTRO 100

#define RRN2NBLOCK(rrn) (rrn / TAM_BLOCK)
#define RRN2REGINBLOCK(rrn) (rrn - RRN2NBLOCK(rrn) * TAM_BLOCK)

using namespace std;

int n_registros = 0,
	n_excluidos = 0;

bool strIsAlpha(const string& s) {
    return (unsigned int) count_if(s.begin(), s.end(), [](unsigned char c){ return isalpha(c); }) == (unsigned int) strlen(s.c_str());
}
bool strIsInteger(const std::string & s) {
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

   char * p;
   strtol(s.c_str(), &p, 10);

   return (*p == 0);
}

int busca(string chave) {
	int pos = -1;

	FILE *lf = fopen("arquivo.bin", "r+b");
	char buffer[TAM_BLOCK];
	
	if(lf == NULL) { // Verifica erros
		printf("Erro na abertura do arquivo.\n");
		return 2;
	}

	int buffer_index = -1;
	bool achou = false;
	char test_chave[3] = "";

	do {
		buffer_index++;
		fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
		fread(buffer, sizeof(char), sizeof(buffer), lf);

		//printf("buffer_index: %d\n", buffer_index);
		//printf("\n\nbuffer:\n%s\n\n", buffer);

		int seek_buffer = 0;
		int max_registros;
		if (buffer_index == 0) {
			seek_buffer = TAM_CABECALHO;
		}

		int tot_registros = n_registros + n_excluidos;

		if (buffer_index == 0) { // Está no primeiro bloco
			max_registros = tot_registros > 4 ? 4 : tot_registros;
		} else if (4 + buffer_index * 5 <= tot_registros) { // Está noutro bloco, completo
			max_registros = 5;
		} else { // Está noutro bloco, incompleto
			max_registros = (tot_registros - 4) % 5;
		}
		//cout << endl << "max_registros " << max_registros << endl;

		for (int i = 0, registro_index = 0; i < max_registros; i++, registro_index++) {
			// REABRE POR QUE POR ALGUM MOTIVO A FUNÇÃO SSCANF "ZERA" O BUFFER
			//cout << "fseek: " << buffer_index * TAM_BLOCK << endl;
			fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
			fread(buffer, sizeof(char), sizeof(buffer), lf);

			//cout << "buffer: " << buffer << endl;

			string pattern;
			if (seek_buffer + registro_index * TAM_REGISTRO > 0) {
				pattern = "%*";
				pattern.append(to_string(seek_buffer + registro_index * TAM_REGISTRO));
				pattern.append("[^\n]%");
			} else {
				pattern = "%";
			}
			pattern.append(to_string(TAM_CHAVE));
			pattern.append("[^\n]");

			//cout << endl << "pattern: " << pattern << endl;

			sscanf(buffer, pattern.c_str(), test_chave);

			//cout << endl << "test_chave: " << test_chave << "\tchave: " << chave << endl;
			if (!strcmp(test_chave, chave.c_str())) {
				pos = buffer_index * TAM_BLOCK + seek_buffer + registro_index * TAM_REGISTRO;
				//cout << endl << "pos: " << pos << endl;

				achou = true;
				break;
			}
		}
		//cout << "\nfim\n";

	} while (!achou && buffer_index < (n_registros + n_excluidos) / 5);

	return pos;
}

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
	sscanf(buffer, "N reg: %d\tN exl: %d", &n_registros, &n_excluidos); // Lê do arquivo o total de registros atual

	int opcao = 0;
	do { // Loop para mostrar o menu
		system(CLEAR);

		printf("Menu:\n" \
				"\t-1 - Sair\n" \
				"\t 0 - Zerar arquivo\n" \
				"\t 1 - Inserir novo registro\n" \
				"\t 2 - Busca por registro\n" \
				"\t 3 - Remover registro\n" \
				"\t 4 - Listar todos os registros");

		printf("\nDigite o opcao: ");
		scanf("%d", &opcao);
		cout << endl;

		switch (opcao) {
			case -1: // Sair
				break;
			case 0: // Zerar arquivo
				{
					pause("zerar o arquivo"); // Confirma a ação com o usuário

					lf = freopen("arquivo.bin", "w+b", lf); // Reabre o arquivo em w+ para zerá-lo
					
					if(lf == NULL) { // Verifica erros
						printf("Erro na criaçao do arquivo.\n");
						return 1;
					}

					n_registros = 0; // Zera o contador de registros
					n_excluidos = 0;
					
					buffer_index = 0;
					for (int i = 0; i < TAM_BLOCK; i++)
						buffer[i] = ' ';

					cabecalho = "N reg: ";
					cabecalho.append(to_string(n_registros));
					cabecalho.append("\tN exl: ");
					cabecalho.append(to_string(n_excluidos));
					cabecalho.append(TAM_CABECALHO - strlen(cabecalho.c_str()), ' '); // Preenche a string com espaço até dar o tamanho do cabeçalho
					char first_char = buffer[TAM_CABECALHO];
					sprintf(buffer, "%s", cabecalho.c_str()); // Imprime o cabeçalho
					buffer[TAM_CABECALHO] = first_char;

					fwrite(buffer, sizeof(char), sizeof(buffer), lf);
					fflush(lf); // Força a atualização do arquivo
				}
				break;
			case 1: // Inserir novo registro
				{ // '{}' necessário para criar um escopo e poder declarar variáveis nele
					lf = freopen("arquivo.bin", "r+b", lf);
					
					if(lf == NULL) { // Verifica erros
						printf("Erro na abertura do arquivo.\n");
						return 2;
					}

					string chave = "";
					while (strlen(chave.c_str()) != TAM_CHAVE || !strIsAlpha(chave)) { // Pede a chave do registro enquanto a inserida não tiver um comprimento de 3 caracteres
						cout << "Insira uma chave alfabetica para o registro (XXX): ";
						cin >> chave;
					}

					string numero_ddd = "", numero_prefixo = "", numero_sufixo = "";
					while (
						strlen(numero_ddd.c_str()) != TAM_NUMERO_DDD || !strIsInteger(numero_ddd) ||
						strlen(numero_prefixo.c_str()) != TAM_NUMERO_PREFIXO || !strIsInteger(numero_prefixo) ||
						strlen(numero_sufixo.c_str()) != TAM_NUMERO_SUFIXO || !strIsInteger(numero_sufixo)
					) { // Pede o numero (DDD, prefixo e sufixo) enquanto não tiverem o comprimento 2, 5 e 4, respectivamente, de caracteres
						cout << "Insira o numero de telefone (XX XXXXX XXXX): ";
						cin >> numero_ddd >> numero_prefixo >> numero_sufixo;
					}

					string data_dia = "", data_mes = "", data_ano = "";
					while (
						strlen(data_dia.c_str()) != TAM_DATA_DIA || !strIsInteger(data_dia) ||
						strlen(data_mes.c_str()) != TAM_DATA_MES || !strIsInteger(data_mes) ||
						strlen(data_ano.c_str()) != TAM_DATA_ANO || !strIsInteger(data_ano)
					) { // Pede a dara (dia, mês e ano) enquanto nao tiverem o comprimento 2, 2, e 4, respectivamente, de caracteres
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
					int pos = -1;
					if (n_excluidos == 0) {
						buffer_index = (n_registros < 4) ? 0 : (n_registros + 1) / 5;
					} else {
						pos = busca("***");
						buffer_index = RRN2NBLOCK(pos);
					}
					fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
					int total_read = fread(buffer, sizeof(char), sizeof(buffer), lf);
					for (unsigned int i = total_read; i < sizeof(buffer); i++)
						buffer[i] = ' ';

					int reg_pos;
					if (n_excluidos == 0) {
						if (n_registros < 4) {
							reg_pos = TAM_CABECALHO + n_registros * TAM_REGISTRO;
						} else {
							reg_pos = ((n_registros + 1) % 5) * TAM_REGISTRO;
						}
					} else {
						reg_pos = RRN2REGINBLOCK(pos);
					}

					char first_char = buffer[(reg_pos + TAM_REGISTRO)];

					snprintf(
						buffer + reg_pos,
						TAM_CHAVE+1,
						"%s",
						chave.c_str()
					);
					snprintf(
						buffer + reg_pos + TAM_CHAVE,
						(TAM_NUMERO_DDD + TAM_NUMERO_PREFIXO + TAM_NUMERO_SUFIXO)+1,
						"%s%s%s",
						numero_ddd.c_str(),
						numero_prefixo.c_str(),
						numero_sufixo.c_str()
					);
					snprintf(
						buffer + reg_pos + TAM_CHAVE + (TAM_NUMERO_DDD + TAM_NUMERO_PREFIXO + TAM_NUMERO_SUFIXO),
						(TAM_DATA_DIA + TAM_DATA_MES + TAM_DATA_ANO)+1,
						"%s%s%s",
						data_dia.c_str(),
						data_mes.c_str(),
						data_ano.c_str()
					);
					snprintf(
						buffer + reg_pos + TAM_CHAVE + (TAM_NUMERO_DDD + TAM_NUMERO_PREFIXO + TAM_NUMERO_SUFIXO) + (TAM_DATA_DIA + TAM_DATA_MES + TAM_DATA_ANO),
						TAM_EMAIL+1,
						"%s",
						email.c_str()
					);
					snprintf(
						buffer + reg_pos + TAM_CHAVE + (TAM_NUMERO_DDD + TAM_NUMERO_PREFIXO + TAM_NUMERO_SUFIXO) + (TAM_DATA_DIA + TAM_DATA_MES + TAM_DATA_ANO) + TAM_EMAIL,
						TAM_NOME+1,
						"%s",
						nome.c_str()
					);
					
					buffer[(reg_pos + TAM_REGISTRO)] = first_char;

					n_registros++;
					if (n_excluidos > 0) {
						n_excluidos--;
					}

					cabecalho = "N reg: ";
					cabecalho.append(to_string(n_registros));
					cabecalho.append("\tN exl: ");
					cabecalho.append(to_string(n_excluidos));
					cabecalho.append(TAM_CABECALHO - strlen(cabecalho.c_str()), ' ');
					if (buffer_index == 0) {
						char first_char = buffer[TAM_CABECALHO];
						snprintf(buffer, TAM_CABECALHO + 1, "%s", cabecalho.c_str());
						buffer[TAM_CABECALHO] = first_char;
					}

					fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
					fwrite(buffer, sizeof(char), sizeof(buffer), lf);

					fflush(lf); // Força a atualização do arquivo

					// Reabre o arquivo e atualiza o cabeçalho
					if (buffer_index != 0) {
						fseek(lf, 0, SEEK_SET);
						fread(buffer, sizeof(char), sizeof(buffer), lf);
						char first_char = buffer[TAM_CABECALHO];
						snprintf(buffer, TAM_CABECALHO + 1, "%s", cabecalho.c_str());
						buffer[TAM_CABECALHO] = first_char;
						fseek(lf, 0, SEEK_SET);
						fwrite(buffer, sizeof(char), sizeof(buffer), lf);
						fflush(lf);
					}

				}
				break;

			case 2: // BUSCAR
				{
					string chave = "";
					do {
						cout << "Insira uma chave alfabetica para ser buscada (XXX): ";
						cin >> chave;
					} while (strlen(chave.c_str()) != 3 || !strIsAlpha(chave));

					int pos = busca(chave);
					bool achou = pos != -1;

					cout << endl;

					if (!achou) {
						cout << "Nao foi encontrado nenhum registro com esta chave." << endl;
					} else {
						int buffer_index = RRN2NBLOCK(pos);
						//cout << endl << buffer_index << endl;


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

						int j = RRN2REGINBLOCK(pos);
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

						cout << "A chave foi encontrada! Aqui estao os dados:" << endl << endl;
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

			case 3: // EXCLUIR
				{
					string chave = "";
					do {
						cout << "Insira uma chave alfabetica para ser excluida (XXX): ";
						cin >> chave;
					} while (strlen(chave.c_str()) != 3 || !strIsAlpha(chave));

					int pos = busca(chave);
					bool achou = pos != -1;

					cout << endl;

					if (!achou) {
						cout << "Nao foi encontrado nenhum registro com esta chave." << endl;
					} else {
						int buffer_index = pos / TAM_BLOCK;

						--n_registros;
						++n_excluidos;
						
						fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
						fread(buffer, sizeof(char), sizeof(buffer), lf);

						int j = pos - buffer_index * TAM_BLOCK;
						for (int k = 0; k < TAM_CHAVE; j++, k++) {
							buffer[j] = '*';
						}

						cabecalho = "N reg: ";
						cabecalho.append(to_string(n_registros));
						cabecalho.append("\tN exl: ");
						cabecalho.append(to_string(n_excluidos));
						cabecalho.append(TAM_CABECALHO - strlen(cabecalho.c_str()), ' ');
						if (buffer_index == 0) {
							char first_char = buffer[TAM_CABECALHO];
							snprintf(buffer, TAM_CABECALHO + 1, "%s", cabecalho.c_str());
							buffer[TAM_CABECALHO] = first_char;
						}
						
						fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
						fwrite(buffer, sizeof(char), sizeof(buffer), lf);

						fflush(lf);

						if (buffer_index != 0) {
							fseek(lf, 0, SEEK_SET);
							fread(buffer, sizeof(char), sizeof(buffer), lf);
							char first_char = buffer[TAM_CABECALHO];
							snprintf(buffer, TAM_CABECALHO + 1, "%s", cabecalho.c_str());
							buffer[TAM_CABECALHO] = first_char;
							fseek(lf, 0, SEEK_SET);
							fwrite(buffer, sizeof(char), sizeof(buffer), lf);
							fflush(lf);
						}

						
						cout << "O registro foi excluido com sucesso." << endl;
					}

					pause();
				}
				break;

			case 4:
				{
					cout << endl;

					if (n_registros > 0) {
						int buffer_index = -1;
								
						string found_chave,
							found_numero_ddd,
							found_numero_prefixo,
							found_numero_sufixo,
							found_data_dia,
							found_data_mes,
							found_data_ano,
							found_email,
							found_nome;

						cout << "Chave";
						for (int i = 0; i < TAM_CHAVE - 5; i++)
							cout << " ";
						cout << "\tNumero";
						for (int i = 0; i < (TAM_NUMERO + 4) - 6; i++)
							cout << " ";
						cout << "\tData";
						for (int i = 0; i < (TAM_DATA + 2) - 4; i++)
							cout << " ";
						cout << "\tE-mail";
						for (int i = 0; i < TAM_EMAIL - 6; i++)
							cout << " ";
						cout << "\tNome";
						for (int i = 0; i < TAM_NOME - 4; i++)
							cout << " ";
						cout << endl << endl;


						do {
							buffer_index++;
							//fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
							//fread(buffer, sizeof(char), sizeof(buffer), lf);

							int seek_buffer = 0;
							int max_registros;
							if (buffer_index == 0) {
								seek_buffer = TAM_CABECALHO;
							}

							int tot_registros = n_registros + n_excluidos;

							if (buffer_index == 0) { // Está no primeiro bloco
								max_registros = tot_registros > 4 ? 4 : tot_registros;
							} else if (4 + buffer_index * 5 <= tot_registros) { // Está noutro bloco, completo
								max_registros = 5;
							} else { // Está noutro bloco, incompleto
								max_registros = (tot_registros - 4) % 5;
							}

							for (int i = 0, registro_index = 0; i < max_registros; i++, registro_index++) {
								// ABRE O ARQUIVO TODA VEZ POR QUE POR ALGUM MOTIVO A FUNÇÃO SSCANF "ZERA" O BUFFER
								fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
								fread(buffer, sizeof(char), sizeof(buffer), lf);

								string pattern;
								if (seek_buffer + registro_index * TAM_REGISTRO > 0) {
									pattern = "%*";
									pattern.append(to_string(seek_buffer + registro_index * TAM_REGISTRO));
									pattern.append("[^\n]%");
								} else {
									pattern = "%";
								}
								pattern.append(to_string(TAM_CHAVE));
								pattern.append("[^\n]");

								//OBTEVE O REGISTRO
								int pos = buffer_index * TAM_BLOCK + seek_buffer + registro_index * TAM_REGISTRO;

								
								found_chave = "",
									found_numero_ddd = "",
									found_numero_prefixo = "",
									found_numero_sufixo = "",
									found_data_dia = "",
									found_data_mes = "",
									found_data_ano = "",
									found_email = "",
									found_nome = "";

								int j = RRN2REGINBLOCK(pos);
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

								if (found_chave != "***") {
									cout << found_chave << "\t";
									cout << "(" << found_numero_ddd << ") " << found_numero_prefixo << "-" << found_numero_sufixo << "\t";
									cout << found_data_dia << "/" << found_data_mes << "/" << found_data_ano << "\t";
									cout << found_email << "\t";
									cout << found_nome << "\t";
									cout << endl;
								}

							}

						} while (buffer_index < (n_registros + n_excluidos) / 5);
					} else {
						cout << "Nao ha nenhum registro no arquivo." << endl;
					}

					cout << endl;
					pause();
				}
				break;
			default:
				cout << "Opcao inválida." << endl;
				pause();
				break;
		}

	} while (opcao != -1);

	fclose(lf);
	
	return 0;
}
