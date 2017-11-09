#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <algorithm>
#include "clear_console.cpp"
#include "pause_console_with_message.cpp"
#include "to_string.cpp"
#include "strIsAlpha.cpp"
#include "strIsInteger.cpp"
#include "ppAlpha.cpp"

#include "macros.cpp"

using namespace std;

void zerar(string);
int busca(string, string);
void insere(string, string, string, string, string, string, string, string, string, string);
int listagem(string, bool = true);
void remove(string, string, bool = true);

int main() {
	
	FILE *lf = fopen(FILE_NAME, "a+b"); // Abre o arquivo, caso exista, ou cria um vazio
	
	if(lf == NULL) { // Verifica a ocorrência de erro ao abrir o arquivo
		printf("Erro na abertura do arquivo.\n");
		return 2;
	}
	rewind(lf); // Move o ponteiro de leitura/escrita para o começo do arquivo

	char buffer[TAM_BLOCK];

	string cabecalho; // Variável utilizada para escrever o texto do cabeçalho

	int opcao = 0;
	do { // Loop para mostrar o menu
		system(CLEAR);

		printf("Menu:\n" \
				"\t-1 - Sair\n" \
				"\t 0 - Zerar arquivo\n" \
				"\t 1 - Inserir novo registro\n" \
				"\t 2 - Busca por registro\n" \
				"\t 3 - Remover registro\n" \
				"\t 4 - Listar todos os registros\n" \
				"\t 5 - Compactar\n\n" \
				"\t99 - Inserir em lote\n");

		printf("\nDigite o opcao: ");
		scanf("%d", &opcao);
		cout << endl;

		switch (opcao) {
			case -1: // Sair
				break;
			case 0: // Zerar arquivo
				{
					pause("zerar o arquivo"); // Confirma a ação com o usuário

					zerar(FILE_NAME);
				}
				break;

			case 1: // Inserir novo registro
				{ // '{}' necessário para criar um escopo e poder declarar variáveis nele

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
					cout << "\tChave:\t\t\t" << chave << endl;
					cout << "\tTelefone:\t\t(" << numero_ddd << ") " << numero_prefixo << "-" << numero_sufixo << endl;
					cout << "\tData de nascimento:\t" << data_dia << "/" << data_mes << "/" << data_ano << endl;
					cout << "\tE-mail:\t\t\t" << email << endl;
					cout << "\tNome:\t\t\t" << nome << endl << endl;

					pause("gravar o registro"); // Confirma a ação com o usuário

					// Grava os dados
					insere(FILE_NAME, chave, numero_ddd, numero_prefixo, numero_prefixo, data_dia, data_mes, data_ano, email, nome);

				}
				break;

			case 2: // BUSCAR
				{
					string chave = "";
					do {
						cout << "Insira uma chave alfabetica para ser buscada (XXX): ";
						cin >> chave;
					} while (strlen(chave.c_str()) != 3 || !strIsAlpha(chave));

					int pos = busca(FILE_NAME, chave);
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
						cout << "\tChave:\t\t\t" << found_chave << endl;
						cout << "\tTelefone:\t\t(" << found_numero_ddd << ") " << found_numero_prefixo << "-" << found_numero_sufixo << endl;
						cout << "\tData de nascimento:\t" << found_data_dia << "/" << found_data_mes << "/" << found_data_ano << endl;
						cout << "\tE-mail:\t\t\t" << found_email << endl;
						cout << "\tNome:\t\t\t" << found_nome << endl;
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

					remove(FILE_NAME, chave);

					pause();
				}
				break;

			case 4: // LISTAGEM
				{
					lf = freopen(FILE_NAME, "r+b", lf);
					fseek(lf, 0, SEEK_SET);
					fread(buffer, sizeof(char), sizeof(buffer), lf);

					int n_registros = 0,
						n_excluidos = 0;
					sscanf(buffer, "N reg: %d\tN exl: %d", &n_registros, &n_excluidos); // Lê do arquivo o total de registros atual

					if (n_registros > 0) {
						listagem(FILE_NAME);
					} else {
						cout << "Nao ha nenhum registro no arquivo.";
						if (n_excluidos > 0)
							cout << " Existe(m) " << n_excluidos << " registro(s) logicamente removido(s).";
						cout << endl;
					}

					cout << endl;
					pause();
				}
				break;

			case 5: // COMPACTAR
				{
					lf = freopen(FILE_NAME, "r+b", lf);
					fseek(lf, 0, SEEK_SET);
					fread(buffer, sizeof(char), sizeof(buffer), lf);

					int n_registros = 0,
						n_excluidos = 0;
					sscanf(buffer, "N reg: %d\tN exl: %d", &n_registros, &n_excluidos); // Lê do arquivo o total de registros atual

					if (n_excluidos > 0) {
						zerar("_new.bin");

						int rrn_first_reg = listagem(FILE_NAME, false);

						while (rrn_first_reg != -1) {
							fseek(lf, RRN2NBLOCK(rrn_first_reg) * TAM_BLOCK, SEEK_SET);
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

							int j = RRN2REGINBLOCK(rrn_first_reg);
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

							insere("_new.bin", found_chave, found_numero_ddd, found_numero_prefixo, found_numero_sufixo, found_data_dia, found_data_mes, found_data_ano, found_email, found_nome);

							remove(FILE_NAME, found_chave, false);

							rrn_first_reg = listagem(FILE_NAME, false);
						}

						rename("_new.bin", FILE_NAME);
						cout << "Compactacao realizada com sucesso!" << endl << endl;
					}
				}
				pause();
				break;

			case 99: // INSERIR EM LOTE
				{
					int count;
					cout << "Insira o numero de registros a serem inseridos: ";
					cin >> count;

					string string_chave = "aaa",
						string_numero_ddd = "00",
						string_numero_prefixo = "00000",
						string_numero_sufixo = "0000",
						string_data_dia = "00",
						string_data_mes = "00",
						string_data_ano = "0000",
						string_email = "aaaaa",
						string_nome = "Aaaa";

					int i = 0;
					while (i < count) {
						insere(FILE_NAME, string_chave, string_numero_ddd, string_numero_prefixo, string_numero_sufixo, string_data_dia, string_data_mes, string_data_ano, string_email, string_nome);

						string_chave = ppAlpha(string_chave);
						string_numero_ddd = ppAlpha(string_numero_ddd);
						string_numero_prefixo = ppAlpha(string_numero_prefixo);
						string_numero_sufixo = ppAlpha(string_numero_sufixo);
						string_data_dia = ppAlpha(string_data_dia);
						string_data_mes = ppAlpha(string_data_mes);
						string_data_ano = ppAlpha(string_data_ano);
						string_email = ppAlpha(string_email);
						string_nome = ppAlpha(string_nome);

						i++;
					}
					cout << endl << "Registros inseridos." << endl << endl;
					pause();
				}
				break;

			default:
				cout << "Opcao invalida." << endl;
				pause();
				break;
		}

	} while (opcao != -1);

	fclose(lf);
	
	return 0;
}

void zerar(string path) {
	FILE *lf = fopen(path.c_str(), "w+b"); // Reabre o arquivo em w+ para zerá-lo
					
	if(lf == NULL) { // Verifica erros
		printf("Erro na criaçao do arquivo.\n");
		return;
	}
	char buffer[TAM_BLOCK];
	int n_registros = 0; // Zera o contador de registros
	int n_excluidos = 0;
	
	for (int i = 0; i < TAM_BLOCK; i++)
		buffer[i] = ' ';

	string cabecalho = "N reg: ";
	cabecalho.append(to_string(n_registros));
	cabecalho.append("\tN exl: ");
	cabecalho.append(to_string(n_excluidos));
	cabecalho.append(TAM_CABECALHO - strlen(cabecalho.c_str()), ' '); // Preenche a string com espaço até dar o tamanho do cabeçalho
	char first_char = buffer[TAM_CABECALHO];
	sprintf(buffer, "%s", cabecalho.c_str()); // Imprime o cabeçalho
	buffer[TAM_CABECALHO] = first_char;

	fwrite(buffer, sizeof(char), sizeof(buffer), lf);

	fclose(lf);
}
int busca(string path, string chave) {
	int pos = -1;

	FILE *lf = fopen(path.c_str(), "r+b");
	char buffer[TAM_BLOCK];

	fread(buffer, sizeof(char), sizeof(buffer), lf);

	int n_registros = 0,
		n_excluidos = 0;
	sscanf(buffer, "N reg: %d\tN exl: %d", &n_registros, &n_excluidos); // Lê do arquivo o total de registros atual
	
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

	fclose(lf);

	return pos;
}
void insere(string path, string chave, string numero_ddd, string numero_prefixo, string numero_sufixo, string data_dia, string data_mes, string data_ano, string email, string nome) {

	char buffer[TAM_BLOCK];

	FILE *lf = fopen(path.c_str(), "r+b");
	fread(buffer, sizeof(char), sizeof(buffer), lf);

	int n_registros = 0,
		n_excluidos = 0;
	sscanf(buffer, "N reg: %d\tN exl: %d", &n_registros, &n_excluidos); // Lê do arquivo o total de registros atual
	
	if(lf == NULL) { // Verifica erros
		printf("Erro na abertura do arquivo.\n");
		return;
	}

	int buffer_index = 0;
	int pos = -1;
	if (n_excluidos == 0) {
		buffer_index = (n_registros < 4) ? 0 : (n_registros + 1) / 5;
	} else {
		pos = busca(path.c_str(), "***");
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

	while (email.length() < TAM_EMAIL)
		email.append(" ");

	while (nome.length() < TAM_NOME)
		nome.append(" ");

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

	string cabecalho = "N reg: ";
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

	fclose(lf);
}
int listagem(string path, bool print) {
	int rrn_first_reg = -1;

	FILE *lf = fopen(path.c_str(), "r+b");

	char buffer[TAM_BLOCK];
	fread(buffer, sizeof(char), sizeof(buffer), lf);

	int n_registros = 0,
		n_excluidos = 0;
	sscanf(buffer, "N reg: %d\tN exl: %d", &n_registros, &n_excluidos); // Lê do arquivo o total de registros atual

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

	if (print) {
		cout << "Nro";
		for (int i = 0; (unsigned int) i < (to_string(n_registros).length() > 3 ? to_string(n_registros).length() - 3 : 0); i++)
			cout << " ";
		cout << "\tChave";
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
	}

	unsigned int count = 1;
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
				if (rrn_first_reg == -1) {
					rrn_first_reg = pos;
				}

				if (print) {
					cout << count;
					for (int j = 0; (unsigned int) j < to_string(n_registros).length() - to_string(count).length(); j++)
						cout << " ";
					cout << "\t";
					count++;

					cout << found_chave << "\t";
					cout << "(" << found_numero_ddd << ") " << found_numero_prefixo << "-" << found_numero_sufixo << "\t";
					cout << found_data_dia << "/" << found_data_mes << "/" << found_data_ano << "\t";
					cout << found_email << "\t";
					cout << found_nome << "\t";
					cout << endl;
				}
			}

		}

	} while (buffer_index < (n_registros + n_excluidos) / 5);

	fclose(lf);

	return rrn_first_reg;
}
void remove(string path, string chave, bool print) {
	FILE *lf = fopen(path.c_str(), "r+b");

	int pos = busca(FILE_NAME, chave);
	bool achou = pos != -1;

	if (print)
		cout << endl;

	if (!achou) {
		if (print)
			cout << "Nao foi encontrado nenhum registro com esta chave." << endl;
	} else {
		int buffer_index = pos / TAM_BLOCK;
		
		char buffer[TAM_BLOCK];
		fseek(lf, 0, SEEK_SET);
		fread(buffer, sizeof(char), sizeof(buffer), lf);

		int n_registros = 0,
			n_excluidos = 0;
		sscanf(buffer, "N reg: %d\tN exl: %d", &n_registros, &n_excluidos); // Lê do arquivo o total de registros atual


		fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
		fread(buffer, sizeof(char), sizeof(buffer), lf);

		n_registros--;
		n_excluidos++;

		int j = pos - buffer_index * TAM_BLOCK;
		for (int k = 0; k < TAM_CHAVE; j++, k++) {
			buffer[j] = '*';
		}

		string cabecalho = "N reg: ";
		cabecalho.append(to_string(n_registros));
		cabecalho.append("\tN exl: ");
		cabecalho.append(to_string(n_excluidos));
		cabecalho.append(TAM_CABECALHO - strlen(cabecalho.c_str()), ' ');
		if (buffer_index == 0) {
			char first_char = buffer[TAM_CABECALHO];
			snprintf(buffer, TAM_CABECALHO + 1, "%s", cabecalho.c_str());
			buffer[TAM_CABECALHO] = first_char;
		}
		
		lf = freopen(FILE_NAME, "r+b", lf);
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

		if (print)
			cout << "O registro foi excluido com sucesso." << endl << endl;
	}

	fclose(lf);
}