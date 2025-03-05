#include "token.h"

typedef struct{
    Token *t;
    int bonus, dim;
}TokenBonus;

typedef struct{
    TokenBonus *tbonus;
    int dim;
}Bonus;

TokenBonus TOKENBONUSread(FILE *f);
void TOKENBONUSclear(TokenBonus *tb);

Bonus BONUSread(FILE *f);
void BONUSclear(Bonus *b);
