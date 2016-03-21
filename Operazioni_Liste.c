/*
    Progetto - Programmazione e Calcolo Scientifico
    FILE SORGENTE C con la definizione di funzioni per varie operazioni sulle liste
    Riferisce all'Header Operazioni_Liste.h

    Marco

*/

#include "Standard.h" // Inclusione librerie Standard

#include "Tipi.h" // Necessita dei tipi

// AGGIUNTA DI ELEMENTI ALLA LISTA DI PUNTI

int Point_add_head(Point newPoint, Point_List headPoint, Point_List* newHead)
/*
    Funzione che aggiunge un nuovo elemento IN TESTA alla lista
    Il primo argomento è il punto da aggiungere alla lista,
    il secondo un puntatore al primo elemento della lista.
    Il terzo è un parametro per riferimento, in cui andare a scrivere l'indirizzo della nuova testa della lista
    Ritorna 0 se tutto okay, -1 se qualcosa va storto
*/
{
    Point_List newEl;
    newEl = (Point_List) malloc(sizeof(Point_List_El)); // Alloca un nuovo elemento
    if (newEl == NULL) // Se ha fallito l'allocazione ESCI
    {
        #ifdef DEBUG_
        printf("Impossibile allocare! Uscita \n");
        #endif
        return -1;
    }
    newEl->P.x = newPoint.x; //copia i dati
    newEl->P.y = newPoint.y;
    newEl->P.z = newPoint.z;
    newEl->P.side = newPoint.side;
    newEl->next = headPoint; // accoda la vecchia testa della lista al nuovo punto

    *newHead = newEl; // sostituisce il nuovo indirizzo in testa alla lista

    return 0;
}

/*int Point_add_after(Point newPoint , Point_List afterEl)

    FUNZIONE che aggiunge un nuovo punto newPoint nella lista, DOPO l'elemento afterEl
    Il primo argomento è il punto da aggiungere alla lista,
    il secondo è un puntatore all'elemento precedente a cui concatenare il nuovo.
    Ritorna 0 se tutto okay, -1 se succede qualche casino.

{
    Point_List newEl;
    newEl = (Point_List) malloc(sizeof(Point_List_El)); // Alloca un nuovo elemento
    if (newEl == NULL) // Se ha fallito l'allocazione ESCI
    {
        #ifdef DEBUG_
        printf("Impossibile allocare! Uscita \n");
        #endif
        return -1;
    }
    newEl->P.x = newPoint.x;
    newEl->P.y = newPoint.y;
    newEl->P.z = newPoint.z;
    newEl->P.S1 = newPoint.S1;
    newEl->P.S2 = newPoint.S2;
    newEl->P.side = newPoint.side;

    newEl->next = afterEl->next; //concatena i puntatori per far posto al nuovo elemento
    afterEl->next = newEl;

    return 0;
}*/

// AGGIUNTA DI ELEMENTI ALLA LISTA DI SEGMENTI

int Seg_add_head(Seg newSeg, Seg_List headSeg, Seg_List* newHead)
/*
    Funzione che aggiunge un nuovo elemento IN TESTA alla lista
    Il primo argomento è il segmento da aggiungere alla lista,
    il secondo un puntatore al primo elemento della lista.
    Il terzo è un parametro per riferimento, in cui andare a scrivere l'indirizzo della nuova testa della lista
    Ritorna 0 se tutto okay, -1 se qualcosa va storto
*/
{
    Seg_List newEl;
    newEl = (Seg_List) malloc(sizeof(Seg_List_El)); // Alloca un nuovo elemento
    if (newEl == NULL) // Se ha fallito l'allocazione ESCI
    {
        #ifdef DEBUG_
        printf("Impossibile allocare! Uscita \n");
        #endif
        return -1;
    }
    newEl->S.A = newSeg.A;
    newEl->S.B = newSeg.B;
    newEl->S.f = newSeg.f;
    newEl->next = headSeg; // accoda la vecchia testa della lista al nuovo punto

    *newHead = newEl;
    return 0;
}

int Face_add_head(Face newFace, Face_List headFace, Face_List* newHead)
/*Funzione che prende una nuova faccia e la incolla in testa alla lista delle facce */
{
    Face_List newEl;
    newEl = (Face_List)malloc(sizeof(Face_List_El)); // alloco
    if (newEl == NULL) // Se ha fallito l'allocazione ESCI
    {
        #ifdef DEBUG_
        printf("Impossibile allocare! Uscita \n");
        #endif
        return -1;
    }
    newEl->F.s = newFace.s;
    newEl->F.norm_vect.x = newFace.norm_vect.x;
    newEl->F.norm_vect.y = newFace.norm_vect.y;
    newEl->F.norm_vect.z = newFace.norm_vect.z;
    newEl->F.side = newFace.side; // copio il side
    newEl->F.Sol1 = newFace.Sol1; // copio i solidi
    newEl->F.Sol2 = newFace.Sol2;
    newEl->next = headFace; //incollo la vecchia testa dopo questa qua
    *newHead = newEl; // assegno la nuova testa della lista
    return 0;
}

int Solid_add_head(Solid newSolid, Solid_List headSolid, Solid_List* newHead)
{
    Solid_List newEl;
    newEl = (Solid_List)malloc(sizeof(Solid_List_El)); // alloco
    if (newEl == NULL) // Se ha fallito l'allocazione ESCI
    {
        #ifdef DEBUG_
        printf("Impossibile allocare! Uscita \n");
        #endif
        return -1;
    }
    newEl->So.f = newSolid.f;
    newEl->next = headSolid;
    *newHead = newEl;
    return 0;
}

int Point_Equality(Point A, Point B) // Funzione che torna 1 se i due punti sono uguali, 0 altrimenti
{
    if (A.x == B.x && A.y == B.y && A.z == B.z)
    {
        return 1;
    }
    else return 0;
}

int Seg_Equality(Seg S1, Seg S2)
/* Ritorna 0 se segmenti diversi, 1 se uguali e nello stesso verso, -1 se uguali ma in verso opposto */
{
    if ( (Point_Equality(*(S1.A), *(S2.A)) == 1) && (Point_Equality(*(S1.B),*(S2.B)) == 1 ))
    {
        return 1;
    }
    else if (Point_Equality(*(S1.A), *(S2.B)) == 1 && (Point_Equality(*(S1.B),*(S2.A)) == 1 ))
    {
        return -1;
    }
    else
        return 0;
}

double dot_prod(Point A, Point B) // prodotto scalare tra due pt intesi come vettori di uno spazio NON AFFINE
{
    return A.x*B.x + A.y*B.y + A.z*B.z;
}

Point cross_prod(Point A, Point B) // prodotto vettore tra due pt intesi come vettori di uno spazio NON AFFINE
{
    Point C;

    C.x = A.y * B.z - A.z * B.y;
    C.y = A.z * B.x - A.x * B.z;
    C.z = A.x * B.y - A.y * B.x;
    return C;
}

double eu_norm(Point p)
{
    return sqrt( p.x*p.x + p.y*p.y + p.z*p.z );
}

Point vett_scal(Point P, double l)
{
    Point A;

    A.x = l * P.x;
    A.y = l * P.y;
    A.z = l * P.z;

    return A;
}
