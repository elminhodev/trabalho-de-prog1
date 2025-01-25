#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_RECORDS 1000

typedef struct {
    char dataHora[20];
    float consumo;
    float custo;
} Registro;

Registro registros[MAX_RECORDS];
int totalRegistros = 0;

void registrarConsumo() {
    if (totalRegistros >= MAX_RECORDS) {
        printf("Capacidade máxima de registros atingida.\n");
        return;
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