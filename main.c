#include "impl.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    int tabelaIni_f = 0;
    int contador_de_colisoes = 0;
    int total_de_alunos = 0;
    tabelaHash_t tabelaHash;
    int opt = 0;

    while (opt != 1) {
        printf("===========-tabela hash-===========\n");
        printf("1: sair\n");
        printf("2: inicializar tabela hash\n");
        printf("3: imprimir tabela\n");
        printf("4: total de aluno e colisões\n");
        printf("5: procurar aluno\n");
        printf("6: adicionar aluno\n");
        printf("7: atualizar aluno\n");
        printf("8: remover aluno\n");
        printf("9: apagar tabela\n");
        printf("10: alterar tamanho da tabela\n");
        printf("11: inserir dados aleatorios\n");
        printf("opção: ");
        scanf("%d", &opt);
        printf("===================================\n");

        switch (opt) {
        case 1:
            break;

        case 2: {
            if (tabelaIni_f) {
                printf("tabela ja inicializada\n");
                break;
            }
            int tamanho;
            float test = 1;
            while (test) {
                printf("tamanho da tabela hash: ");
                scanf(" %d", &tamanho);
                test = log2(tamanho) - (int)log2(tamanho);
                if (test)
                    printf("o tamanho deve ser exponencial de 2\n");
                if (tamanho <= 0)
                    printf("o tamanho não pode ser negativo ou 0\n");
            }
            tabelaHash = inicializar_tabelaHash(tamanho);
            tabelaIni_f = 1;
            break;
        }

        case 3: {
            if (!tabelaIni_f) {
                printf("tabela não inicializada\n");
                break;
            }
            amostrar_tabela(&tabelaHash);
            break;
        }

        case 4: {
            printf("------------------------------\n");
            printf("total de alunos: %d\n", total_de_alunos);
            printf("total de colisões ate agora: %d\n", contador_de_colisoes);
            printf("------------------------------\n");
            break;
        }

        case 5: {
            if (!tabelaIni_f) {
                printf("tabela não inicializada\n");
                break;
            }
            uint32_t mat;
            clock_t start, end;
            printf("------------------------------\n");
            printf("matricula do aluno:");
            scanf("%d", &mat);
            start = clock();
            no_t *found = busca_aluno(mat, &tabelaHash);
            end = clock();
            if (!found) {
                printf("aluno não encontrado\n");
                printf("demorou: %lf segundos\n", ((double)(end - start)) / CLOCKS_PER_SEC);
                break;
            }
            printf("nome: %s\n", found->dado.nome);
            printf("curso: %s\n", found->dado.curso);
            printf("periodo: %s\n\n", found->dado.periodo);
            printf("demorou: %lf segundos\n", ((double)(end - start)) / CLOCKS_PER_SEC);
            printf("------------------------------\n");
            break;
        }

        case 6: {

            if (!tabelaIni_f) {
                printf("tabela não inicializada\n");
                break;
            }

            aluno_t novo;
            novo.mat = 0;
            char string[256];
            while (novo.mat <= 0) {
                printf("dados do novo aluno\n");
                printf("matricula: ");
                scanf("%d", &novo.mat);
                getchar();
                if (novo.mat <= 0)
                    printf("matricula não poder ser 0 ou negativa");
            }

            printf("nome: ");
            fgets(string, 256, stdin);
            string[strcspn(string, "\n")] = 0;
            novo.nome = allocar_string(string);

            printf("curso: ");
            fgets(string, 256, stdin);
            string[strcspn(string, "\n")] = 0;
            novo.curso = allocar_string(string);

            printf("matricula (Ano.Semestre: 2026.2): ");
            fgets(string, 256, stdin);
            string[strcspn(string, "\n")] = 0;
            novo.periodo = allocar_string(string);

            int ret = inserir_aluno(novo, &tabelaHash);
            if (ret) {
                printf("-------------------------\n");
                printf("aluno inserido\n");
                printf("total de alunos: %d\n", ++total_de_alunos);
                printf("-------------------------\n");
            } else {
               free(novo.nome);
               free(novo.curso);
               free(novo.periodo);
            }
            if (ret == 2) {
                printf("-------------------------\n");
                printf("colisão ocorrida/\n");
                printf("total: %d\n", ++contador_de_colisoes);
                printf("-------------------------\n");
            }
            break;
        }

        case 7: {
            if (!tabelaIni_f) {
                printf("tabela não inicializada\n");
                break;
            }
            aluno_t novo;
            printf("matricula do aluno a ser atualizado: ");
            scanf("%d", &novo.mat);
            getchar();

            if (!busca_aluno(novo.mat, &tabelaHash)) {
                printf("aluno não encontrado\n");
                break;
            }

            printf("----- novos dados -----\n");
            printf("nome: ");
            fgets(novo.nome, 256, stdin);
            novo.nome[strcspn(novo.nome, "\n")] = 0;

            printf("curso: ");
            fgets(novo.curso, 256, stdin);
            novo.curso[strcspn(novo.curso, "\n")] = 0;

            printf("matricula (Ano.Semestre: 2026.2): ");
            fgets(novo.periodo, 8, stdin);
            novo.periodo[strcspn(novo.periodo, "\n")] = 0;

            atualizar_aluno(novo, &tabelaHash);
            break;
        }

        case 8: {
            if (!tabelaIni_f) {
                printf("tabela não inicializada\n");
                break;
            }
            int mat;
            printf("matricula do aluno a ser removido: ");
            scanf("%d", &mat);
            if (remover_aluno(mat, &tabelaHash)) {
                printf("-------------------------\n");
                printf("aluno removido\n");
                printf("total de alunos: %d\n", --total_de_alunos);
                printf("-------------------------\n");
            } else {
                printf("-------------------------\n");
                printf("aluno não removido\n");
                printf("-------------------------\n");
            }
            break;
        }

        case 9: {
            if (!tabelaIni_f) {
                printf("tabela não inicializada\n");
                break;
            }
            clock_t start, end;
            printf("--------------\n");
            start = clock();
            liberar_tabela(&tabelaHash);
            end = clock();
            printf("tabela apagada\n");
            printf("demorou: %lf segundos\n", ((double)(end - start)) / CLOCKS_PER_SEC);
            tabelaIni_f = 0;
            contador_de_colisoes = 0;
            total_de_alunos = 0;
            printf("--------------\n");
            break;
        }

        case 10: {
            if (!tabelaIni_f) {
                printf("tabela não iniciada\n");
                break;
            }
            int tam;
            printf("--------------\n");
            printf("novo tamanho: ");
            scanf(" %d", &tam);
            getchar();
            if (tam <= 0) {
                printf("tamanho não pode ser negativo ou 0\n");
                break;
                printf("--------------\n");
            }
            clock_t start, end;
            printf("--------------\n");
            start = clock();
            alterar_tamanho(tam, &tabelaHash);
            end = clock();
            printf("demorou: %lf segundos\n", ((double)(end - start)) / CLOCKS_PER_SEC);
            printf("--------------\n");
            break;
        }

        case 11: {
            if (!tabelaIni_f) {
                printf("tabela não iniciada\n");
                break;
            }
            clock_t start, end;
            int qnt;
            printf("--------------\n");
            printf("quantidade de elementos: ");
            scanf("%d", &qnt);
            getchar();
            start = clock();
            rand_dados(qnt, &contador_de_colisoes, &total_de_alunos, &tabelaHash);
            end = clock();
            printf("demorou: %lf segundos\n", ((double)(end - start)) / CLOCKS_PER_SEC);
            printf("--------------\n");
            break;
        }

        default:
            getchar();
        }
    }
    if (tabelaIni_f)
        liberar_tabela(&tabelaHash);
    return 0;
}
