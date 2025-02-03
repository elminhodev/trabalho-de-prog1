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
        printf("Capacidade maxima de registros atingida.\n");
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