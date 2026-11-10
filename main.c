#include "impl.h"
#include <math.h>
#include <stdio.h>

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
        printf("5: adicionar aluno\n");
        printf("6: atualizar aluno\n");
        printf("7: remover aluno\n");
        printf("8: apagar tabela\n");
        printf("opção: ");
        scanf("%d", &opt);
        printf("===================================\n");

        switch (opt) {
        case 1:
            return 0;

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
                    printf("o log com base 2 do tamanho deve resultar em um numero inteiro\n");
            }
            tabelaHash = inicializar_tabelaHash(tamanho);
            tabelaIni_f = 1;
            break;
        }

        case 3: {
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

            aluno_t novo;
            char string[256];
            printf("dados do novo aluno\n");
            printf("matricula: ");
            scanf("%d", &novo.mat);
            getchar();

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
            }
            if (ret == 2) {
                printf("-------------------------\n");
                printf("colisão ocorrida/\n");
                printf("total: %d\n", ++contador_de_colisoes);
                printf("-------------------------\n");
            }
            break;
        }

        case 6: {
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

        case 7: {
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

        case 8: {
            printf("--------------\n");
            liberar_tabela(&tabelaHash);
            printf("tabela apagada\n");
            printf("--------------\n");
        }

        default:
            getchar();
        }
    }
    liberar_tabela(&tabelaHash);
}
