/*
    Funzione per la stampa del risultato
*/

#include "Standard.h"
#include "Strutture.h"
#include "Tipi.h"

//prototipi
int stampa_punto(Point*, FILE*);
int stampa_seg(Seg*, FILE*);
int stampa_faccia(Face*, FILE*);
int stampa_sol(Solid*, FILE*);
void spaced_output(FILE*, long, long);
void spaced_output_char(FILE*, long, char, long);
long Max(long, long);

int Stampa()
{
    FILE* file_stream;
    Seg_List SgCurs;


    file_stream = fopen(RESULT,"w");

    if (file_stream == NULL)
    {
        fprintf(OUTPUT,"Stampa: Non posso scrivere sul file\n");
        return -1;
    }

    fprintf(file_stream, "hold on \n");
    fprintf(file_stream, "title('Risultato'); \n");
    fprintf(file_stream, "xlabel('x'); \n");
    fprintf(file_stream, "ylabel('y'); \n");
    fprintf(file_stream, "zlabel('z'); \n");
    fprintf(file_stream, "view(-37.5, 30); \n");

    SgCurs = Sg;

    while (SgCurs != NULL)
    {
        fprintf(file_stream,"plot3([ %lf , %lf ], [ %lf , %lf ], [ %lf , %lf ]); \n",SgCurs->S.A->x,SgCurs->S.B->x, SgCurs->S.A->y, SgCurs->S.B->y, SgCurs->S.A->z, SgCurs->S.B->z);

        SgCurs = SgCurs->next;
    }

    fclose(file_stream);

    system(PLOT_RESULT_COMM);


    return 0;
}


int stampa_punto(Point* P, FILE* output_stream)
{
    fprintf(output_stream, "%f %f %f  ", P->x, P->y, P->z );

    return 0;
}

int stampa_seg(Seg* S, FILE* output_stream)
{
    stampa_punto(S->A,output_stream);
    fprintf(output_stream, "<->  ");
    stampa_punto(S->B,output_stream);

    return 0;
}

int stampa_faccia(Face* F, FILE* output_stream)
{
    Seg_PointerList SgPtr;

    SgPtr = F->s;
    while (SgPtr != NULL)
        {
            stampa_seg(SgPtr->sptr, output_stream);
            fprintf(output_stream, " %c \n", SgPtr->orient);
            SgPtr = SgPtr -> next;
        }
    return 0;
}

int stampa_sol(Solid* S, FILE* output_stream)
{
    Face_PointerList FcPtr;
    int j = 1;

    FcPtr = S->f;
    while (FcPtr != NULL)
    {
        fprintf(output_stream,"  %d^ Faccia:\n", j);
        stampa_faccia(FcPtr->fptr, output_stream);
        fprintf(output_stream,"\n");
        j++;
        FcPtr = FcPtr->next;
    }

    return 0;
}

void print_all(char dest) // parametro: 0 per stampa su console, 1 su file
{
    Point_List cursPoint;
    Seg_List cursSeg;
    Face_PointerList cursf;
    Face_List cursface;
    Solid_List cursSolid;
    Seg_PointerList cursSegpointer;
    Face_PointerList cursFacepointer;

    FILE* DEST;
    if (dest == 0)
    {
        DEST = stdout;
    }
    else
    {
        DEST = fopen("Risultato.txt","w");
    }

    int k;
    cursPoint = Pt;
    k=1;
    fprintf(DEST,"\n\nPunti\n");
    while(cursPoint!=NULL){
            fprintf(DEST,"%d) %f %f %f %c \n",k++, cursPoint->P.x, cursPoint->P.y, cursPoint->P.z, cursPoint->P.side);
            cursPoint=cursPoint->next;
    }
    cursSeg=Sg;
    k=1;
    fprintf(DEST,"\n\n Segmenti\n");
    while(cursSeg!=NULL){
            fprintf(DEST,"%d) %f %f %f -> %f %f %f ",k++, cursSeg->S.A->x, cursSeg->S.A->y, cursSeg->S.A->z, cursSeg->S.B->x, cursSeg->S.B->y, cursSeg->S.B->z);
            cursf=cursSeg->S.f;
            fprintf(DEST," facce del segmento: ");
            while(cursf!=NULL){
                fprintf(DEST,"%p ",cursf->fptr);
                cursf=cursf->next;
            }
            fprintf(DEST,"\n");
            cursSeg=cursSeg->next;
    }
    cursface=Fc;
    k=1;
    fprintf(DEST,"\n\n Facce\n");
    while(cursface!=NULL){
            fprintf(DEST,"%d) %p %d sol1:%p sol2:%p " ,k++, &(cursface->F), cursface->F.side, cursface->F.Sol1, cursface->F.Sol2);
            cursSegpointer=cursface->F.s;
            fprintf(DEST,"segmenti della faccia: \n");
            while(cursSegpointer!=NULL){
                if (cursSegpointer->orient=='+'){
                    fprintf(DEST,"%f %f %f\n",cursSegpointer->sptr->A->x,cursSegpointer->sptr->A->y,cursSegpointer->sptr->A->z);
                    fprintf(DEST,"%f %f %f\n",cursSegpointer->sptr->B->x,cursSegpointer->sptr->B->y,cursSegpointer->sptr->B->z); }
                else
                {
                    fprintf(DEST,"%f %f %f\n",cursSegpointer->sptr->B->x,cursSegpointer->sptr->B->y,cursSegpointer->sptr->B->z);
                    fprintf(DEST,"%f %f %f\n",cursSegpointer->sptr->A->x,cursSegpointer->sptr->A->y,cursSegpointer->sptr->A->z);
                }
                cursSegpointer=cursSegpointer->next;
            }
            fprintf(DEST,"\n");
            cursface=cursface->next;
    }
    fprintf(DEST,"\n\nSolidi\n");
    k=1;
    cursSolid=Sol;
    while(cursSolid!=NULL){
            fprintf(DEST,"%d) %p ",k++, &cursSolid->So);
            cursFacepointer=cursSolid->So.f;
            fprintf(DEST,"facce del solido:\n");
            while(cursFacepointer!=NULL){
                    fprintf(DEST,"%p%c ",cursFacepointer->fptr, cursFacepointer->orient);
                    cursFacepointer=cursFacepointer->next;
            }
            fprintf(DEST,"\n");
            cursSolid=cursSolid->next;
            }
    }

void new_print_all(char dest) // parametro: 0 per stampa su console, 1 su file
{
    Point_List cursPoint;
    Seg_List cursSeg;
    Face_PointerList cursf;
    Face_List cursface;
    Solid_List cursSolid;
    Seg_PointerList cursSegpointer;
    Face_PointerList cursFacepointer;
    int i = 0;

    FILE* DEST;
    if (dest == 0)
    {
        DEST = stdout;
    }
    else
    {
        DEST = fopen("Risultato.txt","w");
    }

    cursPoint = Pt;
    fprintf(DEST,"Punti\n");
    while(cursPoint!=NULL)
    {
            fprintf(DEST,"%p - %f %f %f \n", &(cursPoint->P), cursPoint->P.x, cursPoint->P.y, cursPoint->P.z);
            cursPoint=cursPoint->next;
    }
    cursSeg=Sg;
    fprintf(DEST,"\nSegmenti\n");
    while(cursSeg!=NULL){
            fprintf(DEST,"%p - %p -> %p ", &(cursSeg->S), cursSeg->S.A, cursSeg->S.B);
            cursf=cursSeg->S.f;
            fprintf(DEST," facce: ");
            while(cursf!=NULL){
                fprintf(DEST,"%p ",cursf->fptr);
                cursf=cursf->next;
            }
            fprintf(DEST,"\n");
            cursSeg=cursSeg->next;
    }
    cursface=Fc;
    fprintf(DEST,"\nFacce\n");
    while(cursface!=NULL){
            fprintf(DEST,"%p sol1:%p sol2:%p ", &(cursface->F), cursface->F.Sol1, cursface->F.Sol2);
            cursSegpointer=cursface->F.s;
            fprintf(DEST,"segmenti: ");
            while(cursSegpointer!=NULL){

                fprintf(DEST,"%p %c ", cursSegpointer->sptr , cursSegpointer->orient );
                cursSegpointer=cursSegpointer->next;
            }
            fprintf(DEST, "OriginalFace: %d - CuttingPlane: %ld ", cursface->F.OriginalFace, cursface->F.CausingPlane);
            fprintf(DEST,"\n");
            cursface=cursface->next;
    }
    fprintf(DEST,"\nSolidi\n");
    cursSolid=Sol;
    while(cursSolid!=NULL){
            fprintf(DEST,"%p ", &(cursSolid->So) );
            cursFacepointer=cursSolid->So.f;
            fprintf(DEST,"facce: ");
            while(cursFacepointer!=NULL){
                    fprintf(DEST,"%p %c ",cursFacepointer->fptr, cursFacepointer->orient);
                    cursFacepointer=cursFacepointer->next;
            }
            fprintf(DEST,"\n");
            cursSolid=cursSolid->next;
            }


    fprintf(DEST,"\nFacce derivate dalle originali:\n");
    for (i=0 ; i < NUMFACCE_ORIG ; i++)
    {
        cursf = FigliOriginali[i];
        fprintf(DEST, "Marker %d - Facce :",i+1);
        while (cursf != NULL)
        {
            fprintf(DEST, " %p" , cursf->fptr);
            cursf = cursf->next;
        }
        fprintf(DEST, "\n");
    }

    fprintf(DEST,"\nFacce generate dal piano k-esimo:\n");
    for (i=0 ; i < NUMPIANI ; i++)
    {
        cursf = GeneratiFrattura[i];
        fprintf(DEST, "Frattura %d - Facce :",i+1);
        while (cursf != NULL)
        {
            fprintf(DEST, " %p" , cursf->fptr);
            cursf = cursf->next;
        }
        fprintf(DEST, "\n");
    }

    }

    void print_all_with_indexes(char dest) // parametro: 0 per stampa su console, 1 su file
{
    Point_List cursPoint;
    Seg_List cursSeg;
    Face_PointerList cursf;
    Face_List cursface;
    Solid_List cursSolid;
    Seg_PointerList cursSegpointer;
    Face_PointerList cursFacepointer;
    int i = 0;

    FILE* DEST;
    if (dest == 0)
    {
        DEST = stdout;
    }
    else
    {
        DEST = fopen("Risultato.txt","w");
    }

    cursPoint = Pt;
    // fprintf(DEST,"Punti\n");
    while(cursPoint!=NULL)
    {
            fprintf(DEST,"%ld %f %f %f \n", cursPoint->P.indice, cursPoint->P.x, cursPoint->P.y, cursPoint->P.z);
            cursPoint=cursPoint->next;
    }
    cursSeg=Sg;
    //fprintf(DEST,"\nSegmenti\n");
    fprintf(DEST,"!\n");
    while(cursSeg!=NULL){
            fprintf(DEST,"%ld - %ld -> %ld ", cursSeg->S.indice, cursSeg->S.A->indice, cursSeg->S.B->indice);
            cursf=cursSeg->S.f;
            fprintf(DEST," facce: ");
            while(cursf!=NULL){
                fprintf(DEST,"%ld ",cursf->fptr->indice);
                cursf=cursf->next;
            }
            fprintf(DEST,"\n");
            cursSeg=cursSeg->next;
    }
    cursface=Fc;
    fprintf(DEST,"\nFacce\n");
    while(cursface!=NULL){
            if (cursface->F.Sol2 != NULL)
            {
                fprintf(DEST,"%ld sol1: %ld sol2: %ld ", cursface->F.indice, cursface->F.Sol1->indice, cursface->F.Sol2->indice);
            }
            else
            {
                fprintf(DEST,"%ld sol1: %ld sol2: (nil) ", cursface->F.indice, cursface->F.Sol1->indice);
            }
            cursSegpointer=cursface->F.s;
            fprintf(DEST,"segmenti: ");
            while(cursSegpointer!=NULL){

                fprintf(DEST,"%ld %c ", cursSegpointer->sptr->indice , cursSegpointer->orient );
                cursSegpointer=cursSegpointer->next;
            }
            fprintf(DEST, "OriginalFace: %d - CuttingPlane: %ld ", cursface->F.OriginalFace, cursface->F.CausingPlane);
            fprintf(DEST,"\n");
            cursface=cursface->next;
    }
    fprintf(DEST,"\nSolidi\n");
    cursSolid=Sol;
    while(cursSolid!=NULL){
            fprintf(DEST,"%ld ", cursSolid->So.indice );
            cursFacepointer=cursSolid->So.f;
            fprintf(DEST,"facce: ");
            while(cursFacepointer!=NULL){
                    fprintf(DEST,"%ld %c ",cursFacepointer->fptr->indice, cursFacepointer->orient);
                    cursFacepointer=cursFacepointer->next;
            }
            fprintf(DEST,"\n");
            cursSolid=cursSolid->next;
            }


    fprintf(DEST,"\nFacce derivate dalle originali:\n");
    for (i=0 ; i < NUMFACCE_ORIG ; i++)
    {
        cursf = FigliOriginali[i];
        fprintf(DEST, "Marker %d - Facce :",i+1);
        while (cursf != NULL)
        {
            fprintf(DEST, " %ld" , cursf->fptr->indice);
            cursf = cursf->next;
        }
        fprintf(DEST, "\n");
    }

    fprintf(DEST,"\nFacce generate dal piano k-esimo:\n");
    for (i=0 ; i < NUMPIANI ; i++)
    {
        cursf = GeneratiFrattura[i];
        fprintf(DEST, "Frattura %d - Facce :",i+1);
        while (cursf != NULL)
        {
            fprintf(DEST, " %ld" , cursf->fptr->indice);
            cursf = cursf->next;
        }
        fprintf(DEST, "\n");
    }

    }

    void print_neat_with_indexes(char dest)
    /* parametro: 0 per stampa su console, 1 su file
       Stampa in modo HUMAN-READABLE */
{
    Point_List cursPoint;
    Seg_List cursSeg;
    Face_PointerList cursf;
    Face_List cursface;
    Solid_List cursSolid;
    Seg_PointerList cursSegpointer;
    Face_PointerList cursFacepointer;
    int i = 0;

    FILE* DEST;
    if (dest == 0)
    {
        DEST = stdout;
    }
    else
    {
        DEST = fopen("Risultato.txt","w");
    }

    cursPoint = Pt;
    fprintf(DEST,"** -------- Punti ( %ld ) ---------------------- ** \n", NUMPUNTI);
    while(cursPoint!=NULL)
    {
            spaced_output(DEST, cursPoint->P.indice , NUMPUNTI);
            fprintf(DEST, " - %f %f %f \n", cursPoint->P.x, cursPoint->P.y, cursPoint->P.z);
            cursPoint=cursPoint->next;
    }
    cursSeg=Sg;
    fprintf(DEST,"\n** -------- Segmenti ( %ld ) ------------------- ** \n", NUMSEG);
    while(cursSeg!=NULL)
    {
            spaced_output(DEST, cursSeg->S.indice, NUMSEG);
            fprintf(DEST," - ");
            spaced_output(DEST, cursSeg->S.A->indice, NUMPUNTI);
            fprintf(DEST," -> ");
            spaced_output(DEST, cursSeg->S.B->indice, NUMPUNTI);
            cursf=cursSeg->S.f;
            fprintf(DEST," | Facce: ");
            while(cursf!=NULL)
            {
                spaced_output(DEST, cursf->fptr->indice, NUMFACCE);
                fprintf(DEST," ");
                cursf=cursf->next;
            }
            fprintf(DEST,"\n");
            cursSeg=cursSeg->next;
    }
    cursface=Fc;
    fprintf(DEST,"\n** -------- Facce ( %ld ) --------------------- ** \n",NUMFACCE);
    while(cursface!=NULL)
    {
            spaced_output(DEST,cursface->F.indice,NUMFACCE);
            fprintf(DEST," Sol1: ");
            spaced_output(DEST,cursface->F.Sol1->indice, NUMSOL ); // nel caso di (nil) uso 4 caratteri!
            fprintf(DEST," Sol2: ");
            if (cursface->F.Sol2 != NULL)
            {
                spaced_output(DEST,cursface->F.Sol2->indice, Max(NUMSOL,40000)); // nel caso di (nil) uso 4 caratteri!
            }
            else
            {
                fprintf(DEST,"(nil)");
            }
            cursSegpointer=cursface->F.s;
            fprintf(DEST," | Segmenti: ");
            while(cursSegpointer!=NULL)
            {
                spaced_output_char(DEST, cursSegpointer->sptr->indice, cursSegpointer->orient, NUMSEG);
                fprintf(DEST," ");
                cursSegpointer=cursSegpointer->next;
            }
            fprintf(DEST, "OriginalFace: ");
            spaced_output(DEST, cursface->F.OriginalFace, NUMFACCE_ORIG);
            fprintf(DEST, " - CuttingPlane: ");
            spaced_output(DEST, cursface->F.CausingPlane, NUMPIANI);
            fprintf(DEST,"\n");
            cursface=cursface->next;
    }
    fprintf(DEST,"\n** -------- Solidi ( %ld ) -------------------- ** \n",NUMSOL);
    cursSolid=Sol;
    while(cursSolid!=NULL){
            spaced_output(DEST, cursSolid->So.indice, NUMSOL);
            fprintf(DEST," - Facce: ");
            cursFacepointer=cursSolid->So.f;
            while(cursFacepointer!=NULL){
                    spaced_output_char(DEST, cursFacepointer->fptr->indice,cursFacepointer->orient, NUMFACCE);
                    fprintf(DEST," ");
                    cursFacepointer=cursFacepointer->next;
            }
            fprintf(DEST,"\n");
            cursSolid=cursSolid->next;
            }


    fprintf(DEST,"\n** ---- Facce derivate dalle originali: ---- **\n");
    for (i=0 ; i < NUMFACCE_ORIG ; i++)
    {
        cursf = FigliOriginali[i];
        fprintf(DEST, "Marker ");
        spaced_output(DEST, (long) i+1 , NUMFACCE_ORIG);
        fprintf(DEST," - Facce : ");
        while (cursf != NULL)
        {
            spaced_output(DEST, cursf->fptr->indice, NUMFACCE);
            fprintf(DEST, " ");
            cursf = cursf->next;
        }
        fprintf(DEST, "\n");
    }

    fprintf(DEST,"\n** ---- Facce generate dalla frattura k-esima: ---- **\n");
    for (i=0 ; i < NUMPIANI ; i++)
    {
        cursf = GeneratiFrattura[i];
        fprintf(DEST, "Frattura ");
        spaced_output(DEST, (long) i+1 , NUMPIANI);
        fprintf(DEST, " - Facce : ");

        while (cursf != NULL)
        {
            spaced_output(DEST, cursf->fptr->indice, NUMFACCE);
            fprintf(DEST, " " );
            cursf = cursf->next;
        }
        fprintf(DEST, "\n");
    }

    }

    void controllo_errori(){

        Seg_List segCurs;
        Face_PointerList facePointerCurs;
        Seg_PointerList segPointerCurs;

        Face_List faceCurs;

        Point *precedente, *primo;



        segCurs= Sg;
        while(segCurs!=NULL){
            facePointerCurs= segCurs->S.f;
            while(facePointerCurs!= NULL){
                segPointerCurs = facePointerCurs->fptr->s;
                while(segPointerCurs!=NULL && (segPointerCurs->sptr!=&segCurs->S) ){
                    segPointerCurs=segPointerCurs->next;
                }
                if (segPointerCurs==NULL){
                    printf("\nERRORE: la lista facce e la lista segmenti non sono d'accordo\n");
                    printf("\nsegmento:%f %f %f -> %f %f %f  faccia:%p \n",segCurs->S.A->x,segCurs->S.A->y,segCurs->S.A->z,segCurs->S.B->x,segCurs->S.B->y,segCurs->S.B->z, facePointerCurs->fptr);
                }
                facePointerCurs=facePointerCurs->next;
            }
            segCurs=segCurs->next;
        }

        faceCurs=Fc;
        while(faceCurs!=NULL){
            segPointerCurs=faceCurs->F.s;
            while(segPointerCurs!=NULL){
                facePointerCurs=segPointerCurs->sptr->f;
                while(facePointerCurs!=NULL && facePointerCurs->fptr!=&faceCurs->F){
                    facePointerCurs=facePointerCurs->next;
                }
                if(facePointerCurs==NULL){
                    printf("\nERRORE: la lista facce e la lista segmenti non sono d'accordo\n");
                    printf("\nsegmento:%f %f %f -> %f %f %f  faccia:%p \n",segPointerCurs->sptr->A->x,segPointerCurs->sptr->A->y,segPointerCurs->sptr->A->z,segPointerCurs->sptr->B->x,segPointerCurs->sptr->B->y,segPointerCurs->sptr->B->z,&faceCurs->F);
                }
                segPointerCurs=segPointerCurs->next;
            }
            faceCurs=faceCurs->next;

        }

        faceCurs=Fc;
        while(faceCurs!=NULL){
            facePointerCurs=faceCurs->F.Sol1->f;
            while(facePointerCurs!=NULL && facePointerCurs->fptr!=&faceCurs->F){
                facePointerCurs=facePointerCurs->next;
            }
            if(facePointerCurs==NULL){
                printf("ERRORE:la lista facce e la lista solidi non sono d'accordo\n");
                printf("\nfaccia:%p solido:%p",&faceCurs->F,faceCurs->F.Sol1);
            }

            if(faceCurs->F.Sol2!=NULL){
                facePointerCurs=faceCurs->F.Sol2->f;
                while(facePointerCurs!=NULL && facePointerCurs->fptr!=&faceCurs->F){
                    facePointerCurs=facePointerCurs->next;
                }
                if(facePointerCurs==NULL){
                    printf("ERRORE:la lista facce e la lista solidi non sono d'accordo\n");
                    printf("\nfaccia:%p solido:%p",&faceCurs->F,faceCurs->F.Sol2);
                }
            }
            faceCurs=faceCurs->next;
        }

        faceCurs=Fc;
        while(faceCurs!=NULL){

            segPointerCurs=faceCurs->F.s;
            if (segPointerCurs->orient=='+')
            {
               primo=segPointerCurs->sptr->A;
               precedente=segPointerCurs->sptr->B;
            }
            else
            {
                primo=segPointerCurs->sptr->B;
                precedente=segPointerCurs->sptr->A;
            }
            segPointerCurs=segPointerCurs->next;
            while(segPointerCurs!=NULL){

                if (segPointerCurs->orient=='+')
                {
                    if(precedente!=segPointerCurs->sptr->A ||( (segPointerCurs->next==NULL) && (primo!=segPointerCurs->sptr->B) ) )
                        printf("\nERRORE:segmenti non consecutivi. faccia:%p ", &faceCurs->F);

                    precedente=segPointerCurs->sptr->B;
                }
                else
                {
                    if(precedente!=segPointerCurs->sptr->B ||( (segPointerCurs->next==NULL) && (primo!=segPointerCurs->sptr->A) ) )
                        printf("\nERRORE:segmenti non consecutivi. faccia:%p ", &faceCurs->F);

                    precedente=segPointerCurs->sptr->A;
                }

                segPointerCurs=segPointerCurs->next;
            }
            faceCurs=faceCurs->next;
        }

    }

    int order_of_magnitude(int number)
    /* Funzione per calcolare l'odg di un numero intero, ovvero di quanti digit ho bisogno per scriverlo
       che è order of magnitude + 1 */
    {
        int ind = 0;
        double num = abs( (double) number);

        if (num < 10 ) // ritorna zero se è meno di uno!
        {
            return 0;
        }
        while ( num >= 10 )
        {
            num /= 10;
            ind++;
        }

        return ind;
    }

    void spaced_output(FILE* file, long out, long limit)
    /* Stampa un intero più degli spazi in modo da mantenere l'allineamento */
    {
        int i;
        fprintf(file,"%ld", out);
        for ( i = order_of_magnitude(out) ; i < order_of_magnitude(limit) ; i++)
        {
            fprintf(file," ");
        }
    }

    void spaced_output_char(FILE* file, long out, char ch, long limit)
    /* Stessa cosa di quella sopra, ma serve per stampare il numero, un carattere (orient) e poi gli spazi */
    {
        int i;
        fprintf(file,"%ld%c", out, ch);
        for ( i = order_of_magnitude(out) ; i < order_of_magnitude(limit) ; i++)
        {
            fprintf(file," ");
        }
    }

    long Max(long a, long b)
    {
        if (a >= b)
        {
            return a;
        }
        return b;
    }

