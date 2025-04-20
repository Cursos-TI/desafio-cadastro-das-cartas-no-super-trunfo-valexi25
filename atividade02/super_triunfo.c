#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ESTADOS 8
#define CIDADE_POR_ESTADO 4
#define MAX_CARTA 32

typedef struct {
    char codigo[4];
    int populacao;
    float area;
    float pib;
    int pontos_turisticos;
} CartaCidades;

void trim(char *str) {
    char *end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
}

int validarCodigo(const char *codigo) {
    if (strlen(codigo) != 3) return 0;
    if (!isalpha(codigo[0]) || toupper(codigo[0]) < 'A' || toupper(codigo[0]) > 'H') return 0;
    if (!isdigit(codigo[1]) || !isdigit(codigo[2])) return 0;

    int numeroCidade = (codigo[1] - '0') * 10 + (codigo[2] - '0');
    return (numeroCidade >= 1 && numeroCidade <= CIDADE_POR_ESTADO);
}

float calcularDensidadePopulacional(CartaCidades cidade) {
    return cidade.populacao / cidade.area;
}

float calcularPIBPerCapita(CartaCidades cidade) {
    return (cidade.pib * 1000000000) / cidade.populacao;
}

int cadastrarCartas(CartaCidades cartas[], int maxCartas, int cidadePorEstado[]) {
    int qtdCartas = 0;
    char continuar = 'S';

    printf("+++ Cadastro de cartas +++\n");
    while (continuar == 'S' && qtdCartas < maxCartas) {
        char codigo[4];
        printf("\nCarta %d:\n", qtdCartas + 1);
        do {
            printf("Digite o código da cidade (ex: A01): ");
            fgets(codigo, sizeof(codigo), stdin);
            trim(codigo);

            if (!validarCodigo(codigo)) {
                printf("Código inválido! Deve estar no formato A01 a H04.\n");
                continue;
            }

            int estado = toupper(codigo[0]) - 'A';
            if (cidadePorEstado[estado] >= CIDADE_POR_ESTADO) {
                printf("O estado %c já atingiu o limite de 4 cidades.\n", codigo[0]);
                codigo[0] = '\0';
            }
        } while (codigo[0] == '\0');

        strcpy(cartas[qtdCartas].codigo, codigo);
        int estado = toupper(codigo[0]) - 'A';
        cidadePorEstado[estado]++;

        printf("Digite a população: ");
        scanf("%d", &cartas[qtdCartas].populacao);
        printf("Digite a área (Km²): ");
        scanf("%f", &cartas[qtdCartas].area);
        printf("Digite o PIB (em bilhões): ");
        scanf("%f", &cartas[qtdCartas].pib);
        printf("Digite o número de pontos turísticos: ");
        scanf("%d", &cartas[qtdCartas].pontos_turisticos);
        getchar(); // limpar buffer

        qtdCartas++;

        if (qtdCartas < maxCartas) {
            printf("Deseja cadastrar outra carta? (S/N): ");
            scanf(" %c", &continuar);
            continuar = toupper(continuar);
            getchar(); // limpar buffer
        } else {
            printf("Limite máximo de cartas alcançado.\n");
        }
    }
    return qtdCartas;
}

void exibirCartas(CartaCidades cartas[], int qtdCartas) {
    printf("\n=== Cartas cadastradas ===\n");
    for (int i = 0; i < qtdCartas; i++) {
        printf("\nCarta %s:\n", cartas[i].codigo);
        printf("População: %d\n", cartas[i].populacao);
        printf("Área: %.2f (km²)\n", cartas[i].area);
        printf("PIB: %.2f bilhões\n", cartas[i].pib);
        printf("Pontos Turísticos: %d\n", cartas[i].pontos_turisticos);
        printf("Densidade Populacional: %.2f hab/km²\n", calcularDensidadePopulacional(cartas[i]));
        printf("PIB per capita: %.2f\n", calcularPIBPerCapita(cartas[i]));
    }
}

int buscarCartaPorCodigo(CartaCidades cartas[], int qtdCartas, char codigo[]) {
    for (int i = 0; i < qtdCartas; i++) {
        if (strcmp(cartas[i].codigo, codigo) == 0) {
            return i;
        }
    }
    return -1;
}

void compararCartas(CartaCidades cartas[], int qtdCartas) {
    char cod1[4], cod2[4];
    printf("\nDigite o código da primeira carta: ");
    fgets(cod1, sizeof(cod1), stdin); trim(cod1);
    printf("Digite o código da segunda carta: ");
    fgets(cod2, sizeof(cod2), stdin); trim(cod2);

    int idx1 = buscarCartaPorCodigo(cartas, qtdCartas, cod1);
    int idx2 = buscarCartaPorCodigo(cartas, qtdCartas, cod2);

    if (idx1 == -1 || idx2 == -1) {
        printf("Uma ou ambas as cartas não foram encontradas.\n");
        return;
    }

    printf("\nEscolha o atributo para comparar:\n");
    printf("1 - População\n");
    printf("2 - Área\n");
    printf("3 - PIB\n");
    printf("4 - Pontos Turísticos\n");
    printf("5 - Densidade Populacional\n");
    printf("6 - PIB per capita\n");

    int opcao;
    scanf("%d", &opcao); getchar();

    float valor1, valor2;
    switch (opcao) {
        case 1:
            valor1 = cartas[idx1].populacao;
            valor2 = cartas[idx2].populacao;
            break;
        case 2:
            valor1 = cartas[idx1].area;
            valor2 = cartas[idx2].area;
            break;
        case 3:
            valor1 = cartas[idx1].pib;
            valor2 = cartas[idx2].pib;
            break;
        case 4:
            valor1 = cartas[idx1].pontos_turisticos;
            valor2 = cartas[idx2].pontos_turisticos;
            break;
        case 5:
            valor1 = calcularDensidadePopulacional(cartas[idx1]);
            valor2 = calcularDensidadePopulacional(cartas[idx2]);
            break;
        case 6:
            valor1 = calcularPIBPerCapita(cartas[idx1]);
            valor2 = calcularPIBPerCapita(cartas[idx2]);
            break;
        default:
            printf("Opção inválida.\n");
            return;
    }

    int resultado = (valor1 > valor2) ? 1 : (valor1 < valor2) ? -1 : 0;

    printf("\nResultado da comparação:\n");
    printf("%s: %.2f\n", cartas[idx1].codigo, valor1);
    printf("%s: %.2f\n", cartas[idx2].codigo, valor2);

    if (resultado == 1) {
        printf("A carta %s venceu!\n", cartas[idx1].codigo);
    } else if (resultado == -1) {
        printf("A carta %s venceu!\n", cartas[idx2].codigo);
    } else {
        printf("Empate!\n");
    }
}

int main() {
    CartaCidades cartas[MAX_CARTA];
    int cidadePorEstado[MAX_ESTADOS] = {0};
    int qtdCartas = 0;
    int opcao;

    do {
        printf("\n=== Menu Super Trunfo ===\n");
        printf("1 - Cadastrar cartas\n");
        printf("2 - Exibir cartas\n");
        printf("3 - Comparar cartas\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao); getchar();

        switch (opcao) {
            case 1:
                qtdCartas = cadastrarCartas(cartas, MAX_CARTA, cidadePorEstado);
                break;
            case 2:
                exibirCartas(cartas, qtdCartas);
                break;
            case 3:
                if (qtdCartas >= 2)
                    compararCartas(cartas, qtdCartas);
                else
                    printf("Cadastre ao menos duas cartas para comparar.\n");
                break;
            case 0:
                printf("Saindo do programa.\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}
