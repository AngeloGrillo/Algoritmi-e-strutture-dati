#include "ACT.h"

struct activities{
    attivita *vettAttivita;
    int nAtt;
};

static ACT ACTinit(int nAtt);

ACT activityRead(FILE *f){
    int nA,nP,i,j,k,index,stop;
    char tmp[20], carattere;
    ACT elencoAct;
    fscanf(f,"%d %d", &nA, &nP);
    elencoAct = ACTinit(nA);

    for(i=0;i<nA;i++){
        fscanf(f,"%s %d %d %d", elencoAct->vettAttivita[i].nome, &(elencoAct->vettAttivita[i].inizio), &(elencoAct->vettAttivita[i].durata), &(elencoAct->vettAttivita[i].profitto));
    }
    for(i=0;i<nP;i++){
        fscanf(f,"%s", tmp);
        fscanf(f,"%c", &carattere); ///Aggiungo la lettura dello spazio dopo la stringa
        index = ACTfindByName(tmp, elencoAct);
        if(index == -1){
            printf("Attivita' a cui inserire vincoli inesistente!\n");
            exit(1);
        }
        stop=0;
        j=0;
        k=0; ///Aggiungo azzeramento di k
        while(!stop){
            if(j==2){
                stop = 1;
            }
            fscanf(f,"%c", &carattere);
            if(carattere == ' ' || carattere == '\n' || feof(f)){ ///Ho dimenticato di inserire il caso in cui la fscanf dovesse incontrare uno spazio e quando il file finisce, li aggiungo.
                tmp[k] = '\0'; ///Ho aggiunto l'inserimento del carattere di terminazione della stringa
                strcpy(elencoAct->vettAttivita[index].vincoli[j++],tmp); ///Nell'esame per una svista non ho usato strcpy, lo inserisco ora
                elencoAct->vettAttivita[index].nVin = j;
                k=0; ///Aggiungo l'azzeramento di k
                if(carattere == '\n' || feof(f)){
                    stop = 1;
                }
            }else{
                tmp[k++] = carattere;
            }
        }
    }
    return elencoAct;
}

int ACTfindByName(char *tmp, ACT elencoAct){
    int i;
    for(i=0; i<elencoAct->nAtt; i++)
        if(strcmp(tmp, elencoAct->vettAttivita[i].nome) == 0)
            return i;
    return -1;
}

static ACT ACTinit(int nAtt){
    ACT elencoAct;
    elencoAct = (ACT)malloc(sizeof(*elencoAct));
    elencoAct->vettAttivita = (attivita *)malloc(sizeof(attivita)*nAtt);

    for(int i=0; i<nAtt; i++)
        elencoAct->vettAttivita[i].nVin = 0;
    elencoAct->nAtt = nAtt;
    return elencoAct;
}

int ACTgetN(ACT elencoAct){
    return elencoAct->nAtt;
}

void ACTprintAtt(int index, ACT a){
    printf("%s %d %d %d\n", a->vettAttivita[index].nome, a->vettAttivita[index].inizio, a->vettAttivita[index].durata, a->vettAttivita[index].profitto);
}

void ACTdealloc(ACT a){
    for(int i=0; i < a->nAtt; i++){
        free(a->vettAttivita);
    }
    free(a);
}

int checkSelection_r(int pos, ACT a, int *sol, int nsel, int count, int *bsol, int *maxtop) {
    int i, totp;

    // Se abbiamo raggiunto la posizione finale, controlla la combinazione attuale
    if (pos >= nsel) {
        totp = 0;
        if (compatible(a, sol) == 1) {  // Verifica se la combinazione è compatibile
            for (i = 0; i < a->nAtt; i++) {
                if (sol[i] != 0) {
                    totp += a->vettAttivita[i].profitto;  // Somma il profitto delle attività selezionate
                }
            }

            // Se il profitto attuale è maggiore del massimo trovato finora, aggiorna la soluzione migliore
            if (totp > *maxtop) {
                *maxtop = totp;  // Aggiorna il profitto massimo
                for (int j = 0; j < a->nAtt; j++) {
                    bsol[j] = sol[j];  // Copia la soluzione corrente nella soluzione migliore
                }
            }
        }
        return count + 1;
    }

    // Escludi l'attività corrente
    sol[pos] = 0;
    count = checkSelection_r(pos + 1, a, sol, nsel, count, bsol, maxtop);

    // Includi l'attività corrente
    sol[pos] = 1;
    count = checkSelection_r(pos + 1, a, sol, nsel, count, bsol, maxtop);

    return count;  // Ritorna il conteggio aggiornato
}


int compatible(ACT a, int *sol) {
    for (int i = 0; i < a->nAtt; i++) {
        if (sol[i] != 0) {  // Se l'attività i è selezionata
            // 1. Verifica la sovrapposizione temporale con tutte le altre attività selezionate
            for (int j = 0; j < a->nAtt; j++) {
                if (sol[j] != 0 && j != i) {  // Se l'attività j è selezionata e non è la stessa attività
                    int inizio_i = a->vettAttivita[i].inizio;
                    int fine_i = a->vettAttivita[i].inizio + a->vettAttivita[i].durata;
                    int inizio_j = a->vettAttivita[j].inizio;
                    int fine_j = a->vettAttivita[j].inizio + a->vettAttivita[j].durata;

                    // Controllo sovrapposizione: fine di i deve essere <= inizio di j o viceversa
                    if (!(fine_i <= inizio_j || fine_j <= inizio_i)) {
                        return 0;  // C'è una sovrapposizione temporale
                    }
                }
            }

            // 2. Verifica i vincoli di precedenza
            for (int k = 0; k < a->vettAttivita[i].nVin; k++) {
                // Trova l'attività di cui i dipende (vincolo k)
                char *vincolo = a->vettAttivita[i].vincoli[k];
                int vincoloIndex = ACTfindByName(vincolo, a);

                // Se l'attività di cui i dipende non è selezionata o non finisce prima di i, allora non è compatibile
                if (vincoloIndex != -1 && (sol[vincoloIndex] == 0 ||
                                           (a->vettAttivita[vincoloIndex].inizio + a->vettAttivita[vincoloIndex].durata > a->vettAttivita[i].inizio))) {
                    return 0;  // Non rispetta i vincoli di precedenza
                }
            }
        }
    }

    // Se tutte le verifiche sono passate, le attività sono compatibili
    return 1;
}

void PrintSol(ACT a, int *bestsol){
    for(int i=0; i < a->nAtt; i++){
        if(bestsol[i] == 1)
            printf("%s ", a->vettAttivita[i].nome);
    }
}