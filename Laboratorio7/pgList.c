#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "pgList.h"

typedef struct node *link;
struct node {
    pg_t head;
    link next;
};

struct pgList_s {
    link headPg;
    int nPg;
};

pgList_t pgList_init(){
    pgList_t t = malloc(sizeof(pgList_t));

    t->headPg = NULL;
    t->nPg = 0;

    return t;
}

void pgList_free(pgList_t pgList){
    link p,q;
    for(p = pgList->headPg; p != NULL; p = q){
        pg_clean(&(p->head));
        q = p->next;

        free(p);
    }

    if(pgList != NULL) free(pgList);
}

void pgList_read(FILE *fp, pgList_t pgList){
    pg_t pers;
    while(pg_read(fp, &(pers))) {
        pgList_insert(pgList,pers);
    }
}

void pgList_print(FILE *fp, pgList_t pgList, invArray_t invArray){
    link x;
    for(x = pgList->headPg; x != NULL; x = x->next){
        pg_print(fp, &(x->head), invArray);
    }
    fprintf(fp,"\n\n");
}

void pgList_insert(pgList_t pgList, pg_t pg){
    link toAdd = malloc(sizeof(struct node));

    toAdd->head = pg;
    toAdd->next = pgList->headPg;
    pgList->headPg = toAdd;
    pgList->nPg++;
}

void pgList_remove(pgList_t pgList, char* cod){
    link p,q;

    // caso limite, eliminazione primo elemento
    if(strcmp(pgList->headPg->head.cod,cod) == 0) {
        pg_clean(&(pgList->headPg->head));
        q = pgList->headPg;
        free(q);
        pgList->headPg = pgList->headPg->next;
        return;
    }

    for(p = pgList->headPg; p != NULL; q = p, p = p->next) {
        if(strcmp(p->head.cod,cod) == 0) {
            pg_print(stdout,&(p->head),NULL);
            q->next = p->next;
            pg_clean(&(p->head));
            free(p);
            return;
        }
    }
    fprintf(stderr,"Personaggio non trovato.\n");
}

pg_t *pgList_searchByCode(pgList_t pgList, char* cod){
    link x;
    for(x = pgList->headPg; x != NULL; x = x->next){
        if(strcmp(x->head.cod, cod) == 0)
            return &(x->head);
    }
    return NULL;
}