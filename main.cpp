#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "to_string.cpp"

using namespace std;

#ifdef _WIN32
#define CLEAR "cls"
#else //In any other OS
#define CLEAR "clear"
#endif

void pause(string msg = "continuar") {
	string command = "echo Pressione ENTER para ";
	command.append(msg);
	command.append(" . . . && ( read x 2> nul; rm nul || pause > nul )");

    system(command.c_str());
    return;
}

int main() {
	
	FILE *lf;
	lf = fopen("arquivo.bin", "a+b");
	
	if(lf == NULL) {
		printf("Erro na abertura do arquivo.\n");
		return 2;
	}

	string cabecalho;
	int n_registros;
	fscanf(lf, "Número de registros: %d", &n_registros);

	int comando = 0;
	do {
		system(CLEAR);

		printf("Menu\n" \
				"-1 - Sair\n" \
				" 1 - Zerar arquivo\n" \
				" 2 - Inserir\n\n");

		printf("Digite o comando: ");
		scanf("%d", &comando);

		switch (comando) {
			case 1:

				pause("zerar o arquivo");
				
				lf = freopen("arquivo.bin", "w+b", lf);
				
				if(lf == NULL) {
					printf("Erro na criacao do arquivo.\n");
					return 1;
				}

				n_registros = 0;
				cabecalho = "Número de registros: ";
				cabecalho.append(to_string(n_registros));
				cabecalho.append(50 - strlen(cabecalho.c_str()), ' ');
				fprintf(lf, "%s", cabecalho.c_str());
				break;
			case 2:
				lf = freopen("arquivo.bin", "a+b", lf);
				
				if(lf == NULL) {
					printf("Erro na criacao do arquivo.\n");
					return 2;
				}

				string chave = "";
				while (strlen(chave.c_str()) != 3) {
					cout << "Insira a chave para o registro (XXX): ";
					cin >> chave;
				}

				string numero_ddd = "", numero_prefixo = "", numero_sufixo = "";
				while (strlen(numero_ddd.c_str()) != 2 || strlen(numero_prefixo.c_str()) != 5 || strlen(numero_sufixo.c_str()) != 4) {
					cout << "Insira o número de telefone (XX XXXXX XXXX): ";
					cin >> numero_ddd >> numero_prefixo >> numero_sufixo;
				}

				string data_dia = "", data_mes = "", data_ano = "";
				while (strlen(data_dia.c_str()) != 2 || strlen(data_mes.c_str()) != 2 || strlen(data_ano.c_str()) != 4) {
					cout << "Insira a data de nascimento (XX XX XXXX): ";
					cin >> data_dia >> data_mes >> data_ano;
				}

				string email = "";
				do {
					cout << "Insira o e-mail: ";
					cin >> email;
				} while (strlen(email.c_str()) > 30);
				email.append(30 - strlen(email.c_str()), ' ');

				string nome = "";
				do {
					cout << "Insira o nome: ";
					getline(cin >> ws, nome);
				} while (strlen(nome.c_str()) > 48);
				nome.append(48 - strlen(nome.c_str()), ' ');

				cout << "\nDados reconhecidos:" << endl;
				cout << "Chave: " << chave << endl;
				cout << "Telefone: (" << numero_ddd << ") " << numero_prefixo << "-" << numero_sufixo << endl;
				cout << "Data de nascimento: " << data_dia << "/" << data_mes << "/" << data_ano << endl;
				cout << "E-mail: " << email << endl;
				cout << "Nome: " << nome << endl << endl;

				pause("gravar o registro");

				fprintf(lf, "%s", chave.c_str());
				fprintf(lf, "%s%s%s", numero_ddd.c_str(), numero_prefixo.c_str(), numero_sufixo.c_str());
				fprintf(lf, "%s%s%s", data_dia.c_str(), data_mes.c_str(), data_ano.c_str());
				fprintf(lf, "%s", email.c_str());
				fprintf(lf, "%s", nome.c_str());

				lf = freopen("arquivo.bin", "r+b", lf);
				fseek(lf, 0, SEEK_SET);
				cabecalho = "Número de registros: ";
				cabecalho.append(to_string(++n_registros));
				cabecalho.append(50 - strlen(cabecalho.c_str()), ' ');
				fprintf(lf, "%s", cabecalho.c_str());

				fflush(lf);

				break;
		}
		
		
		fflush(lf);

	} while (comando != -1);

	fclose(lf);
	
	return 0;
}
