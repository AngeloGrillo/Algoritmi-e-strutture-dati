#ifndef ALGORITMI_E_STRUTTURE_DATI_ACT_H
#define ALGORITMI_E_STRUTTURE_DATI_ACT_H

#endif //ALGORITMI_E_STRUTTURE_DATI_ACT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXC 21 ///Modifica del numero massimo di caratteri da 20 a 21 per comprenere anche nomi di 20 caratteri + \0

typedef struct activities *ACT;
typedef struct{
    char nome[MAXC];
    int inizio,
        durata,
        profitto,
        nVin;
    char vincoli[2][MAXC]; ///Ho dimenticato di modificare il tipo di dato vincoli con char al posto di attivita
}attivita;

ACT activityRead(FILE *f);
int ACTfindByName(char *tmp, ACT elencoAct);
int checkSelection_r(int pos, ACT a/*int *val*/, int *sol/*int *sol*/, int nsel, int count, int *bsol, int *maxtop);
int ACTgetN(ACT elencoAct);
void ACTprintAtt(int index, ACT a);
void ACTdealloc(ACT a);
int compatible(ACT a, int *sol);
void PrintSol(ACT a, int *bestsol);
