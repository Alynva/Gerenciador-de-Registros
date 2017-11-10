# Gerenciador de Registros

Programa desenvolvido como trabalho na disciplina de ORI (2017.2) no curso BCC da UFSCar

## Desenvolvedores

- Alisson Nunes Vieira Amancio `RA: 725862`
- Caio Ferreira `RA: 726502`
- Gabriel Bortolote Pitarelli `RA: 726514`

## Proposta do projeto

Implementar uma estrutura simples, mas que possua as principais características relativas ao acesso e organização de dados em memória secundária.

## Opções do menu

### Zerar o arquivo

O programa simplesmente chama a função `zerar`, passando o nome do arquivo como parâmentro.

Essa função por sua vez zera o arquivo e escreve o cabeçalho zerado.

### Inserir registro

O programa solicita ao usuário todas as [informações de um registro](#caracteristicas-dos-registros) e chama a função `insere` passando o nome do arquivo e os dados capturados.

Essa função reabre o arquivo e, baseado na quantidade de registro existentes e logicamente removidos, encontra o posição de inserção do novo registro (caso haja um registro logicamente removido, o novo registro é escrito em sua posição), e reescreve todo o bloco com as novas mudanças, além de reescrever o bloco do cabeçalho atualizando o número de registros. Caso a inserção esteja ocorrendo no bloco do cabeçalho, é feita apenas uma reescrita com os dados atualizados.

### Buscar registro

O programa solicita a chave de busca ao usuário e, utilizando a função `busca`, encontra a localização do registro no arquivo e, caso encontre, mostra todas as informações do mesmo.

Para obter esta localização, a função varre o arquivo bloco a bloco e com o bloco em memória primária lê a chave de cada registro, realizando todos os cálculos de pular bytes tanto do cabeçalho quanto o do final do bloco, parando na primeira ocorrência.

### Apagar registro

O programa solicita a chave ao usuário e chama a função `remove`, passando o nome do arquivo e a chave.

Essa função utiliza a função `busca`, encontra a localização da chave no arquivo e, caso encontre, abre o arquivo, calcula em qual bloco está o registro, obtém esse bloco do arquivo, substitui a chave desse registro por `***` e reescreve todo o bloco novamente, além de reescrever o bloco do cabeçalho atualizando os números de registros existentes e logicamente removidos. Caso a remoção esteja ocorrendo no bloco do cabeçalho, é feita apenas uma reescrita com os dados atualizados.

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

Essa opção solicita ao usuário o número de registros a serem inseridos e, então, chama a função `insere` com dados gerados automaticamente.

## O arquivo de dados

### Características da estrutura

- Não possui ordenação entre os registros;
- Os registros são de tamanho fixo (100 bytes);
- Existe controle de blocos (cada bloco possui um número inteiro de registros);
- Os blocos possuem 512 bytes.

### Características dos registros

Os registro possuem 5 campos de tamanhos fixos para serem preenchidos (todos strings):
1. Chave (3 bytes)
2. Telefone (11 bytes)
3. Data de nascimento (8 bytes)
4. E-mail (30 bytes)
5. Nome (48 bytes)

### Características do cabeçalho

No primeiro bloco, o arquivo possui um cabeçalho de tamanho 50 bytes que mostra quantos registros existem no arquivo e quantos foram logicamente removidos. Assim, o primeiro bloco possui o cabeçalho e 4 registros, enquanto todos os outros blocos possuem 5 registros cada.

## Funções utilizadas no programa

- `void zerar(std::string path);`
- `int busca(std::string path, std::string chave);`
- `void insere(std::string path, std::string chave, std::string numero_ddd, std::string numero_prefixo, std::string numero_sufixo, std::string data_dia, std::string data_mes, std::string data_ano, std::string email, std::string nome);`
- `int listagem(std::string path, bool print = true);`
- `void remove(std::string path, std::string chave, bool print = true);`