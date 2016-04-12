/*
    File per scorrere sulla lista di facce!

*/

#include "Standard.h"
#include "Tipi.h"
#include "Operazioni_Liste.h"
#include "Strutture.h"

int face_slicer(Face_List silvia)
{
    Seg_PointerList tmpS, tmpSold; // cursore temporaneo per scorrere i segmenti della faccia
    Point* P1, *P2; // puntatore a punti temporanei
    Seg_PointerList S1; // lista segmento temporaneo
    Seg newSeg; // variabile segmento nuovo
    Seg_PointerList newTail, tmpHead, tmpCurs; // variabile faccia nuova
    Face newFace; // faccia nuova
    Face_PointerList tmpFace, tmpFaceCurs; //faccia temporanea
    short num = 0, primo=0, ultimo =0; // numero intersezioni trovate finora: si ferma a 2
    char flag_cons = 0; //flag: 0 spento, 1 acceso
    char original_orient; // char dell'orientamento nel primo solido

    unsigned int OrigFace; // marker per propagare l'info sul marker di quale faccia originale ha originato la corrente.
    unsigned long CausePlane; // marker per propagare l'info su quale piano ha originato la faccia corrente.

    if ( silvia ->F.side == 0) // controlla il flag se non devi operare
    {
        #ifdef DEBUG_H
        fprintf(OUTPUT, "Face_slicer: La faccia non necessita operazioni \n");
        #endif
        // la faccia non interseca: se è di tipo + assegno 4, se è di tipo - assegno 1
        if ( silvia->F.s->sptr->A->side == '+')
        {
            silvia->F.side = 4; // caso +
        }
        else
        {
            silvia->F.side = 1; // caso -
        }
        return 1;
    }

    OrigFace = silvia->F.OriginalFace; // assegna marker faccia originale.
    CausePlane = silvia->F.CausingPlane; // assegna marker piano di giacitura.

    tmpS = silvia->F.s;
    // primo caso fuori

    if (tmpS->orient == '+') // se il segmento è concorde
    {
        if (tmpS->sptr->A->side == '0')
        {
            flag_cons = 1;
            primo = 1;
            num++;
            P1 = tmpS->sptr->A; //salvo il punto in una var temporanea
            S1 = tmpS; // salvo il punto della lista del nuovo segmento
        }
    }
    else // se invece l'orient è negativo
    {
        if (tmpS->sptr->B->side == '0')
        {
            primo = 1;
            flag_cons = 1;
            num++;
            P1 = tmpS->sptr->B; //salvo il punto in una var temporanea
            S1 = tmpS; // idem
        }
    }

    tmpSold = silvia->F.s;
    tmpS = tmpS->next; // scorro avanti la lista
    while (tmpS != NULL && num<2)
    {
        if (tmpS->next == NULL){
            ultimo =1;
        }

        if (tmpS->orient == '+')
        {
            if (tmpS->sptr->A->side == '0')
            {
                if (flag_cons || (primo && ultimo) )
                {
                    #ifdef DEBUG_H
                    printf("\nFace_Slicer: La faccia era tangente al piano o a un lato \n");
                    #endif
                    return 1;
                }
                if (num<1)
                {
                    P1 = tmpS->sptr->A; // salvo il primo punto
                    S1 = tmpS; // e il primo segmento
                    num++;
                }
                else
                {
                    num++;
                    P2 = tmpS->sptr->A ; // salvo il punto A
                }

                flag_cons = 1;
            } // fine punto intersezione
            else // ALTRIMENTI SPENGO IL FLAG
            {
                flag_cons = 0;
            }

        } // fine orient +
        else // se invece l'orient era -
        {
            if (tmpS->sptr->B->side == '0')
            {
                if (flag_cons || (primo & ultimo))
                {

                    printf("Face_Slicer: La faccia era tangente al piano o a un lato \n");
                    return 1;
                }
                if (num<1)
                {
                    P1 = tmpS->sptr->B; // salvo il primo punto
                    S1 = tmpS; // salvo il primo segmento
                    num++;
                }
                else
                {
                    num++;
                    P2 = tmpS->sptr->B; // salvo il punto B

                }

                flag_cons = 1;
            } // fine punto intersezione
            else
            {
                flag_cons = 0;
            }
        } // fine orient negativo

        if(num<2){
            tmpSold = tmpS; // scorro avanti quello vecchio
            tmpS = tmpS->next; // scorro avanti nella lista
        }

    } // fine while

    #ifdef DEBUG_H
    if (num == 0)
    {
        fprintf(OUTPUT, "Face_Slicer: ERRORE SUL FLAG! \n");
        return -1;
    }
    #endif

    if (num == 1)
    {
        #ifdef DEBUG_H
        printf("\nFace_Slicer: La faccia era tangente in un vertice \n");
        #endif
        return 1;
    }
    // else: ci sono due punti e devo crearmi il segmento nuovo
    newSeg.A = P1; // assegno il primo punto
    newSeg.B = P2; // secondo

    // non abbiamo assegnato la lista di facce perchè lo facciamo dopo

    Seg_add_head(newSeg, Sg, &Sg); // aggiungi il nuovo segmento in testa alla lista di Sg
    NUMSEG++;

    // Assegno i marker delle fratture alla faccia nuova
    newFace.OriginalFace = OrigFace;
    newFace.CausingPlane = CausePlane;

    // ora "creo" le facce nuove
    Face_add_head(newFace, Fc, &Fc); // aggiungo la nuova faccia in testa alla lista


    if(!primo){
        newTail = tmpS; // salvo la lista della seconda parte della faccia, da P2
        tmpHead = silvia->F.s;     // salvo l'inizio della lista di segmenti, se no poi la perdo

        tmpSold->next = (Seg_PointerList)malloc(sizeof(Seg_PointerList_El)); // Alloco nuovo spazio per il nuovo segmento
        tmpSold->next->sptr = &(Sg->S); // gli assegno il segmento nuovo che avevo messo in testa alla lista
        tmpSold->next->orient = '-'; // rispetto alla prima faccia il nuovo segmento è discorde
        tmpSold->next->next = NULL; // indico la fine lista
        silvia->F.s = S1; // salvo la nuova testa della lista

        tmpCurs = tmpHead; // Cursore alla seconda lista
        while (!(Point_Equality(*(tmpCurs->sptr->A),*P1) || Point_Equality(*(tmpCurs->sptr->B),*P1) ))
        {
            tmpFace = tmpCurs->sptr->f; // assegno la testa della lista facce del segmento, per modificare e mettere quella nuova
            while ((tmpFace != NULL) && (tmpFace->fptr != &(silvia->F)) ) // verifico finchè non trovo la faccia vecchia
            {
                tmpFace = tmpFace->next; // scorro avanti la lista
            }
            if (tmpFace == NULL){
                printf("\n**********************ERRORE********************************\n");
            }
            if (tmpFace != NULL){
                tmpFace->fptr = &(Fc->F); // assegno la nuova faccia
            }

            tmpCurs = tmpCurs->next;
        } // quando esce è perchè è ora di incollarci il segmento nuovo.

        tmpFace = tmpCurs->sptr->f; // assegno la testa della lista facce del segmento, per modificare e mettere quella nuova
        while ((tmpFace != NULL) && (tmpFace->fptr != &(silvia->F)) ) // verifico finchè non trovo la faccia vecchia
        {
            tmpFace = tmpFace->next; // scorro avanti la lista
        }
        if (tmpFace == NULL){
            printf("\n**********************ERRORE********************************\n");
        }
        if (tmpFace != NULL){
            tmpFace->fptr = &(Fc->F); // assegno la nuova faccia
        }

        tmpCurs->next = (Seg_PointerList) malloc (sizeof(Seg_PointerList_El)); // alloco il nuovo spazio
        tmpCurs->next->sptr = &(Sg->S); //assegno il nuovo segmento
        tmpCurs->next->orient = '+'; // nella seconda faccia il seg nuovo è concorde

        tmpCurs->next->next = newTail; // ora incollo la parte restante della lista
        tmpCurs = tmpCurs->next->next;
        while (tmpCurs != NULL) // rifaccio lo stesso lavoro sui segmenti fino a fine lista
        {
            tmpFace = tmpCurs->sptr->f; // assegno la testa della lista facce del segmento, per modificare e mettere quella nuova
            while (tmpFace->fptr != &(silvia->F) ) // verifico finchè non trovo la faccia vecchia
            {
                tmpFace = tmpFace->next; // scorro avanti la lista
            }
            tmpFace->fptr = &(Fc->F); // assegno la nuova faccia
            tmpCurs = tmpCurs->next;
        } // arrivato a fine lista di segmenti, modificando tutti i puntatori a faccia
    } else{
        //faccia vecchia
        tmpSold->next=(Seg_PointerList) malloc (sizeof(Seg_PointerList_El));
        tmpSold->next->sptr=&Sg->S;
        tmpSold->next->orient='-';
        tmpSold->next->next=NULL;

        //faccia nuova
        tmpHead = tmpS;
        tmpCurs=tmpHead;

        while (tmpCurs->next != NULL) // rifaccio lo stesso lavoro sui segmenti fino a fine lista
        {
            tmpFace = tmpCurs->sptr->f; // assegno la testa della lista facce del segmento, per modificare e mettere quella nuova
            while (tmpFace->fptr != &(silvia->F) ) // verifico finchè non trovo la faccia vecchia
            {
                tmpFace = tmpFace->next; // scorro avanti la lista
            }
            tmpFace->fptr = &(Fc->F); // assegno la nuova faccia
            tmpCurs = tmpCurs->next;
        }

        tmpFace = tmpCurs->sptr->f; // assegno la testa della lista facce del segmento, per modificare e mettere quella nuova
        while ((tmpFace != NULL) && (tmpFace->fptr != &(silvia->F)) ) // verifico finchè non trovo la faccia vecchia
        {
            tmpFace = tmpFace->next; // scorro avanti la lista
        }
        if (tmpFace == NULL){
            printf("\n**********************ERRORE********************************\n");
        }
        if (tmpFace != NULL){
            tmpFace->fptr = &(Fc->F); // assegno la nuova faccia
        }

        tmpCurs->next= (Seg_PointerList) malloc (sizeof(Seg_PointerList_El));
        tmpCurs->next->sptr=&Sg->S;
        tmpCurs->next->orient='+';
        tmpCurs->next->next=NULL;


    }

    Fc->F.s = tmpHead; // nella nuova faccia assegno la lista segmenti
    Fc->F.norm_vect.x = silvia->F.norm_vect.x ; // assegno il vettore normale, che è uguale
    Fc->F.norm_vect.y = silvia->F.norm_vect.y ;
    Fc->F.norm_vect.z = silvia->F.norm_vect.z ;
    Fc->F.Sol1 = silvia->F.Sol1; // assegno i due solidi
    Fc->F.Sol2 = silvia->F.Sol2;

    //Face_add_head(newFace, Fc, &Fc); // aggiungo la nuova faccia in testa alla lista
    NUMFACCE++;

    //adesso creo la lista di face_pointerlist del nuovo segmento posto in head

    Sg->S.f = (Face_PointerList)malloc(sizeof(Face_PointerList_El));
    Sg->S.f->fptr = &(silvia->F); // al segmento nuovo associo la faccia vecchia
    Sg->S.f->next = (Face_PointerList)malloc(sizeof(Face_PointerList_El)); // alloco
    Sg->S.f->next->fptr = &(Fc->F); // assegno la faccia nuova
    Sg->S.f->next->next = NULL; //e dichiaro fine lista

    // infine modifico i solidi che puntavano alla faccia vecchia, perchè puntino sia alla vecchia che a quella nuova

    tmpFace = silvia->F.Sol1->f; // assegno l'inizio della lista di facce
    tmpFaceCurs = tmpFace; // scorrere in avanti per trovare l'orientamento
    silvia->F.Sol1->f = (Face_PointerList) malloc(sizeof(Face_PointerList_El)); // alloco per la nuova faccia
    silvia->F.Sol1->f->fptr = &(Fc->F); // assegno l'indirizzo della nuova faccia che ora è in testa
    while (tmpFaceCurs->fptr != &(silvia->F) ) // scorro la lista facce
    {
        tmpFaceCurs = tmpFaceCurs->next;
    }
    original_orient = tmpFaceCurs->orient; // salvo l'orientamento
    silvia->F.Sol1->f->orient = original_orient; // assegno lo stesso orientamento dell'altra faccia
    silvia->F.Sol1->f->next = tmpFace; // incollo il resto della lista

    if (silvia->F.Sol2 != NULL)
    {
        tmpFace = silvia->F.Sol2->f; // salvo la testa della lista
        silvia->F.Sol2->f = (Face_PointerList) malloc (sizeof(Face_PointerList_El));
        silvia->F.Sol2->f->fptr = &(Fc->F); // assegno l'indirizzo della nuova faccia
        if (original_orient == '+')
        {
            silvia->F.Sol2->f->orient = '-';
        }
        else
        {
            silvia->F.Sol2->f->orient = '+';
        }
            silvia->F.Sol2->f->next = tmpFace; // incollo il resto della lista
    }
    // fine del lavoro sui solidi

    return 0;
}

int classifica_faccia(Face_List Fp)
{
    Seg_PointerList segCurs;

    segCurs=Fp->F.s;
    while (segCurs->sptr->A->side == '0' && segCurs->sptr->B->side == '0')
    {
        segCurs = segCurs->next;
    }
    if(segCurs==NULL){
        Fp->F.side=0;
    }
    else{
        if (segCurs->sptr->A->side == '+' || segCurs->sptr->B->side == '+')
        {
            Fp->F.side=3;
        }
        else
        {
            Fp->F.side=2;
        }
    }

    return 1;
}


int slice_faces(Face_List F) // esegue il face_slicer su ogni faccia della lista F
{
    while (F != NULL)
    {
        face_slicer(F);

        F = F->next;
    }

    F=Fc;

    while (F != NULL)
    {
        classifica_faccia(F);

        F = F->next;
    }

    return 0;
}
