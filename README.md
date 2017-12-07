# Gerenciador de Registros

Programa desenvolvido como trabalho na disciplina de ORI (2017.2) no curso BCC da UFSCar

## Desenvolvedores

- Alisson Nunes Vieira Amancio `RA: 725862`
- Caio Ferreira `RA: 726502`
- Gabriel Bortolote Pitarelli `RA: 726514`

## Proposta do projeto

Implementar uma estrutura simples, mas que possua as principais características relativas ao acesso e organização de dados em memória secundária com indexação.

## Opções do menu

### Zerar o arquivo

O programa simplesmente chama a função `zerar` e a função `zerarIndice`, passando o nome dos arquivos (de dados e de índices) como parâmentro.

Essas funções zeram os arquivos e escrevem os cabeçalhos zerados do arquivo de dados e do arquivo de índices, respectivamente.

### Inserir registro

O programa solicita ao usuário todas as [informações de um registro](#características-dos-registros) e chama a função `insere` passando o nome do arquivo e os dados capturados e a função `insereIndice` passando a chave do novo registro.

Essa função procura, baseando-se no arquivo de índices, o próximo espaço livre onde o registro (e o índice) serão inseridos, guiado pela quantidade de registro existentes e logicamente removidos, e reescreve todo o bloco com as novas mudanças, além de reescrever o bloco do cabeçalho atualizando o número de registros. Caso a inserção esteja ocorrendo no bloco do cabeçalho, é feita apenas uma reescrita com os dados atualizados. O arquivo de índices é, então, ordenado.

### Buscar registro

O programa solicita a chave de busca ao usuário e, utilizando a função `buscaIndice`, encontra a localização do índice no arquivo e, caso encontre, mostra todas as informações do mesmo.

Para obter esta localização, a função varre o arquivo de índice bloco a bloco e com o bloco em memória primária, procura a chave e, caso encontre, abre o arquivo de dados e lê o registro localizado no offset de seu respectivo índice.

### Apagar registro

O programa solicita a chave ao usuário e chama a função `remove`, passando o nome do arquivo e a chave.

Essa função utiliza a função `buscaIndice`, encontra a localização da chave no arquivo de índices e, caso encontre, abre o arquivo de dados, vai para a posição apontada pelo offset do índice, substitui a chave desse registro por `***` e reescreve todo o bloco novamente, além de reescrever o bloco do cabeçalho atualizando os números de registros existentes e logicamente removidos. Caso a remoção esteja ocorrendo no bloco do cabeçalho, é feita apenas uma reescrita com os dados atualizados. Então, o arquivo de índices é ordenado.

### Listar registros

O programa lê o cabeçalho e caso o número de registros seja maior do que zero, chama a função `listagem`. Caso não haja registros, mas existam registros logicamente removidos, o programa mostra a quantidade de registros logicamente removidos.

Esta função percorre todo o arquivo, imprimindo na tela as informações de todos os registros existentes que **não estão logicamente removidos**.

### Compactar o arquivo

Há a possibilidade de existirem tantos registros logicamente removidos que acaba ocupando blocos inteiros sem necessidade e fazendo o arquivo ocupar um espaço que não utiliza, além de que possibilita uma lentidão nos demais algoritmos (como o de listagem). Essa opção, então, permite que todos os registros logicamente removidos sejam realmente removidos do arquivo, economizando espaço.

Basicamente, com a ajuda das funções, `listagem`, `insere` e `remove`, o programa cria um novo arquivo e realiza os seguintes passos até acabar os registros no arquivo de dados:

1. busca no arquivo de dados a localização do primeiro registro que não está logicamente removido;
2. insere no novo arquivo todos os campos do registro encontrado;
3. remove do arquivo de dados o registro encontrado.

Após o fim do processo, renomeia o novo arquivo com o nome do arquivo de dados.

### Inserir em lote

Essa opção pergunta ao usuário se ele deseja obter os dados de um arquivo o .csv ou que o programa gere eles sequencialmente. Selecionado a opção de arquivo, o usuário deve inserir o caminho do arquivo que contém os dados compatíveis com o programa (campos delimitados por espaço em branco). Ao selecionado a opção de gerar sequencialmente, o programa solicita o número de registros a serem inseridos e, então, chama a função `insere` com dados gerados automaticamente.

## O arquivo de dados

### Características da estrutura

- Não possui ordenação entre os registros;
- Os registros são de tamanho fixo (100 bytes);
- Existe controle de blocos (cada bloco possui um número inteiro de registros);
- Os blocos possuem 512 bytes.

### Características dos registros

Os registros possuem 5 campos de tamanhos fixos para serem preenchidos (todos strings):
1. Chave (3 bytes)
2. Telefone (11 bytes)
3. Data de nascimento (8 bytes)
4. E-mail (30 bytes)
5. Nome (48 bytes)

### Características do cabeçalho

No primeiro bloco, o arquivo possui um cabeçalho de tamanho 50 bytes que mostra quantos registros existem no arquivo e quantos foram logicamente removidos. Assim, o primeiro bloco possui o cabeçalho e 4 registros, enquanto todos os outros blocos possuem 5 registros cada.

## O arquivo de índices

### Características da estrutura

-Sempre está em ordem alfabética (das chaves);
-Os registros são de tamanho fixo (11 bytes);
-Existe controle de blocos (cada bloco possui um número inteiro de índices);
-Os blocos possuem 512 bytes.

### Características dos registros

Os registros possuem 2 campos de tamanhos fixos para serem preenchidos (todos strings);
1. Chave (3 bytes)
2. Offset (8 bytes)

### Características do cabeçalho

No primeiro bloco, o arquivo possui um cabeçalho de tamanho 50 bytes que mostra quantos índices existem no arquivo e quantos foram logicamente removidos. Assim, o primeiro bloco possui o cabeçalho e 42 índices, enquanto todos os outros blocos possuem 46 índices cada.

## Funções utilizadas no programa

- `void zerar(std::string path);`
  - `path` define em qual arquivo será feita as operações.
- `int busca(std::string path, std::string chave);`
  - `path` define em qual arquivo será feita as operações;
  - `chave` define a chave que será buscada;
  - O valor retornado representa a posição do registro encontrado no arquivo (-1 representa que o registro não foi encontrado).
- `void insere(std::string path, std::string chave, std::string numero_ddd, std::string numero_prefixo, std::string numero_sufixo, std::string data_dia, std::string data_mes, std::string data_ano, std::string email, std::string nome);`
  - `path` define em qual arquivo será feita as operações;
  - `chave`, `numero_ddd`, `numero_prefixo`, `numero_sufixo`, `data_dia`, `data_mes`, `data_ano`, `email` e `nome` são os campos do registro a ser inserido.
- `int listagem(std::string path, bool print = true);`
  - `path` define em qual arquivo será feita as operações;
  - `print` define se as informações serão exibidas no terminal;
  - O valor retornado representa a posição do primeiro registro válido no arquivo (-1 representa que o registro não foi encontrado).
- `void removeReg(std::string path, std::string chave, bool print = true);`
  - `path` define em qual arquivo será feita as operações;
  - `chave` define a chave que será buscada e removida;
  - `print` define se as informações serão exibidas no terminal;

---

- `void zerarIndice(string path);`
  - `path` define em qual arquivo será feita as operações;
- `int buscaIndice(string file, string chave);`
  - `file` define em qual arquivo será feita as operações;
  - `chave` define a chave que será buscada;
  - O valor retornado representa a posição do índice encontrado no arquivo (-1 representa que o índice não foi encontrado).
- `int primeiroIndice(string file);`
  - `file` define em qual arquivo será feita as operações;
  - O valor retornado representa a posição do primeiro índice válido no arquivo (-1 representa que o índice não foi encontrado).
- `int chaveMenor(string file, string chave);`
  - `file` define em qual arquivo será feita as operações;
  - `chave` define a chave que será buscada;
  - O valor retornado representa a posição do índice que é alfabeticamente menor do que o índice buscado (-1 representa que não há um índice menor).
- `void compactarIndice(string file);`
  - `file` define em qual arquivo será feita as operações;
- `void insereIndice(string file, string chave, int reg_pos);`
  - `file` define em qual arquivo será feita as operações;
  - `chave` define a chave do registro a ser inserido no índice;
  - `reg_pos` define a posição do registro a qual o índice se refere.
- `bool removeIndice(string, string);`
  - `file` define em qual arquivo será feita as operações;
  - `chave` define a chave do registro a ser excluído do índice;
