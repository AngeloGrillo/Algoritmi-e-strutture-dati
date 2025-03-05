#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bonus.h"
#include "griglia.h"

typedef struct{
    Token t;
    int riga, colonna;
}Scelta;

typedef struct{
    Scelta *scelte;
    int dim;
}Soluzione;

int **malloc2D(int nr, int nc, int val);
void free2D(int **mat, int nr);
int Verify(Grid g, Bonus b, FILE *f, int *bonus);
int verifica(Soluzione proposta, Grid griglia, Bonus b, int L, int *bonus);
void solve(Bonus b, Grid griglia, int L);
void dispSemp(Bonus b, Grid griglia, int L, Soluzione *sol, Token *bestBuffer, int**mark, int *max, int pos, int riga, int colonna);

int main(){
    int bonus_proposta = 0;
    FILE *fp;
    Bonus b;
    Grid griglia;

    fp = fopen("../20230307/grid.txt", "r");
    griglia = GRIDread(fp);
    /*for(int i=0; i<griglia.l; i++){
        for(int j=0; j<griglia.l; j++)
            printf("%s ", griglia.grid[i][j].car);
        printf("\n");
    }*/
    fp = fopen("../20230307/bonus2.txt", "r");
    b = BONUSread(fp);
    /*for(int i=0; i<b.dim; i++) {
        printf("%d %d ", b.tbonus[i].dim, b.tbonus[i].bonus);
        for(int j=0; j<b.tbonus[i].dim; j++)
            printf("%s ", b.tbonus[i].t[j].car);
        printf("\n");
    }*/

    fp = fopen("../20230307/mosse2.txt", "r");
    if(Verify(griglia, b, fp, &bonus_proposta) == 0) printf("SEQUENZA VALIDA, BONUS = %d\n", bonus_proposta);
    else printf("SEQUENZA NON VALIDA\n");

    printf("---PROBLEMA DI RICERCA 1:---\n");
    solve(b , griglia, griglia.l);
    printf("\n\n");


    GRIDclear(&griglia);
    BONUSclear(&b);
    BONUSclear(&b);
    fclose(fp);
    return 0;
}

int Verify(Grid g, Bonus b, FILE *f, int *bonus){
    int nmove, r, rb = 0, c, cb = 0, count = 0, j;
    char car[10]; // Dimensione maggiore per evitare overflow
    char *str, *str2;

    // Legge il numero di mosse
    fscanf(f, "%d", &nmove);

    // Alloca dinamicamente memoria per la stringa
    str = calloc(nmove * 2 + 1, sizeof(char)); // +1 per terminatore


    // VALIDITA
    for(int i = 0; i < nmove; i++){
        // Legge un carattere e due interi
        fscanf(f, "%s %d %d", car, &r, &c);

        // Controlli logici
        if(count == 0 && r != 0) {
            free(str);
            return 1;
        }
        if(count % 2 == 0 && r != rb && count != 0) {
            free(str);
            return 1;
        }
        if(count % 2 != 0 && c != cb) {
            free(str);
            return 1;
        }
        // Concatenazione sicura
        strcat(str, car);
        // Aggiorna valori di riferimento
        rb = r;
        cb = c;
        count++;
    }

    str2 = calloc(nmove * 2 + 1, sizeof(char));
    // CONTA BONUS
    for(int i=0; i<b.dim; i++){
        for(j=0; j<b.tbonus[i].dim; j++){
            strcat(str2, b.tbonus[i].t[j].car);

        }
        if(strstr(str, str2)) *bonus += b.tbonus[i].bonus;
        str2[0] = '\0';
    }

    free(str);
    free(str2);
    return 0;
}

void solve(Bonus b, Grid griglia, int L){
    Soluzione sol;
    int **mark, max=0, i;
    Token *bestBuffer;

    sol.dim=L;

    sol.scelte=(Scelta*)malloc(L*sizeof(Scelta));
    if(sol.scelte==NULL) exit(EXIT_FAILURE);

    bestBuffer=(Token*)malloc(L*sizeof(Token));
    if(bestBuffer==NULL) exit(EXIT_FAILURE);

    mark=malloc2D(griglia.l, griglia.l, 0);

    dispSemp(b, griglia, L, &sol, bestBuffer, mark, &max, 0, 0, 0);

    printf("BestSolution:\n");
    printf("Buffer: ");
    for(i=0; i<L; i++)
        TOKENprint(bestBuffer[i], stdout);
    printf("\nBonus: %d", max);

    free(sol.scelte);
    free(bestBuffer);
    free2D(mark, griglia.l);
}

int verifica(Soluzione proposta, Grid griglia, Bonus b, int L, int *bonus){
    int i, index=0, valBonus=0;
    Token *buffer;
    buffer=(Token*)malloc(L*sizeof(Token));
    if(buffer==NULL) exit(EXIT_FAILURE);
    if(L!=proposta.dim)
        return 1;
    if(proposta.scelte[0].riga!=0)
        return 1;
    if(TOKENcompare(proposta.scelte[0].t, griglia.grid[proposta.scelte[0].riga][proposta.scelte[0].colonna])!=0)
        return 1;
    buffer[index++]=proposta.scelte[0].t;
    for(i=1; i<L; i++){
        if(i%2!=0 && proposta.scelte[i].colonna!=proposta.scelte[i-1].colonna)
            return 1;
        else if(i%2==0 && proposta.scelte[i].riga!=proposta.scelte[i-1].riga)
            return 1;
        if(TOKENcompare(proposta.scelte[i].t, griglia.grid[proposta.scelte[i].riga][proposta.scelte[i].colonna])!=0)
            return 1;
        buffer[index++]=proposta.scelte[i].t;
    }
    for(i=0; i<b.dim; i++){
        if(TOKENisSubToken(buffer, L, b.tbonus[i].t, b.tbonus[i].dim)==0){
            valBonus+=b.tbonus[i].bonus;
        }
    }
    *bonus=valBonus;
    return 0;
}

void dispSemp(Bonus b, Grid griglia, int L, Soluzione *sol, Token *bestBuffer, int**mark, int *max, int pos, int riga, int colonna){
    int bonus, i, r, c;
    if(pos>=L){
        if(verifica(*sol, griglia, b, L, &bonus)==0 && bonus > (*max)){
            (*max)=bonus;
            for(i=0; i<L; i++)
                bestBuffer[i]=sol->scelte[i].t;
        }
        return;
    }
    if(pos%2!=0){
        for(r=0; r<griglia.l; r++){
            if(mark[r][colonna]==0){
                mark[r][colonna]=1;
                sol->scelte[pos].t=griglia.grid[r][colonna];
                sol->scelte[pos].riga=r;
                sol->scelte[pos].colonna=colonna;
                dispSemp(b, griglia, L, sol, bestBuffer, mark, max, pos+1, r, colonna);
                mark[r][colonna]=0;
            }
        }
    }
    else if(pos%2==0){
        for(c=0; c<griglia.l; c++){
            if(mark[riga][c]==0){
                mark[riga][c]=1;
                sol->scelte[pos].t=griglia.grid[riga][c];
                sol->scelte[pos].riga=riga;
                sol->scelte[pos].colonna=c;
                dispSemp(b, griglia, L, sol, bestBuffer, mark, max, pos+1, riga, c);
                mark[riga][c]=0;
            }
        }
    }
}

int **malloc2D(int nr, int nc, int val){
    int **mat;
    int i, j;
    mat=(int**)malloc(nr*sizeof(int*));
    if(mat==NULL) exit(EXIT_FAILURE);
    for(i=0; i<nr; i++){
        mat[i]=(int*)malloc(nc*sizeof(int));
        if(mat[i]==NULL) exit(EXIT_FAILURE);
    }
    for(i=0; i<nr; i++){
        for(j=0; j<nc; j++){
            mat[i][j]=val;
        }
    }
    return mat;
}

void free2D(int **mat, int nr){
    int i;
    if(mat!=NULL){
        for(i=0; i<nr; i++){
            if(mat[i]!=NULL)
                free(mat[i]);
        }
        free(mat);
    }
    return;
}