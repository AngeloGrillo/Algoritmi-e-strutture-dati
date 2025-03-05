#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <stdlib.h>

typedef struct{
    char car[3];
}Token;

Token TOKENread(FILE *f);
int TOKENcompare(Token t1, Token t2);
int TOKENisSubToken(Token *t1, int dim1, Token *t2, int dim2);
void TOKENprint(Token t, FILE *f);

#endif // TOKEN_H