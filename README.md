# Gerenciador-de-Registros
Programa desenvolvido como trabalho na disciplina de ORI (2017) no curso BCC da UFSCar

### Desenvolvedores
Alisson Nunes Vieira Amancio, RA: 725862
Caio Ferreira, RA: 726502
Gabriel Bortolote Pitarelli, RA: 726514

## Proposta do projeto
Implementar uma estrutura simples, mas que possua as principais características relativas ao acesso e organização de dados em memória secundária.

### Características da estrutura
1. Não possui ordenação em seus registros;
2. Os registros são de tamanho fixo (100 bytes);
3. Existe controle de blocos (cada bloco possui um número inteiro de registro);
4. Os blocos possuem 512 bytes.

### Características dos registros
Os registro possuem 5 campos para serem preenchidos (todos strings):
1. Chave (3 bytes)
2. Telefone (11 bytes)
3. Data de nascimento (8 bytes)
4. E-mail (30 bytes)
5. Nome (48 bytes)

### Características do cabeçalho
No primeiro bloco, o arquivo possui um cabeçalho de tamanho 50 bytes que mostra quantos arquivos existem no arquivo e quantos foram logicamente removidos.
Assim, o primeiro bloco possui o cabeçalho e 4 registros, enquanto todos os outros blocos possuem 5 registros cada.

### Funções utilizadas no programa
1. void zerar(string);
2. int busca(string, string);
3. void insere(string, string, string, string, string, string, string, string, string, string);
4. int listagem(string, bool = true);
5. void remove(string, string, bool = true);

### Opções do menu

#### Zerar o arquivo
O programa simplesmente chama a função zerar, passando o arquivo como parâmentro. 
Essa função zera os contadores de registros e escreve, por cima de arquivos anteriores, o primeiro bloco apenas com o cabeçalho.

#### Inserir registro
O programa solicita todas as informações de um registro para o usuário e chama a função insere.
Essa função reabre o arquivo e, baseado na quantidade de registro existentes e logicamente removidos, encontra o posição de inserção do novo registro (caja haja um registro logicamente removido, o novo registro é escrito em sua posição), e reescreve todo o bloco, inserindo as novas mudanças, além de atualizar o número de registros.

#### Buscar registro
O programa solicita a chave ao usuário e, utilizando a função busca, procura a chave no arquivo e, caso encontre, mostra todas as informações do registro.

#### Apagar registro
O programa solicita a chave ao usuário e, utilizando a função busca, procura a chave no arquivo e, caso encontre, substitui a chave desse registro por *** e altera os números de registros existentes e logicamente removidos (com ajuda da função remove).

#### Listar registros
A partir da função listagem, o programa percorre todo o arquivo, imprimindo na tela as informações de todos os registros existentes.
Caso não haja registros, mas existam registros logicamente removidos, o programa mostra a quantidade de registros logicamente removidos.

#### Compactar o arquivo
Há a possibilidade de existirem tantos registros logicamente removidos que acabando ocupando bloco sem necessidade.
Essa opção, então, permite que todos os registros logicamente removidos sejam realmente removidos do arquivo, economizando espaço.
Basicamente, com a ajuda das funções, remove, insere e listagem, o programa cria um novo arquivo, insere nele todos os registros existentes no arquivo atual, e renomeia o novo arquivo com o nome do arquivo antigo.

#### Inserir em lote
Essa opção solicita ao usuário o número de arquivos a serem inseridos e, então, insere com dados gerados automaticamente.