#include <stdio.h>
#include <stdlib.h>
#include "token.h"

typedef struct {
    Token **grid;
    int l;
}Grid;

Grid GRIDread(FILE *f);
void GRIDclear(Grid *g);
