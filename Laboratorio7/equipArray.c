#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "equipArray.h"

struct equipArray_s {
    int *array;
    int n,max_n;
};

equipArray_t equipArray_init(){
    equipArray_t equipArray = malloc(sizeof(struct equipArray_s));
    equipArray->n = 0;
    equipArray->max_n = EQUIP_SLOT;

    equipArray->array = malloc(EQUIP_SLOT * sizeof(int));
    if(equipArray->array == NULL) {
        fprintf(stderr, "Errore allocazione dinamica.\n");
        exit(EXIT_FAILURE);
    }

    return equipArray;
}

void equipArray_free(equipArray_t equipArray){
    if(equipArray != NULL) {
        if(equipArray->array != NULL)
            free(equipArray->array);

        free(equipArray);
    }
}

int equipArray_inUse(equipArray_t equipArray){
    return equipArray->n;
}

void equipArray_print(FILE *fp, equipArray_t equipArray, invArray_t invArray){
    int i;
    if(equipArray->n == 0) return;

    fprintf(fp,"\tEquipaggiamenti:\n\t\t%-20s %-20s\t%s\n","NOME","TIPO","STATISTICHE");
    for(i = 0; i < equipArray->n; i++){
        fprintf(fp,"\t\t");
        invArray_printByIndex(fp,invArray,equipArray->array[i]);
    }
}

void equipArray_update(equipArray_t equipArray, invArray_t invArray){
    int i;
    if(equipArray->n >= equipArray->max_n){
        fprintf(stderr, "Errore, slot per l'aggiunta di equipaggiamento pieni.\n");
        return;
    }

    printf("Equipaggiamenti disponibili:\n");
    invArray_print(stdout,invArray);

    printf("Scegli quale equipaggiamento aggiungere: ");
    scanf("%d",&i);
    equipArray->array[equipArray->n++] = i;
}

int equipArray_getEquipByIndex(equipArray_t equipArray, int index){
    if(index > equipArray->n){
        fprintf(stderr, "Errore, equipaggio non esistente!\n");
    }
    return(equipArray->array[index]);
}