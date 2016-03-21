/*
    Progetto - Programmazione e Calcolo Scientifico
    Header C con i prototipi delle funzioni per le varie operazioni sulle liste
    Utilizza le #include guard OPERAZIONI_LISTE_H_INCLUDED

    Marco


*/

#ifndef OPERAZIONI_LISTE_H_INCLUDED
#define OPERAZIONI_LISTE_H_INCLUDED

// Funzioni sulla lista di punti
int Point_add_head(Point , Point_List , Point_List* );

int Point_add_after(Point , Point_List );

int Seg_add_head(Seg , Seg_List, Seg_List*);

int Face_add_head(Face, Face_List, Face_List*);

int Solid_add_head(Solid, Solid_List, Solid_List*);

int Point_Equality(Point, Point);

int Seg_Equality(Seg, Seg);

double dot_prod(Point, Point);

Point cross_prod(Point , Point);

double eu_norm(Point);

Point vett_scal(Point , double);

void shred();

#endif // OPERAZIONI_LISTE_H_INCLUDED
