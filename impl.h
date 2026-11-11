#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint32_t mat;
    char *nome;
    char *curso;
    char *periodo;
} aluno_t;

typedef struct no {
    aluno_t dado;
    struct no* prox;
} no_t;

typedef struct {
    size_t tamanho;
    no_t** bucket;
} tabelaHash_t;


tabelaHash_t inicializar_tabelaHash(size_t tamanho);
size_t hash(uint32_t matricula, size_t tamanho);
no_t* busca_aluno(uint32_t matricula, tabelaHash_t* tabelaHash);
int inserir_aluno(aluno_t aluno, tabelaHash_t* tabelaHash);
int remover_aluno(uint32_t matricula, tabelaHash_t* tabelaHash);
no_t *atualizar_aluno(aluno_t aluno, tabelaHash_t *tabelaHash);
void liberar_tabela(tabelaHash_t *tabelaHash);
void amostrar_tabela(tabelaHash_t *tabelaHash);
char *allocar_string(char *string);
void alterar_tamanho(int tamanho, tabelaHash_t *tabela);

