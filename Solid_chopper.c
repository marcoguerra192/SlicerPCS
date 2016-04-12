/*
    Scorre sui solidi
    #include guard SOLID_CHOPPER_H_INCLUDED

*/

#include "Standard.h"
#include "Tipi.h"
#include "Operazioni_Liste.h"
#include "Strutture.h"
#include "Stampa.h"


int solid_chopper(Solid_List s, Plane_List p, unsigned long PlIndex)
{
    Face_PointerList faceCurs;
    Seg_PointerList segCurs;
    Face newFace;
    Seg_PointerList tmpListSeg, tmpListSegHead, tmpListSeg2,tmpListSeg2old, tmp,tmp2; // lista per salvare man mano che li trovo i segmenti int int, per poi ordinare!
    int adiacenti = 0, trovato_adiacenti=0 , trovato_primo_seg=0;
    Point V1,V2,V3; // punti per fare i calcoli sul verso
    double proiezione;
    char invertito;
    Face_PointerList newFaceList, newFaceListHead, oldFaceList;
    Solid newSol;

    faceCurs = s->So.f; // assegno la lista facce
    tmpListSegHead = (Seg_PointerList) malloc (sizeof(Seg_PointerList_El));
    tmpListSeg = tmpListSegHead;

    while (faceCurs != NULL &&!trovato_primo_seg) // primo caso in un ciclo while a parte
    {
        if (faceCurs->fptr->side == 2) // se è di tipo intersezione, ma solo fra quelle sotto! (per non trovarle due volte)
        {
            // devo scorrere i segmenti della faccia fino a trovarne uno di tipo int - int
            segCurs = faceCurs->fptr->s;

            while (segCurs != NULL && !(segCurs->sptr->A->side == '0' && segCurs->sptr->B->side == '0'))
            {
                segCurs = segCurs->next; // scorro avanti
            }

            if (segCurs != NULL)
            {
                trovato_primo_seg=1;
                tmpListSeg->sptr = segCurs->sptr; // salvo
                faceCurs = faceCurs->next; // scorri avanti la lista facce
            }


        }
        if(!trovato_primo_seg){
            faceCurs = faceCurs->next; // scorri avanti la lista facce
        }
    } // fine del primo caso

    while (faceCurs != NULL) // tutti gli altri casi
    {
        if (faceCurs->fptr->side == 2) // guardiamo solo quelle sotto perchè quelle sopra trovano gli stessi segmenti
        {
            // devo scorrere i segmenti della faccia fino a trovarne uno di tipo int - int
            segCurs = faceCurs->fptr->s;

            while (segCurs != NULL && !(segCurs->sptr->A->side == '0' && segCurs->sptr->B->side == '0'))
            {
                segCurs = segCurs->next; // scorro avanti
            }

            if (segCurs != NULL)
            {
                tmpListSeg->next = (Seg_PointerList) malloc (sizeof(Seg_PointerList_El)); // alloco il next
                tmpListSeg = tmpListSeg->next; // scorro avanti
                tmpListSeg->sptr = segCurs->sptr; // salvo il segmento
            }
        }
        faceCurs = faceCurs->next; // avanti con le facce
    }

    tmpListSeg->next = NULL;

    /* a questo punto ho in tmpListSegHead una lista di tutti i segmenti di tipo int int, però DISORDINATI
     devo ordinarli in modo adiacente e antiorario rispetto al vettore normale:
     Prima li ordiniamo in modo adiacente ma in verso come viene, poi facciamo il cross tra due consecutivi, e vediamo se è concorde o discorde
     alla normale alla faccia: se lo è bene, se no INVERTIAMO IL VETTORE NORMALE, e tenendone conto per decidere interni ed esterni, ovvero:
     CASO 1 NON SI CAMBIA => il solido S1 formato da facce + ha il vettore entrante, e viceversa
     CASO 2 SI INVERTE IL VETTORE => il solido fatto da facce + ha il vettore uscente, e viceversa;
    */

    tmpListSeg = tmpListSegHead;
    tmpListSeg2old=tmpListSegHead;
    tmpListSeg2 = tmpListSegHead->next;

    if (tmpListSeg2 == NULL){
        return 1;
    }

    while (tmpListSeg->next!= NULL)
    {

        while (tmpListSeg2 != NULL && !trovato_adiacenti) // qui la condizione è inutile perchè lo deve trovare sempre! se arriva a null c'è un errore
        {
            adiacenti = ((tmpListSeg2->sptr->A == tmpListSeg->sptr->A) || (tmpListSeg2->sptr->A == tmpListSeg->sptr->B) || (tmpListSeg2->sptr->B == tmpListSeg->sptr->A) || (tmpListSeg2->sptr->B == tmpListSeg->sptr->B));
            if (adiacenti)
            {
                trovato_adiacenti=1;
                // tmplistseg2 va subito dopo tmplistseg
                if(tmpListSeg2!=tmpListSeg->next){
                    tmp = tmpListSeg->next;
                    tmp2=tmpListSeg2->next;
                    tmpListSeg->next = tmpListSeg2;
                    tmpListSeg2->next=tmp;
                    tmpListSeg2old->next=tmp2;
                }

                // l'orientamento lo vediamo dopo!

                tmpListSeg = tmpListSeg->next; // sistemo i cursori nelle posizioni per il prossimo run
                if (tmpListSeg->next != NULL) // se non sono arrivato a fine lista
                {
                    tmpListSeg2 = tmpListSeg->next;
                    tmpListSeg2old=tmpListSeg;
                }

            }
            if(!trovato_adiacenti){
                    tmpListSeg2old=tmpListSeg2;
                    tmpListSeg2 = tmpListSeg2->next; // scorro avanti se non l'ha trovato
            }
        }
        trovato_adiacenti=0;

    }

    // ora abbiamo la lista coi segmenti adiacenti, ma non ancora ordinati in verso!
    tmpListSeg = tmpListSegHead;
    tmpListSeg2 = tmpListSegHead->next; // riporto i cursori al primo e secondo posto

    if (tmpListSeg->sptr->A == tmpListSeg2->sptr->A) //vedi disegno su faiscbuk!
    {
        tmpListSeg->orient = '-'; // il primo è meno
        tmpListSeg2->orient = '+'; // il secondo è +
    }
    else if (tmpListSeg->sptr->A == tmpListSeg2->sptr->B)
    {
        tmpListSeg->orient = '-'; // il primo è meno
        tmpListSeg2->orient = '-'; // il secondo è -
    }
    else if (tmpListSeg->sptr->B == tmpListSeg2->sptr->A)
    {
        tmpListSeg->orient = '+'; // il primo è +
        tmpListSeg2->orient = '+'; // il secondo è +
    }
    else if (tmpListSeg->sptr->B == tmpListSeg2->sptr->B)
    {
        tmpListSeg->orient = '+'; // il primo è +
        tmpListSeg2->orient = '-'; // il secondo è -
    }
    // ho sistemato l'ordine dei primi 2 segmenti, ora faccio tutti gli altri

    while (tmpListSeg2->next != NULL)
    {
        if (tmpListSeg2->orient == '+')
        {
            if (tmpListSeg2->sptr->B == tmpListSeg2->next->sptr->A)
            {
                // il successivo è concorde
                tmpListSeg2->next->orient = '+';
            }
            else
            {
                // il successivo è discorde
                tmpListSeg2->next->orient = '-';
            }
        }
        else /* if (tmpListSeg2->orient == '-') */
        {
            if (tmpListSeg2->sptr->A == tmpListSeg2->next->sptr->A)
            {
                // il successivo è concorde
                tmpListSeg2->next->orient = '+';
            }
            else
            {
                // il successivo è discorce
                tmpListSeg2->next->orient = '-';
            }
        }

        tmpListSeg2 = tmpListSeg2->next; // scorro avanti
    } // fine ciclo di ordinamento
    // ora ho i segmenti della mia nuova faccia, salvati adiacenti e ordinati in tmpListSegHead!
    // ora devo capire se sono orarie o antiorarie rispetto al versore uscente
    tmpListSeg = tmpListSegHead;
    // calcolo i vettori dei primi due segmenti, tenendo conto dell'ordine!
    if (tmpListSeg->orient == '+')
    {
        V1.x = tmpListSeg->sptr->B->x - tmpListSeg->sptr->A->x;
        V1.y = tmpListSeg->sptr->B->y - tmpListSeg->sptr->A->y;
        V1.z = tmpListSeg->sptr->B->z - tmpListSeg->sptr->A->z;
    }
    else
    {
        V1.x = tmpListSeg->sptr->A->x - tmpListSeg->sptr->B->x;
        V1.y = tmpListSeg->sptr->A->y - tmpListSeg->sptr->B->y;
        V1.z = tmpListSeg->sptr->A->z - tmpListSeg->sptr->B->z;
    }

    tmpListSeg = tmpListSeg->next;
    if (tmpListSeg->orient == '+')
    {
        V2.x = tmpListSeg->sptr->B->x - tmpListSeg->sptr->A->x;
        V2.y = tmpListSeg->sptr->B->y - tmpListSeg->sptr->A->y;
        V2.z = tmpListSeg->sptr->B->z - tmpListSeg->sptr->A->z;
    }
    else
    {
        V2.x = tmpListSeg->sptr->A->x - tmpListSeg->sptr->B->x;
        V2.y = tmpListSeg->sptr->A->y - tmpListSeg->sptr->B->y;
        V2.z = tmpListSeg->sptr->A->z - tmpListSeg->sptr->B->z;
    }

    //ora ho i due vettori dei primi due segmenti


    V3 = cross_prod(V1,V2); // prodotto vettore
    #ifdef DEBUG_
    if (abs(eu_norm(V3)) < tol)
    {
        fprintf(OUTPUT, "OCHO! I due vettori sono un po' troppo paralleli! \n");
    }
    #endif
    V3 = vett_scal(V3 , 1/eu_norm(V3));  // normalizzo V3

    proiezione = dot_prod(V3 , p->pl.n);

    if ( proiezione > tol ) // maggiore della tolleranza
    {
        newFace.norm_vect.x = p->pl.n.x; // copio il vettore normale del piano nella nuova faccia SENZA INVERTIRE
        newFace.norm_vect.y = p->pl.n.y;
        newFace.norm_vect.z = p->pl.n.z;
        invertito = 0;
    }
    else if (proiezione < -tol)
    {
        newFace.norm_vect.x = -(p->pl.n.x); // copio il vettore normale del piano nella nuova faccia INVERTENDOLO
        newFace.norm_vect.y = -(p->pl.n.y);
        newFace.norm_vect.z = -(p->pl.n.z);
        invertito = 1;
    }
    #ifdef DEBUG_
    else
    {
        fprintf(OUTPUT, "OCHO! I due vettori sono un po' troppo paralleli! \n");
    }
    #endif

    newFace.s = tmpListSegHead; // assegno la lista di segmenti, che ora è ordinata, adiacente e antioraria rispetto al vettore normale
    newFace.side = 0;

    // assegno i MARKER DELLA FRATTURA alla faccia nuova!

    newFace.OriginalFace = 0; // marker faccia originale
    newFace.CausingPlane = (unsigned int) 0; // marker piano di giacitura

    Face_add_head(newFace,Fc,&Fc); // aggiungo la nuova faccia in testa alla lista
    NUMFACCE++;

    //ai segmenti della faccia nuova devo aggiungere la faccia in head nella propria face_pointerlist f

    segCurs = Fc->F.s;
    while(segCurs!=NULL){
        faceCurs=segCurs->sptr->f;
        segCurs->sptr->f = (Face_PointerList) malloc(sizeof(Face_PointerList_El));
        segCurs->sptr->f->fptr= &(Fc->F);
        segCurs->sptr->f->next=faceCurs;
        segCurs=segCurs->next;
    }


    // aggiungo la nuova faccia al solido attuale CHE DECIDO ESSERE QUELLO CON I PUNTI +

    newFaceList = s->So.f; // salvo tmp la lista facce
    s->So.f = (Face_PointerList) malloc(sizeof(Face_PointerList_El)); // alloco
    s->So.f->fptr = &(Fc->F) ; //aggiungo la nuova faccia
    s->So.f->next = newFaceList; // accodo il resto della lista
    if (invertito) // conto di sovrascrivere il solido con i +
    {
        s->So.f->orient = '+'; // è uscente
    }
    else
    {
        s->So.f->orient = '-'; // è entrante
    }

    newFaceListHead = (Face_PointerList) malloc(sizeof(Face_PointerList_El)); // alloco PER LA FACCIA COI -
    newFaceList = newFaceListHead; // assegno il cursore

    newFaceList->fptr = &(Fc->F); // Copio la faccia nuova nel nuovo solido
    if (invertito)
    {
        newFaceList->orient = '-'; // è entrante
    }
    else
    {
        newFaceList->orient = '+'; // è uscente
    }
    newFaceList->next = NULL;

    Solid_add_head(newSol, Sol, &Sol); // ho aggiunto il nuovo solido in testa
    NUMSOL++;

    Fc->F.Sol1 = &(s->So); // sistemo i puntatori a solido della faccia nuova
    Fc->F.Sol2 = &(Sol->So);

    // ora devo dividere la lista originale in due a seconda che siano + o -
    oldFaceList = s->So.f; // assegno la prima vecchia faccia
    while (oldFaceList->next != NULL) // finchè non arrivo all'ultimo prima di NULL
    {
        if (oldFaceList->next->fptr->side == 2 || oldFaceList->next->fptr->side == 1) // se la faccia è - o - intersezione devo spostarla nella lista nuova
        {
            newFaceList->next = (Face_PointerList) malloc(sizeof(Face_PointerList_El));
            newFaceList->next = oldFaceList->next;
            newFaceList = newFaceList->next; // scorro avanti la nuova lista
            oldFaceList->next = oldFaceList->next->next; // cucio le due estremità della lista
            newFaceList->next = NULL; // termino la lista nuova
            // ora vado a modificare i puntatori a solido della faccia che ho spostato nella lista nuova
            if (newFaceList->fptr->Sol1 == &(s->So)) // se era salvato come il primo solido modifico questo
            {
                newFaceList->fptr->Sol1 = &(Sol->So);
            }
            else // se invece era Sol2 il solido che sto modificando, allora modifico quello (non faccio altri controlli)
            {
                newFaceList->fptr->Sol2 = &(Sol->So);
            }

        } else
            {
                oldFaceList = oldFaceList->next;
            }
    }

    // adesso ho le due liste divise per facce di tipo sx e dx,con la faccia nuova in comune

    // assegno la lista - al nuovo solido
    Sol->So.f = newFaceListHead;

    return 0;
}


int chop_solids(Solid_List S, Plane_List p)
{
    unsigned long ind = 1; // Indice dei piani che tagliano

    while (S != NULL)
    {
        solid_chopper(S,p,ind);
        ind++;
        S = S->next;
    }

    return 0;
}
