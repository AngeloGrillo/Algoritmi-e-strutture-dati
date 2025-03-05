#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "invArray.h"

struct invArray_s {
    inv_t *tabInv;
    int nInv;
};

invArray_t invArray_init(){
    invArray_t t = malloc(sizeof(struct invArray_s));

    t->nInv = 0;
    return t;
}

void invArray_free(invArray_t invArray){
    if(invArray != NULL && invArray->tabInv != NULL)
        free(invArray->tabInv);
    if(invArray != NULL)
        free(invArray);
}

void invArray_read(FILE *fp, invArray_t invArray){
    int i;

    fscanf(fp, "%d", &(invArray->nInv));
    invArray->tabInv = malloc(invArray->nInv * sizeof(inv_t));
    for(i = 0; i < invArray->nInv; i++){
        inv_read(fp, &(invArray->tabInv[i]));
        invArray->tabInv[i].inUso = 0;
    }

}

void invArray_print(FILE *fp, invArray_t invArray){
    for(int i=0; i < invArray->nInv; i++){
        fprintf(fp, "%s\t%s\nhp: %d   mp: %d   atk: %d   def: %d   mag: %d   spr: %d\n",
                invArray->tabInv[i].nome,
                invArray->tabInv[i].tipo,
                invArray->tabInv[i].stat.hp,
                invArray->tabInv[i].stat.mp,
                invArray->tabInv[i].stat.atk,
                invArray->tabInv[i].stat.def,
                invArray->tabInv[i].stat.mag,
                invArray->tabInv[i].stat.spr);
    }
}

void invArray_printByIndex(FILE *fp, invArray_t invArray, int index){
    inv_print(fp, &(invArray->tabInv[index]));
}

inv_t *invArray_getByIndex(invArray_t invArray, int index){
    return &(invArray->tabInv[index]);
}

int invArray_searchByName(invArray_t invArray, char *name){
    int i;

    for(i=0; i < invArray->nInv; i++){
        if(strcmp(name,invArray->tabInv[i].nome) == 0)
            return i;
    }
    return -1;
}