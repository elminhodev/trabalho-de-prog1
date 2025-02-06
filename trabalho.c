#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECORDS 1000

typedef struct {
    char dataHora[20];
    float consumo;
    float custo;
} Registro;

Registro *registros = NULL;
int totalRegistros = 0;
int capacidade = 10;

void alocarMemoria() {
    registros = (Registro *)malloc(capacidade * sizeof(Registro));
    if (registros == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
            }
}

void realocarMemoria() {
    capacidade += 10;
    registros = (Registro *)realloc(registros, capacidade * sizeof(Registro));
    if (registros == NULL) {
        printf("Erro ao realocar memória.\n");
        exit(1);
    }
}

void registrarConsumo() {
    if (totalRegistros >= capacidade) {
        realocarMemoria();
    }

    Registro novo;

    printf("Digite a data e hora (DD-MM-YYYY HH:MM): ");
    scanf(" %19[^\n]", novo.dataHora);

    printf("Digite o consumo em kWh: ");
    scanf("%f", &novo.consumo);

    printf("Digite o custo por kWh (em reais): ");
    scanf("%f", &novo.custo);

    registros[totalRegistros++] = novo;

    FILE *arquivo = fopen("consumo.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    fprintf(arquivo, "%s %.2f %.2f\n", novo.dataHora, novo.consumo, novo.custo);
    fclose(arquivo);

    printf("Registro salvo com sucesso.\n");
}

float calcularMedia(char tipo) {
    if (totalRegistros == 0) {
        printf("Nenhum registro encontrado.\n");
        return 0;
    }

    float totalConsumo = 0;
    int unidadesUnicas = 0;
    char ultimaUnidade[20] = "";

    for (int i = 0; i < totalRegistros; i++) {
        totalConsumo += registros[i].consumo;
        char unidadeAtual[20] = "";

        if (tipo == 'D') {
            strncpy(unidadeAtual, registros[i].dataHora, 10);
            unidadeAtual[10] = '\0';
        } else if (tipo == 'S') {
            int dia, mes, ano;
            sscanf(registros[i].dataHora, "%d-%d-%d", &dia, &mes, &ano);
            int semana = (dia - 1) / 7 + 1;
            sprintf(unidadeAtual, "%d-%02d", ano, semana);
        } else if (tipo == 'M') {
            int dia, mes, ano;
            sscanf(registros[i].dataHora, "%d-%d-%d", &dia, &mes, &ano);
            sprintf(unidadeAtual, "%d-%02d", ano, mes);
        }

        if (strcmp(unidadeAtual, ultimaUnidade) != 0) {
            unidadesUnicas++;
            strcpy(ultimaUnidade, unidadeAtual);
        }
    }

    return totalConsumo / unidadesUnicas;
}

void identificarPicos() {
    if (totalRegistros == 0) {
        printf("Nenhum registro encontrado.\n");
        return;
    }

    float media = calcularMedia('D');
    printf("Picos de consumo (consumo acima de 1.5 vezes a media diaria):\n");
    for (int i = 0; i < totalRegistros; i++) {
        if (registros[i].consumo > 1.5 * media) {
            printf("%s - %.2f kWh\n", registros[i].dataHora, registros[i].consumo);
        }
    }
}

void consumoTotalRecursivo(int index, float *totalConsumo, float *totalCusto) {
    if (index >= totalRegistros) {
        return;
    }
    *totalConsumo += registros[index].consumo;
    *totalCusto += registros[index].consumo * registros[index].custo;
    consumoTotalRecursivo(index + 1, totalConsumo, totalCusto);
}

void exibirConsumoTotal() {
    float totalConsumo = 0, totalCusto = 0;
    consumoTotalRecursivo(0, &totalConsumo, &totalCusto);
    printf("Consumo total: %.2f kWh\n", totalConsumo);
    printf("Custo total estimado: R$ %.2f\n", totalCusto);
}

void gerarRelatorio() {
    FILE *arquivo = fopen("relatorio.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo de relatorio.\n");
        return;
    }

    fprintf(arquivo, "Relatorio de Consumo:\n\n");
    for (int i = 0; i < totalRegistros; i++) {
        fprintf(arquivo, "%s - Consumo: %.2f kWh - Custo por kWh: R$ %.2f\n",
                registros[i].dataHora, registros[i].consumo, registros[i].custo);
    }

    fclose(arquivo);
    printf("Relatorio gerado com sucesso no arquivo 'relatorio.txt'.\n");
}

void analisarPadroes() {
    if (totalRegistros == 0) {
        printf("Nenhum registro para analise.\n");
        return;
    }

    float consumoPorHora[24] = {0};
    int contagemPorHora[24] = {0};

    for (int i = 0; i < totalRegistros; i++) {
        int hora;
        sscanf(registros[i].dataHora, "%*d-%*d-%*d %d", &hora);
        consumoPorHora[hora] += registros[i].consumo;
        contagemPorHora[hora]++;
    }

    int horaPico = 0;
    float maiorMedia = 0;
    for (int h = 0; h < 24; h++) {
        if (contagemPorHora[h] > 0) {
            float media = consumoPorHora[h] / contagemPorHora[h];
            if (media > maiorMedia) {
                maiorMedia = media;
                horaPico = h;
            }
        }
    }

    printf("\n=== SUGESTOES PARA ECONOMIA ===\n");
    printf("Horario de maior consumo: %02d:00 as %02d:59\n", horaPico, horaPico);
    printf("1. Reduzir uso neste horario.\n");
    printf("2. Verificar desperdicios.\n");
}

void menu() {
    int opcao;

    do {
        printf("\n--- Sistema de Gerenciamento de Consumo de Energia ---\n");
        printf("1. Registrar consumo\n");
        printf("2. Calcular media diaria de consumo\n");
        printf("3. Calcular media semanal de consumo\n");
        printf("4. Calcular media mensal de consumo\n");
        printf("5. Identificar picos de consumo\n");
        printf("6. Exibir consumo total e custo estimado\n");
        printf("7. Gerar relatorio detalhado\n");
        printf("8. Sugestoes para melhorar o consumo\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                registrarConsumo();
                break;
            case 2:
                printf("Media diaria de consumo: %.2f kWh\n", calcularMedia('D'));
                break;
            case 3:
                printf("Media semanal de consumo: %.2f kWh\n", calcularMedia('S'));
                break;
            case 4:
                printf("Media mensal de consumo: %.2f kWh\n", calcularMedia('M'));
                break;
            case 5:
                identificarPicos();
                break;
            case 6:
                exibirConsumoTotal();
                break;
            case 7:
                gerarRelatorio();
                break;
            case 8:
                analisarPadroes();
                break;
            case 0:
                printf("Saindo do programa.\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);
}

int main() {
    alocarMemoria();
    menu();
    free(registros);
    return 0;
}