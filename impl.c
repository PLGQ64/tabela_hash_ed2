#include "impl.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <time.h>


tabelaHash_t inicializar_tabelaHash(size_t tamanho)
{
    tabelaHash_t tabelaHash = (tabelaHash_t) {
        .tamanho = tamanho,
        .bucket = malloc(sizeof(no_t*) * tamanho)
    };
    for (size_t i = 0; i < tamanho; i++) {
        tabelaHash.bucket[i] = NULL;
    }
    return tabelaHash;
}

size_t hash(uint32_t matricula, size_t tamanho)
{
    // o interger overflow é intensionado;
    // o overflow de um unsined int é definido;
    // os nós são mais espalhados com 32 bits;
    uint32_t temp = matricula * matricula;
    uint32_t moverBits = ((sizeof(temp) * 8) - log2(tamanho));
    // o log é o total de bits necessario para
    // representar um indice do vetor
    temp = (temp << (moverBits / 2)) >> moverBits;
    return temp;
}

no_t* busca_aluno(uint32_t matricula, tabelaHash_t* tabelaHash)
{
    size_t indice = hash(matricula, tabelaHash->tamanho);
    no_t* aux = tabelaHash->bucket[indice];
    while (aux != NULL) {
        if (matricula == aux->dado.mat)
            return aux;
        aux = aux->prox;
    }
    return NULL;
}

int inserir_aluno(aluno_t aluno, tabelaHash_t* tabelaHash)
{
    // retorna 1 por padrao
    int8_t ret = 1;
    size_t indice = hash(aluno.mat, tabelaHash->tamanho);
    // retorna 2 para indicar que ocorreu uma colisao
    if (tabelaHash->bucket[indice])
        ret = 2;

    if (busca_aluno(aluno.mat, tabelaHash)) {
        printf("------ info ------\n");
        printf("aluno ja presente\n");
        printf("------------------\n");
        return 0;
    }

    no_t* novo = malloc(sizeof(no_t));

    if (!novo) {
        perror("erro:");
        return 0;
    }

    novo->dado.mat = aluno.mat;
    novo->dado.nome = aluno.nome;
    novo->dado.curso = aluno.curso;
    novo->dado.periodo = aluno.periodo;
    novo->prox = tabelaHash->bucket[indice];
    tabelaHash->bucket[indice] = novo;
    return ret;
}

int remover_aluno(uint32_t matricula, tabelaHash_t* tabelaHash)
{
    size_t indice = hash(matricula, tabelaHash->tamanho);
    no_t* aux = tabelaHash->bucket[indice];

    // o bucket não possui elementos
    if (aux == NULL) {
        return 0;
    }

    // o primeiro nó é o a ser removido
    if (matricula == aux->dado.mat) {
        tabelaHash->bucket[indice] = tabelaHash->bucket[indice]->prox;
        free(aux->dado.nome);
        free(aux->dado.curso);
        free(aux->dado.periodo);
        free(aux);
        return 1;
    }

    // anda o ponteiro axiliar pelo lista encadeada ate achar
    // o aluno
    while (aux != NULL || matricula != aux->prox->dado.mat) {
        aux = aux->prox;
    }

    // caso tenha andado a lista inteira e não tenha achado o aluno
    if (aux == NULL) {
        return 0;
    }

    // faz a manipulçao de ponteiros
    no_t* aux2 = aux->prox;
    aux->prox = aux2->prox;
    // libera o ponteiro
    free(aux2->dado.nome);
    free(aux2->dado.curso);
    free(aux2->dado.periodo);
    free(aux2);

    return 1;
}

no_t *atualizar_aluno(aluno_t aluno, tabelaHash_t *tabelaHash)
{
    no_t *temp = busca_aluno(aluno.mat, tabelaHash);
    if (!temp)
        return NULL;

    temp->dado.mat = aluno.mat;
    strncpy(temp->dado.periodo, aluno.periodo, 8);
    strncpy(temp->dado.nome, aluno.nome, 256);
    strncpy(temp->dado.curso, aluno.curso, 256);
    return temp;
}

void liberar_tabela(tabelaHash_t *tabelaHash)
{
    no_t *aux;
    for (int i = 0; i < tabelaHash->tamanho; i++) {
        aux = tabelaHash->bucket[i];
        while (tabelaHash->bucket[i]) {
            aux = tabelaHash->bucket[i];
            tabelaHash->bucket[i] = tabelaHash->bucket[i]->prox;
            free(aux->dado.nome);
            free(aux->dado.curso);
            free(aux->dado.periodo);
            free(aux);
        }
    }
    free(tabelaHash->bucket);
}

void amostrar_tabela(tabelaHash_t *tabelaHash)
{
    no_t *aux;
    for (int i = 0; i < tabelaHash->tamanho; i++) {
        aux = tabelaHash->bucket[i];
        // esse if impede que buckets vasios sejam imprimidos
        if (aux == NULL)
            continue;
        printf("\n========-bucket [%d]-========\n", i);
        while (aux) {
            printf("matricula: %d\n", aux->dado.mat);
            printf("nome: %s\n", aux->dado.nome);
            printf("curso: %s\n", aux->dado.curso);
            printf("periodo: %s\n", aux->dado.periodo);
            printf("----------------------------\n");
            aux = aux->prox;
        }
    }
}

char *allocar_string(char *string)
{
    int string_size = strcspn(string, "\0");
    char *new = malloc(sizeof(char) * (string_size + 1));
    strcpy(new, string);
    return new;
}

void alterar_tamanho(int tamanho, tabelaHash_t *tabela)
{
    tabelaHash_t nova = inicializar_tabelaHash(tamanho);
    no_t *aux;
    size_t indice;
    for (int i = 0; i < tabela->tamanho; i++) {
        aux = tabela->bucket[i];
        while (aux) {
            indice = hash(aux->dado.mat, tamanho);
            tabela->bucket[i] = aux->prox;
            aux->prox = nova.bucket[indice];
            nova.bucket[indice] = aux;
            aux = tabela->bucket[i];
        }
    }
    free(tabela->bucket);
    *tabela = nova;
}

void rand_dados(int num_entradas, int *colisoes, int *alunos, tabelaHash_t *tabelaHash)
{
   srand(time(NULL)); 
   aluno_t novo;
   int data;
   char buf[256];
   for (int i = 0; i < num_entradas; i++) {
       data = (rand() % 1000000000) + 1;
       snprintf(buf, 256, "%d", data);
       novo.mat = data;
       novo.nome = allocar_string(buf);
       novo.curso = allocar_string(buf);
       novo.periodo = allocar_string(buf);
       int ret = inserir_aluno(novo, tabelaHash);
       if (!ret) {
           free(novo.nome);
           free(novo.curso);
           free(novo.periodo);
       } else {
           ++(*alunos);
       }
       if (ret == 2) {
           ++(*colisoes);
       }
   }
}
