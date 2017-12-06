#include <stdio.h>							// printf, scanf, sscanf, snprintf, fopen, freopen, fflush, fclose fread, fwrite, fseek, rewind
#include <iostream>							// std::string, std::cin, std::cout
#include <string.h>							// std::string.c_str(), std::string.append(), strlen, strcmp, stoi
#include <errno.h>							// strerror(errno)

#include "clear_console.cpp"				// Define CLEAR para funcionar multi-plataforma
#include "pausa_console_com_menssagem.cpp"	// Simula system("pause")
#include "to_string.cpp"					// Conversão de variáveis para string
#include "strIsAlpha.cpp"					// Verifica se uma string é completamente alfabética
#include "strIsInteger.cpp"					// Verifica se uma string é completamente numérica
#include "ppAlpha.cpp"						// "Incrementa" (plus plus) a string

#include "macros.cpp"						// Define constantes do programa

using namespace std;

void zerar(string);
int busca(string, string);
void insere(string, string, string, string, string, string, string, string, string, string);
int listagem(string, bool = true);
void removeReg(string, string, bool = true);

void zerarIndice(string);
int buscaIndice(string, string);
int primeiroIndice(string);
void compactarIndice(string);
void insereIndice(string, string, int);
bool removeIndice(string, string);

int main(int argc, char** argv) {
	
	FILE *lf = fopen(FILE_NAME, "a+b");					// Abre o arquivo, caso exista, ou cria um vazio
	
	if (lf == NULL) {						// Verifica a ocorrência de erro ao abrir o arquivo
		printf("Erro na abertura do arquivo: %s\n", strerror(errno));
		return 2;
	}
	rewind(lf);											// Move o ponteiro de leitura/escrita para o começo do arquivo

	char buffer[TAM_BLOCK];								// Vetor para guardar os blocos lidos
	string cabecalho;									// Variável utilizada para escrever o texto do cabeçalho

	int opcao = 0;
	do {												// Loop para mostrar o menu
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
		printf("\n");

		switch (opcao) {
			case -1:									// SAIR
				break;
			case 0:										// ZERAR
				{
					pausa("zerar o arquivo");			// Confirma a ação com o usuário

					zerar(FILE_NAME);
					zerarIndice(INDICE_FILE_NAME);
				}
				break;

			case 1:										// INSERIR
				{										// '{}' necessário para criar um escopo e poder declarar variáveis nele

					string chave = "";
					// Pede a chave do registro enquanto a inserida não tiver um comprimento de 3 caracteres
					while (strlen(chave.c_str()) != TAM_CHAVE || !strIsAlpha(chave)) {
						cout << "Insira uma chave alfabetica para o registro (XXX): ";
						cin >> chave;
					}

					string numero_ddd = "", numero_prefixo = "", numero_sufixo = "";
					// Pede o numero (DDD, prefixo e sufixo) enquanto não tiverem o comprimento 2, 5 e 4, respectivamente, de caracteres
					while (
						strlen(numero_ddd.c_str()) != TAM_NUMERO_DDD || !strIsInteger(numero_ddd) ||
						strlen(numero_prefixo.c_str()) != TAM_NUMERO_PREFIXO || !strIsInteger(numero_prefixo) ||
						strlen(numero_sufixo.c_str()) != TAM_NUMERO_SUFIXO || !strIsInteger(numero_sufixo)
					) {
						cout << "Insira o numero de telefone (XX XXXXX XXXX): ";
						cin >> numero_ddd >> numero_prefixo >> numero_sufixo;
					}

					string data_dia = "", data_mes = "", data_ano = "";
					// Pede a dara (dia, mês e ano) enquanto nao tiverem o comprimento 2, 2, e 4, respectivamente, de caracteres
					while (
						strlen(data_dia.c_str()) != TAM_DATA_DIA || !strIsInteger(data_dia) ||
						strlen(data_mes.c_str()) != TAM_DATA_MES || !strIsInteger(data_mes) ||
						strlen(data_ano.c_str()) != TAM_DATA_ANO || !strIsInteger(data_ano)
					) {
						cout << "Insira a data de nascimento (XX XX XXXX): ";
						cin >> data_dia >> data_mes >> data_ano;
					}

					string email = "";
					// Pede o email novamente caso o comprimento exceda 30 caracteres
					do {
						cout << "Insira o e-mail: ";
						cin >> email;
					} while (strlen(email.c_str()) > TAM_EMAIL);
					email.append(TAM_EMAIL - strlen(email.c_str()), ' ');	// Preenche o restante do campo com espaços

					string nome = "";
					// Pede o nome novamente caso o comprimento exceda 48 caracteres
					do {
						cout << "Insira o nome: ";
						getline(cin >> ws, nome);
					} while (strlen(nome.c_str()) > TAM_NOME);
					nome.append(TAM_NOME - strlen(nome.c_str()), ' ');		// Preenche o restante do campo com espaços

					// Exibe ao usuário os dados inserios para que ele possa confirmar todos
					cout << "\nDados reconhecidos:" << endl;
					cout << "\tChave:\t\t\t" << chave << endl;
					cout << "\tTelefone:\t\t(" << numero_ddd << ") " << numero_prefixo << "-" << numero_sufixo << endl;
					cout << "\tData de nascimento:\t" << data_dia << "/" << data_mes << "/" << data_ano << endl;
					cout << "\tE-mail:\t\t\t" << email << endl;
					cout << "\tNome:\t\t\t" << nome << endl << endl;

					pausa("gravar o registro");								// Confirma a ação com o usuário

					// Grava os dados
					insere(FILE_NAME, chave, numero_ddd, numero_prefixo, numero_prefixo, data_dia, data_mes, data_ano, email, nome);
					compactarIndice(INDICE_FILE_NAME);

				}
				break;

			case 2:										// BUSCAR
				{
					string chave = "";
					// Pede a chave a ser buscada enquanto a inserida não tiver um comprimento de 3 caracteres
					do {
						cout << "Insira uma chave alfabetica para ser buscada (XXX): ";
						cin >> chave;
					} while (strlen(chave.c_str()) != 3 || !strIsAlpha(chave));

					int pos = buscaIndice(INDICE_FILE_NAME, chave);						// Encontra o RRN (posição física do registro no arquivo) da chave
					bool achou = pos != -1;

					printf("\n");

					if (!achou) {
						printf("Nao foi encontrado nenhum registro com esta chave: %s\n", chave.c_str());
					} else {
						int buffer_index = RRN2NBLOCK(pos);					// Calcula o número do bloco que o registro está

						fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
						fread(buffer, sizeof(char), sizeof(buffer), lf);	// Lê o bloco que o registro está

						string found_chave = "",
							found_numero_ddd = "",
							found_numero_prefixo = "",
							found_numero_sufixo = "",
							found_data_dia = "",
							found_data_mes = "",
							found_data_ano = "",
							found_email = "",
							found_nome = "";

						int j = RRN2REGINBLOCK(pos);						// Calcula a posição do registro em relação ao bloco

						// Copia os campos "na unha" pois estava havendo problemas usando sscanf com tantas variáveis
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

						printf("A chave foi encontrada! Aqui estao os dados:\n\n" \
								"\tChave:\t\t\t%s\n" \
								"\tTelefone:\t\t(%s) %s-%s\n" \
								"\tData de nascimento:\t%s/%s/%s\n" \
								"\tE-mail:\t\t\t%s\n" \
								"\tNome:\t\t\t%s\n", found_chave.c_str(), found_numero_ddd.c_str(), found_numero_prefixo.c_str(), found_numero_sufixo.c_str(), found_data_dia.c_str(), found_data_mes.c_str(), found_data_ano.c_str(), found_email.c_str(), found_nome.c_str());
					}

					printf("\n");

					pausa();
				}
				break;

			case 3:										// EXCLUIR
				{
					string chave = "";
					// Pede a chave a ser removida enquanto a inserida não tiver um comprimento de 3 caracteres
					do {
						cout << "Insira uma chave alfabetica para ser excluida (XXX): ";
						cin >> chave;
					} while (strlen(chave.c_str()) != 3 || !strIsAlpha(chave));

					removeReg(FILE_NAME, chave);
					removeIndice(INDICE_FILE_NAME, chave);

					pausa();
				}
				break;

			case 4:										// LISTAGEM
				{
					lf = freopen(FILE_NAME, "r+b", lf);
					fseek(lf, 0, SEEK_SET);
					fread(buffer, sizeof(char), sizeof(buffer), lf);

					int n_registros = 0,
						n_excluidos = 0;
					sscanf(buffer, "N reg: %d\tN exl: %d", &n_registros, &n_excluidos); // Lê do bloco o total de registros atual

					if (n_registros > 0) {
						listagem(FILE_NAME);
					} else {
						printf("Nao ha nenhum registro no arquivo.");
						if (n_excluidos > 0)
							printf(" Existe(m) %d registro(s) logicamente removido(s).", n_excluidos);
						printf("\n");
					}

					printf("\n");

					pausa();
				}
				break;

			case 5:										// COMPACTAR
				{
					lf = freopen(FILE_NAME, "r+b", lf);
					fseek(lf, 0, SEEK_SET);
					fread(buffer, sizeof(char), sizeof(buffer), lf);

					int n_registros = 0,
						n_excluidos = 0;
					sscanf(buffer, "N reg: %d\tN exl: %d", &n_registros, &n_excluidos); // Lê do bloco o total de registros atual

					if (n_excluidos > 0) {
						zerar("_new_dados.bin");												// Cria um novo arquivo

						int rrn_first_reg = listagem(FILE_NAME, false);					// Pega a posição do primeiro registro

						while (rrn_first_reg != -1) {									// Varre por todos os registros disponíveis
							fseek(lf, RRN2NBLOCK(rrn_first_reg) * TAM_BLOCK, SEEK_SET);	// Move o ponteiro de leitura e escrita até o bloco do registro
							fread(buffer, sizeof(char), sizeof(buffer), lf);			// Lê o bloco todo

							string found_chave = "",
								found_numero_ddd = "",
								found_numero_prefixo = "",
								found_numero_sufixo = "",
								found_data_dia = "",
								found_data_mes = "",
								found_data_ano = "",
								found_email = "",
								found_nome = "";

							int j = RRN2REGINBLOCK(rrn_first_reg);						// Calcula a posição do registro em relação ao bloco

							// Copia os campos "na unha" pois estava havendo problemas usando sscanf com tantas variáveis
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

							// Insere no novo arquivo o registro encontrado
							insere("_new_dados.bin", found_chave, found_numero_ddd, found_numero_prefixo, found_numero_sufixo, found_data_dia, found_data_mes, found_data_ano, found_email, found_nome);

							removeReg(FILE_NAME, found_chave, false);						// Remove do arquivo antigo o registro encontrado

							rrn_first_reg = listagem(FILE_NAME, false);					// Atualiza para o próximo registro disponível
						}

						rename("_new_dados.bin", FILE_NAME);									// Renomeia o novo arquivo para sobrescrever o arquivo de dados
						printf("Arquivo temporario _new_dados.bin renomeado para %s.\n", FILE_NAME);

						printf("Compactacao dos dados no arquivo %s realizada com sucesso!\n", FILE_NAME);
					}
				}
				pausa();
				break;

			case 99: // INSERIR EM LOTE
				{
					int count;
					printf("Insira o numero de registros a serem inseridos: ");
					scanf("%d", &count);

					// Registro inicial
					string string_chave = "aaa",
						string_numero_ddd = "00",
						string_numero_prefixo = "00000",
						string_numero_sufixo = "0000",
						string_data_dia = "00",
						string_data_mes = "00",
						string_data_ano = "0000",
						string_email = "aaaaa",
						string_nome = "Aaaa";

					for (int i = 0; i < count; i++) {
						// Insere o registro
						insere(FILE_NAME, string_chave, string_numero_ddd, string_numero_prefixo, string_numero_sufixo, string_data_dia, string_data_mes, string_data_ano, string_email, string_nome);

						// "Incrementa" cada string do registro
						string_chave = ppAlpha(string_chave);
						string_numero_ddd = ppAlpha(string_numero_ddd);
						string_numero_prefixo = ppAlpha(string_numero_prefixo);
						string_numero_sufixo = ppAlpha(string_numero_sufixo);
						string_data_dia = ppAlpha(string_data_dia);
						string_data_mes = ppAlpha(string_data_mes);
						string_data_ano = ppAlpha(string_data_ano);
						string_email = ppAlpha(string_email);
						string_nome = ppAlpha(string_nome);
					}
					compactarIndice(INDICE_FILE_NAME);

					printf("\nRegistros inseridos.\n\n");

					pausa();
				}
				break;

			default:
				printf("Opcao invalida.\n");

				pausa();

				break;
		}

	} while (opcao != -1);

	fclose(lf);
	
	return 0;
}

void zerar(string path) {
	FILE *lf = fopen(path.c_str(), "w+b");								// Abre o arquivo em w+ para zerá-lo
					
	if(lf == NULL) {													// Verifica erros
		printf("Erro na criaçao do arquivo.\n");
		return;
	}
	
	char buffer[TAM_BLOCK];												// Vetor para guardar o bloco

	// Zera os contadores de registros
	int n_registros = 0;
	int n_excluidos = 0;
	
	// Preenche o bloco com espaços
	for (int i = 0; i < TAM_BLOCK; i++)
		buffer[i] = ' ';

	// Inicia o cabeçalho
	string cabecalho = "N reg: ";
	cabecalho.append(to_string(n_registros));
	cabecalho.append("\tN exl: ");
	cabecalho.append(to_string(n_excluidos));
	cabecalho.append(TAM_CABECALHO - strlen(cabecalho.c_str()), ' ');	// Preenche a string com espaço até dar o tamanho do cabeçalho

	// Prevenção de colocar o '\0' no meio do buffer
	char first_char = buffer[TAM_CABECALHO];
	sprintf(buffer, "%s", cabecalho.c_str());							// Passa o cabeçalho para o bloco
	buffer[TAM_CABECALHO] = first_char;

	fwrite(buffer, sizeof(char), sizeof(buffer), lf);					// Imprime o bloco

	fclose(lf);
}
int busca(string path, string chave) {
	int pos = -1;														// Variável que guarda a posição do registro

	FILE *lf = fopen(path.c_str(), "r+b");
	
	if(lf == NULL) {													// Verifica erros
		printf("Erro na abertura do arquivo.\n");
		return 2;
	}

	char buffer[TAM_BLOCK];

	fread(buffer, sizeof(char), sizeof(buffer), lf);

	int n_registros = 0,
		n_excluidos = 0;
	sscanf(buffer, "N reg: %d\tN exl: %d", &n_registros, &n_excluidos);	// Lê do bloco o total de registros atual

	int buffer_index = -1;												// Número do bloco a ser lido
	bool achou = false;
	char test_chave[3] = "";

	do {
		buffer_index++;
		//fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
		//fread(buffer, sizeof(char), sizeof(buffer), lf);

		int tot_registros = n_registros + n_excluidos;

		// Cálculo do número de registros que há no bloco atual
		int max_registros;
		if (buffer_index == 0) {										// Está no primeiro bloco
			max_registros = tot_registros > 4 ? 4 : tot_registros;
		} else if (4 + buffer_index * 5 <= tot_registros) {				// Está noutro bloco, completo
			max_registros = 5;
		} else {														// Está noutro bloco, incompleto
			max_registros = (tot_registros - 4) % 5;
		}

		// Deslocamento a ser realizado caso haja cabeçalho no bloco
		int seek_buffer = 0;
		if (buffer_index == 0) {
			seek_buffer = TAM_CABECALHO;
		}

		// Varre o bloco lendo todos os registos
		for (int i = 0, registro_index = 0; i < max_registros; i++, registro_index++) {

			// REABRE A CADA REGISTRO POR QUE POR ALGUM MOTIVO A FUNÇÃO SSCANF "ZERA" O BUFFER
			fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
			fread(buffer, sizeof(char), sizeof(buffer), lf);

			// Formação do padrão usado para obter a chave a partir do bloco. Ex: %*100[^\n]%[^\n] para o segundo registro de um bloco sem cabeçalho
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

			sscanf(buffer, pattern.c_str(), test_chave);				// Lê o a chave no bloco

			// Comparação da chave atual com a chave desejada
			if (!strcmp(test_chave, chave.c_str())) {
				pos = buffer_index * TAM_BLOCK + seek_buffer + registro_index * TAM_REGISTRO;	// Cálculo do RRN do registro

				achou = true;
				break;
			}
		}

	} while (!achou && buffer_index < (n_registros + n_excluidos) / 5);	// Lê outro bloco enquanto não tiver achado ou haverem blocos para serem lidos

	fclose(lf);

	return pos;
}
void insere(string path, string chave, string numero_ddd, string numero_prefixo, string numero_sufixo, string data_dia, string data_mes, string data_ano, string email, string nome) {

	char buffer[TAM_BLOCK];

	FILE *lf = fopen(path.c_str(), "r+b");
	fread(buffer, sizeof(char), sizeof(buffer), lf);
	
	if(lf == NULL) {													// Verifica erros
		printf("Erro na abertura do arquivo.\n");
		return;
	}

	int n_registros = 0,
		n_excluidos = 0;

	sscanf(buffer, "N reg: %d\tN exl: %d", &n_registros, &n_excluidos);	// Lê do bloco o total de registros atual

	int buffer_index = 0;												// Número do bloco para inserir o registro
	int pos = -1;														// Posição que deve ser inserido
	// Verifica se existe registro excluído
	if (n_excluidos == 0) {
		buffer_index = (n_registros < 4) ? 0 : (n_registros + 1) / 5;
	} else {
		pos = busca(path.c_str(), "***");
		buffer_index = RRN2NBLOCK(pos);
	}

	fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
	int total_read = fread(buffer, sizeof(char), sizeof(buffer), lf);
	
	// Preenche o bloco com espaços para evitar lixo (quando EOF)
	for (unsigned int i = total_read; i < sizeof(buffer); i++)
		buffer[i] = ' ';

	int reg_pos;														// Posição do registro em relação ao bloco
	if (n_excluidos == 0) {
		if (n_registros < 4) {
			reg_pos = TAM_CABECALHO + n_registros * TAM_REGISTRO;		// Fim do primeiro bloco
		} else {
			reg_pos = ((n_registros + 1) % 5) * TAM_REGISTRO;			// Fim do último bloco
		}
	} else {
		reg_pos = RRN2REGINBLOCK(pos);
	}

	insereIndice(INDICE_FILE_NAME, chave, reg_pos);

	// Preenche o campo o email e o nome para evitar lixo (e '\0')
	while (email.length() < TAM_EMAIL)
		email.append(" ");
	while (nome.length() < TAM_NOME)
		nome.append(" ");

	// Prevenção de colocar o '\0' no meio do buffer
	char first_char = buffer[(reg_pos + TAM_REGISTRO)];

	// Cópia de cada campo no bloco
	// Separado em diversos (sorry) snprintf por ser várias variáveis (começa a dar bug quando tem muitas)
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

	// Atualiza os contadores de registros
	n_registros++;
	if (n_excluidos > 0) {
		n_excluidos--;
	}

	// Define o cabeçalho a ser atalizado
	string cabecalho = "N reg: ";
	cabecalho.append(to_string(n_registros));
	cabecalho.append("\tN exl: ");
	cabecalho.append(to_string(n_excluidos));
	cabecalho.append(TAM_CABECALHO - strlen(cabecalho.c_str()), ' ');

	// Passa o cabeçalho para o bloco caso o número dele seja 0
	if (buffer_index == 0) {
		char first_char = buffer[TAM_CABECALHO];
		snprintf(buffer, TAM_CABECALHO + 1, "%s", cabecalho.c_str());
		buffer[TAM_CABECALHO] = first_char;
	}

	fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
	fwrite(buffer, sizeof(char), sizeof(buffer), lf);

	fflush(lf);												// Força a atualização do arquivo

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
	int rrn_first_reg = -1;												// Registra a posição do primeiro registro

	FILE *lf = fopen(path.c_str(), "r+b");
	
	if(lf == NULL) {													// Verifica erros
		printf("Erro na abertura do arquivo.\n");
		return -1;
	}

	char buffer[TAM_BLOCK];

	fread(buffer, sizeof(char), sizeof(buffer), lf);

	int n_registros = 0,
		n_excluidos = 0;
	sscanf(buffer, "N reg: %d\tN exl: %d", &n_registros, &n_excluidos); // Lê do bloco o total de registros atual

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

	// Como outras funções utilizam a listagem para obter a posição do primeiro registro, há um parâmetro para dizer se haverá log de informações ou se só quer obter o valor de retorno
	if (print) {
		printf("Nro");
		for (int i = 0; (unsigned int) i < (to_string(n_registros).length() > 3 ? to_string(n_registros).length() - 3 : 0); i++)
			printf(" ");
		printf("\tChave");
		for (int i = 0; i < TAM_CHAVE - 5; i++)
			printf(" ");
		printf("\tNumero");
		for (int i = 0; i < (TAM_NUMERO + 4) - 6; i++)
			printf(" ");
		printf("\tData");
		for (int i = 0; i < (TAM_DATA + 2) - 4; i++)
			printf(" ");
		printf("\tE-mail");
		for (int i = 0; i < TAM_EMAIL - 6; i++)
			printf(" ");
		printf("\tNome");
		for (int i = 0; i < TAM_NOME - 4; i++)
			printf(" ");
		printf("\n\n");
	}

	unsigned int count = 1;
	do {
		buffer_index++;

		// Deslocamento do cabeçalho caso esteja no primeiro bloco
		int seek_buffer = 0;
		if (buffer_index == 0) {
			seek_buffer = TAM_CABECALHO;
		}

		int tot_registros = n_registros + n_excluidos;
		int max_registros;												// Número de registros no bloco
		if (buffer_index == 0) {										// Está no primeiro bloco
			max_registros = tot_registros > 4 ? 4 : tot_registros;
		} else if (4 + buffer_index * 5 <= tot_registros) {				// Está noutro bloco, completo
			max_registros = 5;
		} else {														// Está noutro bloco, incompleto
			max_registros = (tot_registros - 4) % 5;
		}

		for (int i = 0, registro_index = 0; i < max_registros; i++, registro_index++) {

			// ABRE O ARQUIVO TODA VEZ POR QUE POR ALGUM MOTIVO A FUNÇÃO SSCANF "ZERA" O BUFFER
			fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
			fread(buffer, sizeof(char), sizeof(buffer), lf);

			// Posição do registro no bloco
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

			int j = RRN2REGINBLOCK(pos);						// Calcula a posição do registro em relação ao bloco

			// Copia os campos "na unha" pois estava havendo problemas usando sscanf com tantas variáveis
			for (int k = 0; k < TAM_CHAVE; j++, k++) {
				found_chave.append(to_string(buffer[j]));
			}

			if (found_chave != "***") {					// Verificação se o registro não está excluído

				// Continua a copiar os demais campos
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


				// Atualiza a posição do registro no arquivo caso ainda não tenha sido atualizado
				if (rrn_first_reg == -1) {
					rrn_first_reg = pos;
				}

				if (print) {

					// Imprime os dados em forma de tabela
					printf("%u", count);
					for (int j = 0; (unsigned int) j < to_string(n_registros).length() - to_string(count).length(); j++)
						printf(" ");
					printf("\t");
					count++;

					printf("%s\t", found_chave.c_str());
					printf("(%s) %s-%s\t", found_numero_ddd.c_str(), found_numero_prefixo.c_str(), found_numero_sufixo.c_str());
					printf("%s/%s/%s\t", found_data_dia.c_str(), found_data_mes.c_str(), found_data_ano.c_str());
					printf("%s\t", found_email.c_str());
					printf("%s\t", found_nome.c_str());
					printf("\n");
				}
			}

		}

	} while (buffer_index < (n_registros + n_excluidos) / 5);	// Enquanto não tiver terminado de ler todos os blocos

	fclose(lf);

	return rrn_first_reg;
}
void removeReg(string path, string chave, bool print) {
	
	FILE *lf = fopen(path.c_str(), "r+b");
	
	if(lf == NULL) {													// Verifica erros
		printf("Erro na abertura do arquivo.\n");
		return;
	}

	int pos = buscaIndice(INDICE_FILE_NAME, chave);
	bool achou = pos != -1;

	if (print)
		printf("\n");;

	if (!achou && print) {
			printf("Nao foi encontrado nenhum registro com esta chave.\n");
	} else {
		int buffer_index = RRN2NBLOCK(pos);
		
		char buffer[TAM_BLOCK];
		fseek(lf, 0, SEEK_SET);
		fread(buffer, sizeof(char), sizeof(buffer), lf);

		int n_registros = 0,
			n_excluidos = 0;
		sscanf(buffer, "N reg: %d\tN exl: %d", &n_registros, &n_excluidos); // Lê do bloco o total de registros atual

		// Atualiza os contadores
		n_registros--;
		n_excluidos++;

		// Move para o bloco do registro a ser inserido
		fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
		fread(buffer, sizeof(char), sizeof(buffer), lf);

		// Reescreve o cabeçalho
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

		// Insere "***" no lugar da chave
		int j = pos - buffer_index * TAM_BLOCK;
		for (int k = 0; k < TAM_CHAVE; j++, k++) {
			buffer[j] = '*';
		}
		
		// Grava os dados com o registro já excluído
		lf = freopen(path.c_str(), "r+b", lf);
		fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
		fwrite(buffer, sizeof(char), sizeof(buffer), lf);

		fflush(lf);										// Força o sistema atualizar o arquivo

		// Grava o cabeçalho caso não esteja removendo no primeiro bloco
		if (buffer_index != 0) {
			fseek(lf, 0, SEEK_SET);
			fread(buffer, sizeof(char), sizeof(buffer), lf);

			char first_char = buffer[TAM_CABECALHO];
			snprintf(buffer, TAM_CABECALHO + 1, "%s", cabecalho.c_str());
			buffer[TAM_CABECALHO] = first_char;
			
			fseek(lf, 0, SEEK_SET);
			fwrite(buffer, sizeof(char), sizeof(buffer), lf);
			
			fflush(lf);									// Força o sistema atualizar o arquivo
		}

		if (print)
			printf("O registro foi excluido com sucesso.\n");
	}

	fclose(lf);
}

void zerarIndice(string path) {
	FILE *lf = fopen(path.c_str(), "w+b");								// Abre o arquivo em w+ para zerá-lo
					
	if(lf == NULL) {													// Verifica erros
		printf("Erro na criaçao do arquivo de indice.\n");
		return;
	}
	
	char buffer[TAM_BLOCK];												// Vetor para guardar o bloco

	// Zera os contadores de registros
	int n_indice = 0,
		n_excluidos = 0;
	
	// Preenche o bloco com espaços
	for (int i = 0; i < TAM_BLOCK; i++)
		buffer[i] = ' ';

	// Inicia o cabeçalho
	string cabecalho = "N indice: ";
	cabecalho.append(to_string(n_indice));
	cabecalho.append("\tN exl: ");
	cabecalho.append(to_string(n_excluidos));
	cabecalho.append(TAM_CABECALHO - strlen(cabecalho.c_str()), ' ');	// Preenche a string com espaço até dar o tamanho do cabeçalho

	// Prevenção de colocar o '\0' no meio do buffer
	char first_char = buffer[TAM_CABECALHO];
	sprintf(buffer, "%s", cabecalho.c_str());							// Passa o cabeçalho para o bloco
	buffer[TAM_CABECALHO] = first_char;

	fwrite(buffer, sizeof(char), sizeof(buffer), lf);					// Imprime o bloco

	fclose(lf);
}
int buscaIndice(string file, string chave) {
	int pos = -1;														// Variável que guarda a posição do registro

	FILE *lf = fopen(file.c_str(), "r+b");
	
	if(lf == NULL) {													// Verifica erros
		printf("Erro na abertura do arquivo de indice.\n");
		return 2;
	}

	char buffer[TAM_BLOCK];

	fread(buffer, sizeof(char), sizeof(buffer), lf);

	int n_indice = 0,
		n_excluidos = 0;
	sscanf(buffer, "N indice: %d\tN exl: %d", &n_indice, &n_excluidos);	// Lê do bloco o total de registros atual

	int buffer_index = -1;												// Número do bloco a ser lido
	bool achou = false;
	char test_chave[3] = "",
		test_chave_temp[3] = "",
		test_rrn[8] = "";

	do {
		buffer_index++;

		int tot_indices = n_indice + n_excluidos;

		// Cálculo do número de registros que há no bloco atual
		int max_indices;
		if (buffer_index == 0) {										// Está no primeiro bloco
			max_indices = tot_indices > 4 ? 4 : tot_indices;
		} else if (4 + buffer_index * 5 <= tot_indices) {				// Está noutro bloco, completo
			max_indices = 5;
		} else {														// Está noutro bloco, incompleto
			max_indices = (tot_indices - 4) % 5;
		}

		// Deslocamento a ser realizado caso haja cabeçalho no bloco
		int seek_buffer = 0;
		if (buffer_index == 0) {
			seek_buffer = TAM_CABECALHO;
		}

		// Varre o bloco lendo todos os registos
		for (int i = 0, indice_index = 0; i < max_indices; i++, indice_index++) {

			// REABRE A CADA REGISTRO POR QUE POR ALGUM MOTIVO A FUNÇÃO SSCANF "ZERA" O BUFFER
			fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
			fread(buffer, sizeof(char), sizeof(buffer), lf);

			// Formação do padrão usado para obter a chave a partir do bloco. Ex: %*100[^\n]%[^\n] para o segundo registro de um bloco sem cabeçalho
			string pattern;
			if (seek_buffer + indice_index * (TAM_INDICE+0) > 0) {
				pattern = "%*";
				pattern.append(to_string(seek_buffer + indice_index * (TAM_INDICE+0) - 1));
				pattern.append("[^\n]%");
			} else {
				pattern = "%";
			}
			pattern.append("1[^\n]%3[^\n]%8[^\n]");

			sscanf(buffer, pattern.c_str(), test_chave_temp, test_chave, test_rrn);				// Lê o a chave no bloco

			// Comparação da chave atual com a chave desejada
			if (!strcmp(test_chave, chave.c_str())) {
				pos = buffer_index * TAM_BLOCK + seek_buffer + indice_index * (TAM_INDICE+0);

				achou = true;
				break;
			}
		}

	} while (!achou && buffer_index < (n_indice + n_excluidos) / 5);	// Lê outro bloco enquanto não tiver achado ou haverem blocos para serem lidos

	fclose(lf);

	return pos;
}
int primeiroIndice(string file) {
	int rrn_first_ind = -1;												// Registra a posição do primeiro registro

	FILE *lf = fopen(file.c_str(), "r+b");
	
	if(lf == NULL) {													// Verifica erros
		printf("Erro na abertura do arquivo de indice.\n");
		return -1;
	}

	char buffer[TAM_BLOCK];

	fread(buffer, sizeof(char), sizeof(buffer), lf);

	int n_indice = 0,
		n_excluidos = 0;
	sscanf(buffer, "N indice: %d\tN exl: %d", &n_indice, &n_excluidos); // Lê do bloco o total de registros atual

	int buffer_index = -1;
								
	string found_chave,
		found_reg_pos;

	do {
		buffer_index++;

		// Deslocamento do cabeçalho caso esteja no primeiro bloco
		int seek_buffer = 0;
		if (buffer_index == 0) {
			seek_buffer = TAM_CABECALHO;
		}

		int tot_indices = n_indice + n_excluidos;
		int max_indices;												// Número de registros no bloco
		if (buffer_index == 0) {										// Está no primeiro bloco
			max_indices = tot_indices > 4 ? 4 : tot_indices;
		} else if (4 + buffer_index * 5 <= tot_indices) {				// Está noutro bloco, completo
			max_indices = 5;
		} else {														// Está noutro bloco, incompleto
			max_indices = (tot_indices - 4) % 5;
		}

		for (int i = 0, indice_index = 0; i < max_indices; i++, indice_index++) {

			// ABRE O ARQUIVO TODA VEZ POR QUE POR ALGUM MOTIVO A FUNÇÃO SSCANF "ZERA" O BUFFER
			fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
			fread(buffer, sizeof(char), sizeof(buffer), lf);

			// Posição do registro no bloco
			int pos = buffer_index * TAM_BLOCK + seek_buffer + indice_index * (TAM_INDICE+0);

			found_chave = "",
				found_reg_pos = "";

			int j = RRN2REGINBLOCK(pos);						// Calcula a posição do registro em relação ao bloco

			// Copia os campos "na unha" pois estava havendo problemas usando sscanf com tantas variáveis
			for (int k = 0; k < TAM_CHAVE; j++, k++) {
				found_chave.append(to_string(buffer[j]));
			}

			if (found_chave != "***") {					// Verificação se o registro não está excluído
				// Atualiza a posição do registro no arquivo caso ainda não tenha sido atualizado
				if (rrn_first_ind == -1) {
					rrn_first_ind = pos;
					break;
				}
			}

		}

	} while (buffer_index < (n_indice + n_excluidos) / 5 && rrn_first_ind == -1);	// Enquanto não tiver terminado de ler todos os blocos

	fclose(lf);

	if (rrn_first_ind != -1)
		printf("O primeiro indice do arquivo %s tem a chave %s e esta em %d.\n", file.c_str(), found_chave.c_str(), rrn_first_ind);
	else
		printf("O arquivo de indices %s nao possui indices.\n", file.c_str());

	return rrn_first_ind;
}
void compactarIndice(string file) {
	FILE *lf = fopen(file.c_str(), "r+b");
	char buffer[TAM_BLOCK];

	fseek(lf, 0, SEEK_SET);
	fread(buffer, sizeof(char), sizeof(buffer), lf);

	int n_indice = 0,
		n_excluidos = 0;
	sscanf(buffer, "N indice: %d\tN exl: %d", &n_indice, &n_excluidos); // Lê do bloco o total de registros atual

	zerar("_new_indice.bin");												// Cria um novo arquivo
	printf("Arquivo temporario _new_indice.bin criado.\n");

	int rrn_first_ind = primeiroIndice(file);					// Pega a posição do primeiro registro

	while (rrn_first_ind != -1) {									// Varre por todos os registros disponíveis
		fseek(lf, RRN2NBLOCK(rrn_first_ind) * TAM_BLOCK, SEEK_SET);	// Move o ponteiro de leitura e escrita até o bloco do registro
		fread(buffer, sizeof(char), sizeof(buffer), lf);			// Lê o bloco todo

		string found_chave = "",
			found_reg_pos = "";

		int j = RRN2REGINBLOCK(rrn_first_ind);						// Calcula a posição do registro em relação ao bloco

		// Copia os campos "na unha" pois estava havendo problemas usando sscanf com tantas variáveis
		for (int k = 0; k < TAM_CHAVE; j++, k++) {
			found_chave.append(to_string(buffer[j]));
		}
		for (int k = 0; k < 8; j++, k++) {
			found_reg_pos.append(to_string(buffer[j]));
		}

		// Insere no novo arquivo o registro encontrado
		insereIndice("_new_indice.bin", found_chave, stoi(found_reg_pos));

		if (!removeIndice(file, found_chave)) {						// Remove do arquivo antigo o registro encontrado
			break;
		}

		rrn_first_ind = primeiroIndice(file);					// Atualiza para o próximo registro disponível
	}

	fclose(lf);

	if (remove(INDICE_FILE_NAME) != 0)
		printf("Erro ao excluir o arquivo de indices: %s\n", strerror(errno));
	pausa();

	if (rename("_new_indice.bin", INDICE_FILE_NAME) == 0)									// Renomeia o novo arquivo para sobrescrever o arquivo de dados
		printf("Arquivo temporario _new_indice.bin renomeado para %s.\n", INDICE_FILE_NAME);
	else
		printf("Arquivo temporario _new_indice.bin NAO PODE SER renomeado para %s: %s\n", INDICE_FILE_NAME, strerror(errno));

	printf("Compactacao dos indices no arquivo %s realizada com sucesso!\n", file.c_str());
}
void insereIndice(string file, string chave, int reg_pos) {

	char buffer[TAM_BLOCK];

	FILE *lf = fopen(file.c_str(), "r+b");
	fread(buffer, sizeof(char), sizeof(buffer), lf);
	
	if(lf == NULL) {													// Verifica erros
		printf("Erro na abertura do arquivo de indice.\n");
		return;
	}

	int n_indice = 0,
		n_excluidos = 0;

	sscanf(buffer, "N indice: %d\tN exl: %d", &n_indice, &n_excluidos);	// Lê do bloco o total de registros atual

	int buffer_index = 0;												// Número do bloco para inserir o registro
	int pos = -1;
	if (n_excluidos == 0)
		buffer_index = (n_indice < 4) ? 0 : (n_indice + 1) / 5;
	else {
		pos = busca(file.c_str(), "***");
		buffer_index = RRN2NBLOCK(pos);
	}

	fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
	int total_read = fread(buffer, sizeof(char), sizeof(buffer), lf);
	
	// Preenche o bloco com espaços para evitar lixo (quando EOF)
	for (unsigned int i = total_read; i < sizeof(buffer); i++)
		buffer[i] = ' ';

	int ind_pos = 0;														// Posição do registro em relação ao bloco
	if (n_excluidos == 0) {
		if (n_indice < 4) {
			ind_pos = TAM_CABECALHO + n_indice * (TAM_INDICE+1-1);		// Fim do primeiro bloco
		} else {
			ind_pos = ((n_indice + 1) % 5) * (TAM_INDICE+0);			// Fim do último bloco
		}
	} else
		ind_pos = RRN2REGINBLOCK(pos);


	// Prevenção de colocar o '\0' no meio do buffer
	char first_char = buffer[(ind_pos + (TAM_INDICE+0))];

	string str_reg_pos = to_string(reg_pos);
	while (str_reg_pos.length() < 8)
		str_reg_pos.append(" ");

	// Cópia de cada campo no bloco
	// Separado em diversos (sorry) snprintf por ser várias variáveis (começa a dar bug quando tem muitas)
	snprintf(
		buffer + ind_pos,
		TAM_CHAVE+1,
		"%s",
		chave.c_str()
	);
	snprintf(
		buffer + ind_pos + TAM_CHAVE,
		8+1,
		"%s",
		str_reg_pos.c_str()
	);
	
	buffer[(ind_pos + (TAM_INDICE+0))] = first_char;

	// Atualiza o contador de indice
	n_indice++;
	if (n_excluidos > 0)
		n_excluidos--;

	// Define o cabeçalho a ser atalizado
	string cabecalho = "N indice: ";
	cabecalho.append(to_string(n_indice));
	cabecalho.append("\tN exl: ");
	cabecalho.append(to_string(n_excluidos));
	cabecalho.append(TAM_CABECALHO - strlen(cabecalho.c_str()), ' ');

	// Passa o cabeçalho para o bloco caso o número dele seja 0
	if (buffer_index == 0) {
		char first_char = buffer[TAM_CABECALHO];
		snprintf(buffer, TAM_CABECALHO + 1, "%s", cabecalho.c_str());
		buffer[TAM_CABECALHO] = first_char;
	}

	fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
	fwrite(buffer, sizeof(char), sizeof(buffer), lf);

	fflush(lf);												// Força a atualização do arquivo

	// Reabre o arquivo e atualiza o cabeçalho
	if (buffer_index != 0) {
		fseek(lf, 0, SEEK_SET);
		fflush(lf);
		fread(buffer, sizeof(char), sizeof(buffer), lf);

		char first_char = buffer[TAM_CABECALHO];
		snprintf(buffer, TAM_CABECALHO + 1, "%s", cabecalho.c_str());
		buffer[TAM_CABECALHO] = first_char;
		
		fseek(lf, 0, SEEK_SET);
		fwrite(buffer, sizeof(char), sizeof(buffer), lf);
		
		fflush(lf);
	}

	printf("O indice da chave %s foi inserido com sucesso no arquivo %s.\n", chave.c_str(), file.c_str());

	fclose(lf);
}
bool removeIndice(string file, string chave) {
	
	FILE *lf = fopen(file.c_str(), "r+b");
	
	if(lf == NULL) {													// Verifica erros
		printf("Erro na abertura do arquivo de indice (%s).\n", file.c_str());
		return false;
	}

	int pos = buscaIndice(file, chave);
	bool achou = pos != -1;

	if (!achou) {
		printf("Nao foi encontrado nenhum indice com a chave %s no arquivo %s.\n", chave.c_str(), file.c_str());

		return false;
	} else {
		int buffer_index = RRN2NBLOCK(pos);
		
		char buffer[TAM_BLOCK];
		fseek(lf, 0, SEEK_SET);
		fread(buffer, sizeof(char), sizeof(buffer), lf);

		int n_indice = 0,
			n_excluidos = 0;
		sscanf(buffer, "N indice: %d\tN exl: %d", &n_indice, &n_excluidos); // Lê do bloco o total de registros atual

		// Atualiza os contadores
		n_indice--;
		n_excluidos++;

		// Move para o bloco do registro a ser inserido
		fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
		fread(buffer, sizeof(char), sizeof(buffer), lf);

		// Reescreve o cabeçalho
		string cabecalho = "N indice: ";
		cabecalho.append(to_string(n_indice));
		cabecalho.append("\tN exl: ");
		cabecalho.append(to_string(n_excluidos));
		cabecalho.append(TAM_CABECALHO - strlen(cabecalho.c_str()), ' ');

		if (buffer_index == 0) {
			char first_char = buffer[TAM_CABECALHO];
			snprintf(buffer, TAM_CABECALHO + 1, "%s", cabecalho.c_str());
			buffer[TAM_CABECALHO] = first_char;
		}

		// Insere "***" no lugar da chave
		int j = pos - buffer_index * TAM_BLOCK;
		for (int k = 0; k < TAM_CHAVE; j++, k++) {
			buffer[j] = '*';
		}
		
		// Grava os dados com o registro já excluído
		lf = freopen(file.c_str(), "r+b", lf);
		fseek(lf, buffer_index * TAM_BLOCK, SEEK_SET);
		fwrite(buffer, sizeof(char), sizeof(buffer), lf);

		fflush(lf);										// Força o sistema atualizar o arquivo

		// Grava o cabeçalho caso não esteja removendo no primeiro bloco
		if (buffer_index != 0) {
			fseek(lf, 0, SEEK_SET);
			fread(buffer, sizeof(char), sizeof(buffer), lf);

			char first_char = buffer[TAM_CABECALHO];
			snprintf(buffer, TAM_CABECALHO + 1, "%s", cabecalho.c_str());
			buffer[TAM_CABECALHO] = first_char;
			
			fseek(lf, 0, SEEK_SET);
			fwrite(buffer, sizeof(char), sizeof(buffer), lf);
			
			fflush(lf);									// Força o sistema atualizar o arquivo
		}

		printf("O indice da chave %s foi excluido com sucesso do arquivo %s. Restam %d indice(s).\n", chave.c_str(), file.c_str(), n_indice);
	}

	fclose(lf);

	return true;
}