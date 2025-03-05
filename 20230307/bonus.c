#include "bonus.h"

TokenBonus TOKENBONUSread(FILE *f){
    TokenBonus tb;

    fscanf(f,"%d %d", &tb.dim, &tb.bonus);
    tb.t = (Token*)malloc(tb.dim * sizeof(Token));
    for(int i=0; i<tb.dim; i++){
        tb.t[i] = TOKENread(f);
    }
    return tb;
}

void TOKENBONUSclear(TokenBonus *tb){
    if(tb->t!=NULL)
        free(tb->t);
}

Bonus BONUSread(FILE *f){
    Bonus b;
    int i;
    fscanf(f, "%d", &b.dim);
    b.tbonus=(TokenBonus*)malloc(b.dim*sizeof(TokenBonus));
    if(b.tbonus==NULL) exit(EXIT_FAILURE);
    for(i=0; i<b.dim; i++){
        b.tbonus[i]=TOKENBONUSread(f);
    }
    return b;
}


void BONUSclear(Bonus *b){
    if(b->tbonus!=NULL)
        free(b->tbonus);
}