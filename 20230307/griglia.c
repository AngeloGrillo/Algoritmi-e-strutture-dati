#include "griglia.h"

Token **malloc2d(int l){
    Token **mat;
    int i;
    mat=(Token**)malloc(l*sizeof(Token*));
    if(mat==NULL) exit(EXIT_FAILURE);
    for(i=0; i<l; i++){
        mat[i]=(Token*)malloc(l*sizeof(Token));
        if(mat[i]==NULL) exit(EXIT_FAILURE);
    }
    return mat;
}

void free2d(Token **mat, int nr){
    int i;
    if(mat!=NULL){
        for(i=0; i<nr; i++){
            if(mat[i]!=NULL)
                free(mat[i]);
        }
        free(mat);
    }
}

Grid GRIDread(FILE *f){
    Grid g;

    fscanf(f,"%d", &g.l);
    g.grid = malloc2d(g.l);
    for(int i=0; i<g.l; i++)
        for(int j=0; j<g.l; j++)
            g.grid[i][j] = TOKENread(f);

    return g;
}

void GRIDclear(Grid *g){
    free2d(g->grid, g->l);
}