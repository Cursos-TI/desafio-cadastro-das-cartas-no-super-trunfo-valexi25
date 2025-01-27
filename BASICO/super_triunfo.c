#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ESTADOS 8
#define CIDADE_POR_ESTADO 4
#define MAX_CARTA 32

typedef struct{
    char codigo[4];
    int populacao;
    float area;
    float pib;
    int pontos_turisticos;
}CartaCidades;

void trim(char *str) {
    char *end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
}

int validarCodigo(const char * codigo){
    if(strlen(codigo) != 3) return 0;
    if(!isalpha(codigo[0]) || toupper(codigo[0])< 'A' || toupper(codigo[0])>'H')return 0;
    if(!isalpha(codigo[1]) || !isdigit(codigo[2]))return 0;

    int numeroCidade = (codigo[1]- '0')* 10 + (codigo[2]-'0');
    return(numeroCidade >= 1 && numeroCidade <= CIDADE_POR_ESTADO);
}

int cadastrarCartas(CartaCidades cartas[], int maxCartas,int cidadePorEstado[]){
    int qtdCartas = 0;
    char continuar = 'S';

    printf("+++ cadastro de cartas +++\n");
    while(continuar =='S' && qtdCartas < maxCartas){
        char codigo[4];
        printf("\nCarta %d:\n",qtdCartas+1);
        printf("\n Carta %d:\n",qtdCartas + 1);
        do{
            printf("Digite o código da cidade (ex: A01)");
            fgets(codigo, sizeof(codigo), stdin);
            trim(codigo);
            if(!validarCodigo(codigo)){
                printf("Código invalido! Deve estar no formato A01 a H04.\n");
                continue;
            }
            int estado = toupper(codigo[0]) - 'A';
            if(cidadePorEstado[estado] >= CIDADE_POR_ESTADO){
                printf("O estado %c ja atingiu o limite de 4 cidade.\n",codigo[0]);
                codigo[0]='\0';
            }   
        }while(codigo[0]== '\0');

        strcpy(cartas[qtdCartas].codigo,codigo);
        int estado = toupper(codigo[0]) - 'A';
        cidadePorEstado[estado]++;

        while (getchar() != '\n');
        printf("digite a população: ");
        scanf("%d", &cartas[qtdCartas].populacao);
        while (getchar() != '\n');
        printf("Digite a área (Km²): ");
        scanf("%f", &cartas[qtdCartas].area);
        while (getchar() != '\n');
        printf("Digitei o PBI (em bilhoes): ");
        scanf("%f", &cartas[qtdCartas].pib);
        printf("Digite o número de pontos turisticos: ");
        scanf("%d",&cartas[qtdCartas].pontos_turisticos);

        qtdCartas++;
        if(qtdCartas<maxCartas){
            printf("\nDeseja cadastrar outra carta?(s/n):");
            scanf(" %c",&continuar);
        }else{
            printf("\nLimite maximo de cartas alcançado\n");
        }
    }
    return qtdCartas;
}

void exibirCartas(CartaCidades cartas[],int qtdCartas){
    printf("\n=== cartas cadastradas==\n");
    for(int i= 0;i<qtdCartas;i++){
        printf("\nCarta %s:\n",cartas[i].codigo);
        printf("\nPopulação: %d\n",cartas[i].populacao);
        printf("Área: %.2f (km²)\n",cartas[i].area);
        printf("PIB: %f bilhões\n",cartas[i].pib);
        printf("Pontos Turisticaos: %d\n",cartas[i].pontos_turisticos);
    }
}
int main(){
    CartaCidades cartas[MAX_CARTA];
    int cidadePorEstado[MAX_ESTADOS] = {0};
    int qtdCartas;
    printf("Bem-vindos ao sistema super Trunfo - Paísesz\n");
    qtdCartas=cadastrarCartas(cartas,MAX_CARTA,cidadePorEstado);
    exibirCartas(cartas,qtdCartas);
    return 0;
}