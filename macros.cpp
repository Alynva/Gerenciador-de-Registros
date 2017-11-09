#ifndef __MACROS__
#define __MACROS__

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

#define FILE_NAME "arquivo.bin"

#define TAM_BLOCK 512
#define TAM_CABECALHO 50
#define TAM_REGISTRO 100

#define RRN2NBLOCK(rrn) (rrn / TAM_BLOCK)
#define RRN2REGINBLOCK(rrn) (rrn - RRN2NBLOCK(rrn) * TAM_BLOCK)
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

#endif