/*
    Funzione di intersezione piano segmento
*/

#include "Standard.h"
#include "Tipi.h"
#include "Operazioni_Liste.h"
#include "Strutture.h"


/* Input: Piano L e lista di segmenti t */

int intersection(Seg_List t, Plane L)
{
    int discorde = 0; // valore bool
    Point newPoint; // nuovo punto da aggiungere (se necessario)
    Seg newSeg;  // nuovo segmento da aggiungere (se necessario)
    double t0 = 0; // double per il calcolo dell'intersezione
    Face_PointerList tmpF, tmpFnew, tmpFold; // puntatore per assegnare le facce nuove
    Seg_PointerList tmpS; // puntatore per gestire i segmenti nuovi
    Seg_PointerList tmp_next;

    int trovato = 0;
    //t->S.A  t è un puntatore ad un elemento della lista segmenti:accedo all'oggetto puntato S che a sua volta è una struct e accedo dunque al campo A


    discorde = ((t->S.A->side != t->S.B->side) && (t->S.A->side != '0') && (t->S.B->side != '0')); // controllo se il segmento è di tipo intersezione(ovvero i due suoi
                                                                                                   // estremi appartengono a due zone opposte rispetto al piano) e che
                                                                                                   // non siano tutti e due dentro la tolleranza


    // set del flag tra intersection e face slicer, con 0 e 1 se deve lavorare o no
    // ricordo che A è un puntatore a punto
    if (discorde || t->S.A->side == '0' || t->S.B->side == '0') // se è discorde oppure ha almeno un punto di tipo intersezione
    {
        // imposta il flag perchè face slicer lavori su tutte le facce che condividono quel segmento,cioè segnalo nelle facce che il segmento viene tagliato dal                // piano
        tmpF = t->S.f; // assegno la testa della lista dei puntatori alle facce in cui è presente il segmento

        while (tmpF != NULL)
        {
            tmpF->fptr->side = 1; //setto il flag su lavora
            tmpF = tmpF->next; // e scorro avanti il cursore
        }
    }

    if (discorde) // caso segmento discorde, calcolo
    {
        t0 = ((L.pt.x - t->S.A->x)*(L.n.x) + ((L.pt.y - t->S.A->y)*(L.n.y)) + (L.pt.z - t->S.A->z)*(L.n.z)) / ((t->S.B->x - t->S.A->x)*(L.n.x) + ((t->S.B->y - t->S.A->y)*(L.n.y)) + (t->S.B->z - t->S.A->z)*(L.n.z));
    // Calcolo del paramentro d'intersezione, partendo dal punto A !!

    newPoint.x = t->S.A->x + t0*(t->S.B->x - t->S.A->x); // Calcolo nuove coordinate
    newPoint.y = t->S.A->y + t0*(t->S.B->y - t->S.A->y);
    newPoint.z = t->S.A->z + t0*(t->S.B->z - t->S.A->z);
    newPoint.side = '0'; // Inserisco il nuovo punto come intersezione

    Point_add_head(newPoint, Pt, &Pt); // Aggiunge in nuovo punto newPoint in testa alla lista di punti Pt
    NUMPUNTI++; //incremento il numero di punti totali

    /* Ora agisco sui segmenti: prima componente del primo immutata, la seconda la sostituisco col pt nuovo, nel
     nuovo segmento primo componente è il pt nuovo, la seconda il secondo punto del segmento originale */

    //Lascio immutato il primo punto del segmento originale
    newSeg.B = t->S.B; // seconda comp del nuovo segmento = seconda comp segmento originale
    t->S.B = &(Pt->P); // Il secondo punto lo sostituisco con quello nuovo (NELLA LISTA PRINCIPALE!!)
    newSeg.A = &(Pt->P); // Idem per la prima componente del nuovo segmento

    //************ Ora copio la lista di facce per il nuovo segmento

    tmpFold = t->S.f; // Assegno il punto di partenza(il puntatore al primo elemento della lista di puntatori alle facce che hanno il segmento spaccato come lato
    if (tmpFold != NULL ) // se non è nullo
    {
        newSeg.f = (Face_PointerList) malloc(sizeof(Face_PointerList_El));  // alloco il primo elemento della nuova lista
        newSeg.f->fptr = tmpFold->fptr; // copio il puntatore alla prima faccia
        newSeg.f->next = NULL;
        tmpFnew = newSeg.f;
    }

    while (tmpFold->next != NULL) // ciclo di copia delle facce
    {
        tmpFold = tmpFold -> next;
        tmpFnew->next = (Face_PointerList) malloc(sizeof(Face_PointerList_El)); // alloca il prossimo elemento
        tmpFnew->next->fptr = tmpFold->fptr; // copia il puntatore alla faccia
        tmpFnew->next->next = NULL; // segna il successore come puntatore a null
        tmpFnew = tmpFnew -> next; // e scorri avanti nella lista
    }
    //********************fine copia lista facce

    // finito coi segmenti nuovi

    Seg_add_head(newSeg, Sg, &Sg); // Aggiungo il nuovo segmento in testa alla lista
    NUMSEG++;

    /* Ora agisco sulle facce:

    // ***********Scorro le facce che puntavano al segmento originale e le faccio puntare ai DUE nuovi segmenti !! */

    tmpF = t->S.f; // assegno il cursore temporaneo della lista di puntatori a facce

    while (tmpF != NULL) // ciclo esterno sulle facce che contengono il segmento su cui sto lavorando
    {
        tmpS = tmpF->fptr->s; // assegno il cursore temporaneo dei segmenti
        if (tmpS->sptr == &(t->S)) //controllo se è buono il primo segmento
        {
            trovato=1;
            if (tmpS->orient == '+') //se il vecchio segmento era concorde col verso della faccia -> aggiungere dopo
            {
                tmp_next = tmpS->next; //salvo il secondo elemento
                tmpS->next = malloc(sizeof(Seg_PointerList_El)); //alloco
                tmpS->next->sptr = &(Sg->S); // copio il puntatore al nuovo segmento
                tmpS->next->orient = '+'; // ed è concorde
                tmpS->next->next = tmp_next; // e accodo la lista

            }
            else  // aggiungi prima!
            {
                tmp_next = tmpF->fptr->s; // temporaneo
                tmpF->fptr->s = malloc(sizeof(Seg_PointerList_El)); // alloco
                tmpF->fptr->s->sptr = &(Sg->S); // copio il puntatore al nuovo seg
                tmpF->fptr->s->orient = '-'; // è discorde
                tmpF->fptr->s->next = tmp_next; // accodo la vecchia testa
            }
        }
        //tmpS = tmpS->next ; // NON scorro avanti
        while ( (tmpS->next != NULL) && !trovato) // Ciclo sui segmenti di ogni (retro-) faccia finchè non trovo il segmento
        {
            if (tmpS->next->next == NULL) //sono arrivato alla fine della lista
            {
                 if( tmpS->next->sptr != &(t->S) ){
                    printf("\n\n************ERRORE!!******* \n \n ");
                 }
                 trovato=1;
                 if (tmpS->next->orient == '+' ) // aggiungo dopo
                 {
                    tmpS->next->next = malloc(sizeof(Seg_PointerList_El)); //alloco
                    tmpS->next->next->sptr = &(Sg->S); // copio il puntatore al segmento
                    tmpS->next->next->orient = '+'; // è concorde
                    tmpS->next->next->next = NULL;
                 }
                 else
                 {
                    tmp_next = tmpS->next; // temporaneo
                    tmpS->next = malloc(sizeof(Seg_PointerList_El)); // alloco
                    tmpS->next->sptr = &(Sg->S); // copio il puntatore al nuovo seg
                    tmpS->next->orient = '-'; // è discorde
                    tmpS->next->next = tmp_next; // accodo la vecchia testa
                 }
            }
            if((tmpS->next->sptr == &t->S) && !trovato) // se il prossimo è il segmento giusto
            {
                trovato=1;
                if (tmpS->next->orient == '+') //se il vecchio segmento era concorde col verso della faccia -> aggiungere dopo
                {
                    tmp_next = tmpS->next->next; //salvo il secondo successivo elemento
                    tmpS->next->next = malloc(sizeof(Seg_PointerList_El)); //alloco
                    tmpS->next->next->sptr = &(Sg->S); // copio il puntatore al nuovo segmento
                    tmpS->next->next->orient = '+'; // ed è concorde
                    tmpS->next->next->next = tmp_next; // e accodo la lista

                }
                else  // aggiungi prima!
                {
                    tmp_next = tmpS->next; // temporaneo
                    tmpS->next = malloc(sizeof(Seg_PointerList_El)); // alloco
                    tmpS->next->sptr = &(Sg->S); // copio il puntatore al nuovo seg
                    tmpS->next->orient = '-'; // è discorde
                    tmpS->next->next = tmp_next; // accodo la vecchia testa
                }
            }
            tmpS=tmpS->next;
        }
        tmpF=tmpF->next;
        trovato=0;
    } // Fine ciclo sulle facce

    }  // Fine caso discorde

    return 0;
}


int splitting_segs(Seg_List S, Plane P) // funzione che riceve in input la lista di segmenti ed un piano
{
    while ( S != NULL)
    {
        intersection(S , P);

        S = S->next;
    }

    return 0;
}
