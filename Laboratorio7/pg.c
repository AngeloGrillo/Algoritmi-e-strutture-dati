#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "pg.h"

int pg_read(FILE *fp, pg_t *pgp){
    if(fscanf(fp,"%s%s%s%d%d%d%d%d%d",
              pgp->cod,
              pgp->nome,
              pgp->classe,
              &pgp->b_stat.hp,
              &pgp->b_stat.mp,
              &pgp->b_stat.atk,
              &pgp->b_stat.def,
              &pgp->b_stat.mag,
              &pgp->b_stat.spr) == 9){
        pgp->equip = equipArray_init();
        return 1;
    }

    return 0;
}