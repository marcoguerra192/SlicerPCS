/*
    Progetto - Programmazione e Calcolo Scientifico
    HEADER C per la definizione dei TIPI di dato aggregati
    Utilizza la #include guard TIPI_H_INCLUDED  !!
    Comprende: Punto, Lista di Punti, Segmento, Lista di Segmenti, Faccia, Lista di Facce,
    Solido, Lista di Solidi.

    Marco
*/

#ifndef TIPI_H_INCLUDED // Guardie d'inclusione
#define TIPI_H_INCLUDED

// TIPI VARI


// Tipi Geometrici e Liste di questi

// PUNTO
typedef struct point {  // Definizione del tipo di dato Punto
    double x;
    double y; // coordinate
    double z;
    char side; // Vale '-' se sta sotto il piano, '+' se sta sopra il piano, '0' se entro la tolleranza dal piano
} Point;

// Definizione LISTA di PUNTI
typedef struct point_list_el { // definizione di un membro della lista
    Point P; // Un punto, NON un puntatore!
    struct point_list_el* next; // puntatore al prossimo membro della lista
} Point_List_El;
/* IMPORTANTE L'ultimo membro della lista di punti deve avere l'elemento next che punta a NULL, così che si possa
capire quando la lista è finita! */

typedef Point_List_El* Point_List;
/* NB: Il tipo Point_List è già un tipo puntatore ad un membro della lista. Per usare la lista bisogna
dichiarare variabili di questo tipo, NON Point_List*, se no c'è un puntatore di troppo. */

                          //FINE PUNTI


// SEGMENTI
typedef struct seg {  // Definizione del tipo di dato Segmento
    Point* A;    // descritto da due puntatori a due punti
    Point* B;
    struct face_pointerlist_el* f; // Lista di facce che condividono quel segmento
} Seg;

// Definizione LISTA di SEGMENTI
typedef struct seg_list_el { // definizione di un membro della lista
    Seg S; // Un segmento
    struct seg_list_el* next; // Puntatore al prossimo membro della lista
} Seg_List_El;
/* IMPORTANTE L'ultimo membro della lista di segmenti deve avere l'elemento next che punta a NULL, così che si possa
capire quando la lista è finita! */

typedef Seg_List_El* Seg_List;
/* NB: Il tipo Seg_List è già un tipo puntatore ad un membro della lista. Per usare la lista bisogna
dichiarare variabili di questo tipo, NON Seg_List*, se no c'è un puntatore di troppo. */

                        // FINE SEGMENTI


// FACCE  *** ANDRA' MODIFICATO IL TIPO FACCE COSI'DA AGGIUNGERE I MARKER PER LE NUOVE INFORMAZIONI ***
typedef struct face {  // Definizione del tipo di dato Faccia
struct seg_pointerlist_el* s; // Lista di segmenti. L'ultimo elemento punta a NULL, per capire quando la lista finisce
                      // Contiene già la lista di char per l'orientamento, '+' concorde, '-' discorde.
   short side; /* Se la faccia è sx vale 0, se è sx-intersezione vale 1, se dx-intersezione 2, se dx 3
                 TEMPORANEAMENTE può valere come FLAG se la faccia contiene segmenti spezzati e quindi è da analizzare!!  */
   Point norm_vect; // Vettore normale alla faccia (campo "char" non utilizzato)
   struct solid* Sol1;   //  Puntatori ai due solidi (eventualmente uno NULL) che sono delimitati da quella faccia
   struct solid* Sol2;

   unsigned int OriginalFace; // Marker che indica se la faccia è figlia di una delle facce originali, e se sì quale da 1 a n.
                              // Altrimenti 0 per indicare una faccia interna al dominio.
   unsigned long CausingPlane; // Marker che indica quale piano ha originato la faccia, ovvero su quale frattura il piano giace, da 1 a n.
                               // Altrimenti 0 per indicare che la faccia discende da una di quelle originali.

} Face;

// Definizione LISTA di FACCE
typedef struct face_list_el { // Definizione di un membro della lista
    Face F; // Una faccia
    struct face_list_el* next; // Puntatore al prossimo membro della lista
} Face_List_El;
/*IMPORTANTE L'elemento next dell'ultimo membro della lista di facce deve puntare a NULL, come sopra */

typedef Face_List_El* Face_List;
/* NB: Il tipo Face_List è già un tipo puntatore ad un membro della lista. Per usare la lista bisogna
dichiarare variabili di questo tipo, NON Face_List*, se no come prima c'è un puntatore di troppo. */

                                     // FINE FACCE

// SOLIDI

typedef struct solid {  // Definizione del tipo di dato Solido
    struct face_pointerlist_el* f; // Lista di facce (definita più giù). L'ultimo elemento punta a NULL, per capire quando la lista finisce!
                        // Contiene già la di char che descrive l'orientamento del versore normale alla faccia: 'E' x esterno, 'I' x interno
} Solid;

// Definizione LISTA di SOLIDI
typedef struct solid_list_el{  // Definizione di un membro della lista
    Solid So; // Un solido
    struct solid_list_el* next; //Puntatore al prossimo membro della lista
} Solid_List_El;
/*IMPORTANTE L'elemento next dell'ultimo membro della lista di facce deve puntare a NULL, come sopra */

typedef Solid_List_El* Solid_List;
/* NB: Il tipo Solid_List è già un tipo puntatore ad un membro della lista. Per usare la lista bisogna
dichiarare variabili di questo tipo, NON Solid_List*, se no come prima c'è un puntatore di troppo. */

                                     // FINE SOLIDI

// DEFINIZIONE PIANI
typedef struct plane { // tipo piano
    Point pt; // punto
    Point n; // vettore normale
} Plane;

typedef struct plane_list_el { // membro della lista
    Plane pl; // un piano
    struct plane_list_el* next; // puntatore al prox
} Plane_List_El;

typedef Plane_List_El* Plane_List; // Tipo lista di piani

                                        // FINE SOLIDI

// SUB LISTE

typedef struct face_pointerlist_el {
    Face* fptr; // puntatore a faccia
    char orient; // Per l'orientamento della faccia + uscente, - entrante
    struct face_pointerlist_el* next;
} Face_PointerList_El; // elemento della lista di PUNTATORI  a facce.

typedef Face_PointerList_El* Face_PointerList;

typedef struct seg_pointerlist_el {
    Seg* sptr; // puntatore a segmento
    char orient; // Per l'orientamento del seg, '+' concorde, '-' discorde
    struct seg_pointerlist_el* next;
} Seg_PointerList_El; // elemento della lista di puntatori a segmenti

typedef Seg_PointerList_El* Seg_PointerList;

#endif // TIPI_H_INCLUDED
