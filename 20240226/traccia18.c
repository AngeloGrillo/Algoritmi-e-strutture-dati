#include <stdio.h>
#include "ACT.h"
#define nomeFile "../20240226/input.txt"

int main() {
    ACT a;
    int *v, *bestsol, maxtop;  // Cambiato per allocare dinamicamente l'array
    FILE *fp;

    fp = fopen(nomeFile, "r");
    if (fp == NULL) {
        printf("Errore nell'apertura del file\n");
        exit(1);
    }

    a = activityRead(fp);

    // Allocazione dinamica di 'v' in base al numero di attività
    v = (int *)malloc(ACTgetN(a) * sizeof(int));
    bestsol = (int *)malloc(ACTgetN(a) * sizeof(int));
    // Chiamata a checkSelection_r con nAtt come nsel
    checkSelection_r(0, a, v, ACTgetN(a), 0, bestsol, &maxtop);

    for(int i=0; i < ACTgetN(a); i++){
        printf("%d ", bestsol[i]);
    }
    printf("\nMigliore combinazione: ");
    PrintSol(a, bestsol);
    // Libera la memoria allocata
    free(v);
    ACTdealloc(a);

    return 0;
}
