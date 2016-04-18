/*
    Progetto - Programmazione e Calcolo Scientifico
    FILE C per il caricamento dei dati iniziali da file
    Utilizza la #include guard LOADER_H_INCLUDED  !!

    Marco
*/

// #define DEBUG_H

#include "Standard.h" // Inclusione librerie STANDARD "raccolte" nell'header
#include "Strutture.h" // Includo le strutture dati
#include "Operazioni_Liste.h" // Includo le operazioni su liste e quelle vettoriali
#include "time.h"

#define DEBUG_H

//prototipi
Point* fetch_point(int);
Seg* fetch_seg(int);

int load_data(char* filename) // carica punti
/*
    FUNZIONE che carica in memoria la struttura dati dei PUNTI da un file di nome filename
*/
{
    FILE* loader_stream;   // Dichiara il file stream
    char* str = NULL; // Dichiara due stringhe "di servizio"
    char err_flag = -1; // flag d'errore che si disattiverà se il ciclo di lettura fa almeno un'operazione
    double n[3]; // variabili temporanee per leggere le coordinate
    Point_List p = NULL; // Cursore che scorre sulla lista punti

    NUMPUNTI = 0;

    if (filename == NULL) // verifica che il parametro sia buono
    {
        #ifdef DEBUG_H
        fprintf(OUTPUT, "Load_Data: Il filename non è una stringa valida! \nUscita \n");
        #endif
        return -1;
    }

    loader_stream = fopen(filename, "r"); // Apre il file stream in modalità lettura

    if (loader_stream == NULL) // verifico di aver correttamente linkato il file
    {
        #ifdef DEBUG_H
        fprintf(OUTPUT,"Load_Data: Impossibile linkare il file specificato, sintassi errata o file non esistente \nUscita \n");
        #endif
        return -1;
    }
    #ifdef DEBUG_H
    else
    {
        fprintf(OUTPUT,"Load_Data: File input correttamente linkato \n");
    }
    #endif


    Pt = (Point_List)malloc(sizeof(Point_List_El)); // Alloco il primo elemento di Pt
    if (Pt == NULL)
	 {
			#ifdef DEBUG_H
			fprintf(OUTPUT, "Load_Data: Impossibile allocare la lista principale! Uscita \n");
			#endif
			return -1;
	 }
    p = Pt; // e lo assegno al puntatore p
    str = (char*) malloc((size_t)80*(sizeof(char))); // alloco lo spazio sulla stringa

    fgets(str, 80, loader_stream); // Primo giro fuori dal while per non allocare troppo
       if (str == NULL)
       {
                #ifdef DEBUG_H
                fprintf(OUTPUT, "Load_Data: Ho letto una riga nulla!\n");
                #endif
                return -1;
       }
       if (sscanf(str, "%lf %lf %lf", &n[0],&n[1],&n[2]) != 3) // leggo tre double sulla lista
       {
                #ifdef DEBUG_H
                fprintf(OUTPUT, "Load_Data: Non ho letto tre numeri!\n");
                #endif
                return -1;
       }
       // NON alloco il prossimo elemento al primo giro!
       p->P.x = n[0];
       p->P.y = n[1];
       p->P.z = n[2]; // scrivo le coordinate (NON SCRIVO SIDE)
	   p->next = NULL;
       err_flag = 0;  // azzero il flag
       NUMPUNTI++;

       while (!feof(loader_stream) && err_flag == 0)
       {
           fgets(str, 80, loader_stream);
           if (str == NULL)
           {
               #ifdef DEBUG_H
               fprintf(OUTPUT, "Load_Data: Ho letto una riga nulla!\n");
               #endif
               break;
           }
           if (sscanf(str, "%lf %lf %lf", &n[0],&n[1],&n[2]) != 3) // leggo tre double sulla lista
           {
               #ifdef DEBUG_H
               fprintf(OUTPUT, "Load_Data: Non ho letto tre numeri!\n");
               #endif
               break;
           }
           p->next = (Point_List) malloc(sizeof(Point_List_El)); // alloco il prossimo elemento
		 if (p->next == NULL)
		 {
			#ifdef DEBUG_H
			fprintf(OUTPUT, "Load_Data: Impossibile allocare spazio! Uscita \n");
			#endif
			return -1;
		 }
           p = p->next; // scorro avanti il puntatore sull'elemento nuovo
           p->P.x = n[0];
           p->P.y = n[1];
           p->P.z = n[2];  // scrivo le coordinate (NON SCRIVO SIDE)
		   p->next = NULL; // metto a null l'elemento successivo per indicare la fine lista
           // err_flag = 0;   // azzero il flag INUTILE!
           NUMPUNTI++; // conto un pt in più
       } // finito il ciclo di lettura dei punti

       if (err_flag == -1)
       {
           #ifdef DEBUG_H
           fprintf(OUTPUT, "Load_Data: Non ho letto una mazza, dealloco!\n");
           #endif
           free(Pt); // Libera la memoria di Pt, perchè tanto non ha letto niente
           return -1;
       }
       else
       {
           #ifdef DEBUG_H
           fprintf(OUTPUT, "Load_Data: Punti caricati! \n");
           #endif
           return 0;
       }


    }      // FINE LETTURA PUNTI

int load_edges(char* filename)
/* Carica i lati indicando l'indice con cui i punti estremi sono inseriti nella lista
   da un file che contiene coppie di numeri interi su ogni riga */
{
	FILE* loader_stream;   // Dichiara il file stream
	char* str = NULL;      // Dichiara una stringa "di servizio"
	int n[2];              // Vettore di due interi per
	Seg_List s;            // Cursore temporaneo sulla lista di segmenti
	int err_flag = -1;     // Flag d'errore

	NUMSEG = 0;

	if (filename == NULL)  // verifica che il parametro sia buono
     {
         #ifdef DEBUG_H
         fprintf(OUTPUT, "Load_Edges: Il filename non è una stringa valida! Uscita \n");
         #endif
         return -1;
     }

	loader_stream = fopen(filename, "r"); // Apre il file stream in modalità lettura

     if (loader_stream == NULL) // verifico di aver correttamente linkato il file
     {
         #ifdef DEBUG_H
         fprintf(OUTPUT,"Load_Edges: Impossibile linkare il file specificato, sintassi errata o file non esistente. Uscita \n");
         #endif
         return -1;
     }
     #ifdef DEBUG_H
     else
     {
         fprintf(OUTPUT,"Load_Edges: File input correttamente linkato \n");
     }
     #endif

    // primo giro fuori dal ciclo while
	Sg = (Seg_List) malloc (sizeof(Seg_List_El)); // alloco il primo elemento della lista di segmenti
	if (Sg == NULL)
	{
		#ifdef DEBUG_H
		fprintf(OUTPUT, "Load_Edges: Impossibile allocare la lista principale! Uscita \n");
		#endif
		return -1;
	}
	s = Sg; //assegno la testa lista al cursore temporaneo
	str = (char*) malloc((size_t)80*(sizeof(char))); // alloco lo spazio sulla stringa

	fgets(str,80,loader_stream); // tiro su una linea
	if (str == NULL)
     {
               #ifdef DEBUG_H
               fprintf(OUTPUT, "Load_Edges: Ho letto una riga nulla!\n");
               #endif
               return -1;
     }

	if (sscanf(str, "%d %d", &n[0],&n[1]) != 2) // verifico di aver letto due interi
	{
		#ifdef DEBUG_H
          fprintf(OUTPUT, "Load_Edges: Non ho letto due numeri!\n");
          #endif
          return -1;
	}

    s->S.A = fetch_point( n[0] ); // assegno il primo punto del segmento
    s->S.B = fetch_point( n[1] ); // assegno il secondo punto del segmento
    if (s->S.A == NULL || s->S.B == NULL) // controllo di non aver sforato
        {
            #ifdef DEBUG_H
            fprintf(OUTPUT, "Load_Edges: File errato! Si richiedono punti che non esistono nella lista! \n");
            #endif
            return -1;
        }
    s->S.f = NULL; // la lista facce sarà allocata al passaggio delle FACCE!
    s->next = NULL; // indico la (per ora) fine lista
    err_flag = 0; // ha letto qualcosa, quindi spengo il flag
    NUMSEG++;

    while (!feof(loader_stream) && err_flag == 0) //faccio il ciclo while per la lettura
    {
        fgets(str,80,loader_stream); // tiro su una linea
        if (str == NULL)
        {
               #ifdef DEBUG_H
               fprintf(OUTPUT, "Load_Edges: Ho letto una riga nulla!\n");
               #endif
               return -1;
        }

        if (sscanf(str, "%d %d", &n[0],&n[1]) != 2) // verifico di aver letto due interi
        {
            #ifdef DEBUG_H
            fprintf(OUTPUT, "Load_Edges: Non ho letto due numeri!\n");
            #endif
            return -1;
        }

        s->next = (Seg_List) malloc(sizeof(Seg_List_El)); //alloco il prossimo elemento
        if (s->next == NULL)
        {
            #ifdef DEBUG_H
			fprintf(OUTPUT, "Load_Edges: Impossibile allocare spazio! Uscita \n");
			#endif
			return -1;
        }
        s = s->next; // scorro avanti il puntatore
        s->S.A = fetch_point( n[0] ); // assegno il primo punto del segmento
        s->S.B = fetch_point( n[1] ); // assegno il secondo punto del segmento
        if (s->S.A == NULL || s->S.B == NULL) // controllo di non aver sforato
        {
            #ifdef DEBUG_H
            fprintf(OUTPUT, "Load_Edges: File errato! Si richiedono punti che non esistono nella lista! \n");
            #endif
            return -1;
        }
        s->S.f = NULL; // la lista facce sarà allocata al passaggio delle FACCE!
        s->next = NULL; // indico la (per ora) fine lista
        // err_flag = 0; // ha letto qualcosa, quindi spengo il flag INUTILE già fatto
        NUMSEG++; // conto un segmento in più


    } // fine ciclo while per la lettura

    if (err_flag == -1)
       {
           #ifdef DEBUG_H
           fprintf(OUTPUT, "Load_Edges: Non ho letto una mazza, dealloco!\n");
           #endif
           free(Sg); // Libera la memoria di Sg, perchè tanto non ha letto niente
           return -1;
       }
       else
       {
           #ifdef DEBUG_H
           fprintf(OUTPUT, "Load_Edges: Segmenti caricati! \n");
           #endif
           return 0;
       }

} // Fine lettura segmenti

Point* fetch_point(int n)
/*
    Funzione che dato un intero n ritorna un puntatore al punto nella posizione n della lista punti
    oppure NULL se il numero sfora la lista punti
    IL NUMERO DEI PUNTI INIZIA DA 1unooo!!uno!!undici!!xDtroppostyle
*/
{
    int i = 1;
    Point_List p;

    p = Pt; // assegno la testa della lista
    while (i < n) //scorro avanti nella lista
    {
        if (p->next != NULL) // controlla di non sforare
        {
            p = p->next;
        }
        else
        {
            return NULL;
        }

        i++;
    }
    return &(p->P);
}

int load_faces(char* filename)
/* Nella nuova implementazione la sintassi del file delle facce dev'essere la seguente:
Una riga composta da <numero><+/->< spazio> ripetuto per ogni segmento della faccia, fino all'ultimo nella forma <numero><+/-><!>
Quindi a capo (\n o \r).
Poi una riga con un solo numero, indicante il marker della faccia. I MARKER NON POSSONO PARTIRE DA 0, MA DEVONO PARTIRE DA 1
Ripetere per ogni faccia.
Infine una riga con un solo punto esclamativo.
Deve gestire anche l'ordinamento dei segmenti e la lista facce di ogni segmento */
{
    FILE* loader_stream;   // Dichiara il file stream
	Face_List f;           // Cursore per scorrere sulla lista di facce
	Seg_PointerList SegCurs;
	Face_PointerList tmpFace;
	int tmpnum = 0;
	char ch, ori;
	char* line = (char*) malloc(200*sizeof(char)); // riga del file
	FILE* line_stream; // stream per scorrere la riga
	Point vett1, vett2, norm; // vettori per calcolare la normale

    NUMFACCE = 0;

    unsigned int MarkerFaccia; // Marker per segnare le facce originali

	if (filename == NULL)  // verifica che il parametro sia buono
     {
         #ifdef DEBUG_H
         fprintf(OUTPUT, "Load_Faces: Il filename non è una stringa valida! Uscita \n");
         #endif
         return -1;
     }

	 loader_stream = fopen(filename, "r"); // Apre il file stream in modalità lettura

     if (loader_stream == NULL) // verifico di aver correttamente linkato il file
     {
         #ifdef DEBUG_H
         fprintf(OUTPUT,"Load_Faces: Impossibile linkare il file specificato, sintassi errata o file non esistente. Uscita \n");
         #endif
         return -1;
     }
     #ifdef DEBUG_H
     else
     {
         fprintf(OUTPUT,"Load_Faces: File input correttamente linkato \n");
     }
     #endif

     Fc = (Face_List) malloc (sizeof(Face_List_El)); // alloco il primo elemento della lista di segmenti
	 if (Fc == NULL)
	 {
		#ifdef DEBUG_H
		fprintf(OUTPUT, "Load_Faces: Impossibile allocare la lista principale! Uscita \n");
		#endif
		return -1;
	 }

    f = Fc; //assegno la testa lista al cursore temporaneo

    f->F.s = (Seg_PointerList) malloc (sizeof(Seg_PointerList_El));
    SegCurs = f->F.s; // assegno il cursore

    fgets(line, 199, loader_stream); // leggi una riga dal file fino all'a capo
    if ( line[0] == '!' || line[0]=='\r' || line[0]=='\n' || line[0]=='\0') // file vuoto
    {
        #ifdef DEBUG_H
        fprintf(OUTPUT, "Load_Faces: Il file non contiene facce");
        #endif
        return -1;
    }

    line_stream = fmemopen(line, strlen(line),"r"); // converti la stringa letta in un buffer

    fscanf(line_stream,"%d%c%c",&tmpnum, &ori, &ch);
    SegCurs->sptr = fetch_seg(tmpnum);
    SegCurs->orient = ori;
    SegCurs->next = NULL;

    while (( ch != '!' && fscanf(line_stream,"%d%c%c",&tmpnum, &ori, &ch) == 3)) // ciclo (orizzontale) sui SEGMENTI della prima faccia FUORI
    {
        SegCurs->next = (Seg_PointerList) malloc (sizeof(Seg_PointerList_El));
        SegCurs->next->sptr = fetch_seg(tmpnum); // copio il puntatore al segmento
        SegCurs->next->orient = ori; // copio l'orientamento
        SegCurs->next->next = NULL; //termino lista
        SegCurs = SegCurs->next;
    }

    // Ora dalla riga successiva leggo IL MARKER DELLA FACCIA!

    fgets(line, 199, loader_stream); // leggi una riga dal file fino all'a capo

    MarkerFaccia = atoi(line);
    if (MarkerFaccia == 0)
    {
        #ifdef DEBUG_H
        fprintf(OUTPUT, "Load_Faces: Impossibile leggere il marker della faccia! Uscita \n");
        #endif
        return -1;
    }

    // assegno il marker letto alla faccia corrente
    f->F.OriginalFace = MarkerFaccia;
    // Fine assegnazione marker faccia originale

    // Tutte le facce originali sono generate (formalmente) dal piano 0, dunque assegno marker CausingPlane 0
    f->F.CausingPlane = (unsigned long) 0;
    // Fine assegnazione marker piano generatore


    // adesso calcoliamo la normale

    vett1.x = f->F.s->sptr->B->x - f->F.s->sptr->A->x; // creo un primo vettore
    vett1.y = f->F.s->sptr->B->y - f->F.s->sptr->A->y;
    vett1.z = f->F.s->sptr->B->z - f->F.s->sptr->A->z;

    if (f->F.s->orient == '-')
    {
        vett1.x *= -1;
        vett1.y *= -1;
        vett1.z *= -1;
    }

    vett2.x = f->F.s->next->sptr->B->x - f->F.s->next->sptr->A->x; // creo il secondo vettore
    vett2.y = f->F.s->next->sptr->B->y - f->F.s->next->sptr->A->y;
    vett2.z = f->F.s->next->sptr->B->z - f->F.s->next->sptr->A->z;

    if (f->F.s->next->orient=='-')
    {
        vett2.x *= -1;
        vett2.y *= -1;
        vett2.z *= -1;
    }


    norm = cross_prod(vett1,vett2); // calcolo il prodotto vettore
    norm = vett_scal(norm,1/eu_norm(norm)); // e normalizzo
    f->F.norm_vect.x = norm.x; // copio il vettore normale
    f->F.norm_vect.y = norm.y;
    f->F.norm_vect.z = norm.z;
    f->next = NULL;

    // ora salviamo la faccia nuova nei suoi segmenti
    SegCurs = f->F.s;
    while (SegCurs != NULL)
        {
            if (SegCurs->sptr->f == NULL) // se è la prima faccia che gli inserisci
        {
            SegCurs->sptr->f = (Face_PointerList) malloc(sizeof(Face_PointerList_El)); // alloco
            SegCurs->sptr->f->fptr = &(f->F); // salvo l'indirizzo della faccia
            SegCurs->sptr->f->next = NULL;
        }
        else // se invece ci avevi già scritto aggiungi in testa
        {
            tmpFace = SegCurs->sptr->f;
            SegCurs->sptr->f = (Face_PointerList) malloc(sizeof(Face_PointerList_El)); // alloco
            SegCurs->sptr->f->fptr = &(f->F);
            SegCurs->sptr->f->next = tmpFace;
        }

        SegCurs = SegCurs->next; // scorri avanti al segmento successivo
    }
    // fine del salvataggio delle facce nei segmenti

    NUMFACCE++;
     // ora ciclo dentro al while per le righe dalla seconda in poi
     f=Fc;

     while (!feof(loader_stream))
     {
        /* Qui dentro scriviamo le stesse istruzioni ma allocando i nuovi elementi della lista*/

        fgets(line, 199, loader_stream); // leggi una riga dal file fino all'a capo


        if (line[0]=='!' || line[0]=='\n' || line[0]=='\r' || line[0]=='\0') // se la riga è vuota
        {
            #ifdef DEBUG_H
            fprintf(OUTPUT, "Load_Faces: Riga vuota, uscita. \n");
            #endif
            return 0;
        }

        line_stream = fmemopen(line, strlen(line),"r"); // converti la stringa in un buffer

        fscanf(line_stream,"%d%c%c",&tmpnum, &ori, &ch);

        f->next=(Face_List)malloc(sizeof(Face_List_El));
        f=f->next;
        f->F.s = (Seg_PointerList) malloc (sizeof(Seg_PointerList_El));;
        SegCurs = f->F.s;


        SegCurs->sptr = fetch_seg(tmpnum);
        SegCurs->orient = ori;
        SegCurs->next = NULL;

        while ((ch != '!' && fscanf(line_stream,"%d%c%c",&tmpnum, &ori, &ch) == 3)) // ciclo (orizzontale) sui SEGMENTI della prima faccia DENTRO
        {
            SegCurs->next = (Seg_PointerList) malloc (sizeof(Seg_PointerList_El));
            SegCurs->next->sptr = fetch_seg(tmpnum); // copio il puntatore al segmento
            SegCurs->next->orient = ori; // copio l'orientamento
            SegCurs->next->next = NULL; //termino lista
            SegCurs = SegCurs->next;
        }

        // Ora dalla riga successiva leggo IL MARKER DELLA FACCIA!

        fgets(line, 199, loader_stream); // leggi una riga dal file fino all'a capo

        MarkerFaccia = atoi(line);
        if (MarkerFaccia == 0)
        {
            #ifdef DEBUG_H
            fprintf(OUTPUT, "Load_Faces: Impossibile leggere il marker della faccia! Uscita \n");
            #endif
            return -1;
        }

        // assegno il marker letto alla faccia corrente
        f->F.OriginalFace = MarkerFaccia;
        // Fine assegnazione marker faccia originale

        // Tutte le facce originali sono generate (formalmente) dal piano 0, dunque assegno marker CausingPlane 0
        f->F.CausingPlane = (unsigned long) 0;
        // Fine assegnazione marker piano generatore

        // adesso calcoliamo la normale

        vett1.x = f->F.s->sptr->B->x - f->F.s->sptr->A->x; // creo un primo vettore
        vett1.y = f->F.s->sptr->B->y - f->F.s->sptr->A->y;
        vett1.z = f->F.s->sptr->B->z - f->F.s->sptr->A->z;

        if (f->F.s->orient == '-') // se era al contrario lo giro
        {
            vett1.x *= -1;
            vett1.y *= -1;
            vett1.z *= -1;
        }

        vett2.x = f->F.s->next->sptr->B->x - f->F.s->next->sptr->A->x; // creo il secondo vettore
        vett2.y = f->F.s->next->sptr->B->y - f->F.s->next->sptr->A->y;
        vett2.z = f->F.s->next->sptr->B->z - f->F.s->next->sptr->A->z;

        if (f->F.s->next->orient == '-') // se era al contrario lo giro
        {
            vett2.x *= -1;
            vett2.y *= -1;
            vett2.z *= -1;
        }

        norm = cross_prod(vett1,vett2); // calcolo il prodotto vettore
        norm = vett_scal(norm,1/eu_norm(norm)); // e normalizzo
        f->F.norm_vect.x = norm.x; // copio il vettore normale
        f->F.norm_vect.y = norm.y;
        f->F.norm_vect.z = norm.z;
        f->next = NULL;

        // ora salviamo la faccia nuova nei suoi segmenti
        SegCurs = f->F.s;
        while (SegCurs != NULL)
            {
                if (SegCurs->sptr->f == NULL) // se è la prima faccia che gli inserisci
            {
                SegCurs->sptr->f = (Face_PointerList) malloc(sizeof(Face_PointerList_El)); // alloco
                SegCurs->sptr->f->fptr = &(f->F); // salvo l'indirizzo della faccia
                SegCurs->sptr->f->next = NULL;
            }
            else // se invece ci avevi già scritto aggiungi in testa
            {
                tmpFace = SegCurs->sptr->f;
                SegCurs->sptr->f = (Face_PointerList) malloc(sizeof(Face_PointerList_El)); // alloco
                SegCurs->sptr->f->fptr = &(f->F);
                SegCurs->sptr->f->next = tmpFace;
            }

            SegCurs = SegCurs->next; // scorri avanti al segmento successivo
        }

        NUMFACCE++;
     }

    return 0;
}

Seg* fetch_seg(int n)
{
    int i = 1;
    Seg_List s;

    s = Sg; // assegno la testa della lista
    while (i < n) //scorro avanti nella lista
    {
        if (s->next != NULL) // controlla di non sforare
        {
            s = s->next;
        }
        else
        {
            return NULL;
        }

        i++;
    }
    return &(s->S);
}

int load_solid()
{
    // Si suppone che le facce siano state date con l'orientamento dei segmenti definito rispetto al vettore normale USCENTE
    Face_List Fa; // cursore sulle facce
    Face_PointerList FaPtr; // cursore sulla lista di puntatori a facce

    Sol = (Solid_List) malloc(sizeof(Solid_List_El)); // alloco per un solo solido
    Sol->next = NULL; // pongo il next a null perchè ho un solo elemento

    Fa = Fc; // punta alla lista di facce

    Sol->So.f = (Face_PointerList) malloc(sizeof(Face_PointerList_El)); // alloco il primo caso fuori
    FaPtr = Sol->So.f; // punta alla lista facce del mio solido

    FaPtr->fptr = &(Fa->F);
    FaPtr->orient = '+'; // l'orientamento iniziale deve essere USCENTE

    FaPtr->next = NULL;

    while(Fa->next != NULL)
    {
        FaPtr->next = (Face_PointerList) malloc(sizeof(Face_PointerList_El));
        FaPtr->next->fptr = &(Fa->next->F);
        FaPtr->next->orient = '+'; // l'orientamento iniziale deve essere USCENTE
        FaPtr->next->next = NULL;

        FaPtr = FaPtr->next; // scorro avanti sui puntatori a faccia
        Fa = Fa->next; // passo successivo
    }

    // ho finito di copiare i puntatori a faccia

    // ora segno l'unico solido su tutte le facce

    Fa = Fc; // rimetto il cursore delle facce all'inizio

    while (Fa != NULL) // Salvo il solo solido come Sol1, mentre Sol2 è NULL
    {
        Fa->F.Sol1 =  &(Sol->So);
        Fa->F.Sol2 = NULL;

        Fa = Fa->next; // scorro avanti
    }

    // fatto

    NUMSOL = 1;

    return 0;
}


int load_planes(char* filename)
/* IMPORTANTE La fine del file va segnalata con una riga che contenga il solo carattere '!'
*/
{
    FILE* loader_stream;
    char* str;
    Plane_List p;
    double n[6];
    Point vect;

    NUMPIANI = 0;

    if (filename == NULL)  // verifica che il parametro sia buono
     {
         #ifdef DEBUG_H
         fprintf(OUTPUT, "Load_Planes: Il filename non è una stringa valida! Uscita \n");
         #endif
         return -1;
     }

	 loader_stream = fopen(filename, "r"); // Apre il file stream in modalità lettura

     if (loader_stream == NULL) // verifico di aver correttamente linkato il file
     {
         #ifdef DEBUG_H
         fprintf(OUTPUT,"Load_Planes: Impossibile linkare il file specificato, sintassi errata o file non esistente. Uscita \n");
         #endif
         return -1;
     }
     #ifdef DEBUG_H
     else
     {
         fprintf(OUTPUT,"Load_Planes: File input correttamente linkato \n");
     }
     #endif

     Pl = (Plane_List) malloc (sizeof(Plane_List_El)); // alloco sulla lista di piani

     if (Pl == NULL)
     {
         fprintf(OUTPUT, "Load_Planes: Impossibile allocare la lista, uscita \n");
         return -1;
     }
     #ifdef DEBUG_H
     else
     {
         fprintf(OUTPUT,"Load_Planes: Lista correttamente allocata \n");
     }
     #endif

    p = Pl; // assegno il cursore alla testa della lista
    str = (char*) malloc((size_t) 80*sizeof(char)); // alloco spazio sulla stringa


    fgets(str, 80, loader_stream); // primo caso fuori dal while

    if (str == NULL)
    {
        fprintf(OUTPUT, "Load_Planes: Non ho letto una riga dal file \n");
        return -1;
    }

    if (str[0] == '!') // segnale di fine file
    {
        #ifdef DEBUG_H
        fprintf(OUTPUT,"Load_Planes: Il file è formattato correttamente, ma non contiene piani! Uscita \n");
        #endif // DEBUG_h
        return 1;
    }

    if (sscanf(str,"%lf %lf %lf %lf %lf %lf", &n[0], &n[1], &n[2], &n[3], &n[4], &n[5]) != 6)
    {
        fprintf(OUTPUT,"Load_Planes: non ho letto 6 double");
        return -1;
    }

    Pl->pl.pt.x = n[0];
    Pl->pl.pt.y = n[1];
    Pl->pl.pt.z = n[2];

    vect.x = n[3];
    vect.y = n[4];
    vect.z = n[5];

    vect=vett_scal(vect,1/eu_norm(vect));

    Pl->pl.n.x = vect.x;
    Pl->pl.n.y = vect.y;
    Pl->pl.n.z = vect.z;

    NUMPIANI++;

    Pl->next = NULL; // segno la fine della lista dei piani, poi nel caso andrò avanti

    fgets(str,80,loader_stream);
    if (str == NULL)
        {
            fprintf(OUTPUT, "Load_Planes: Impossibile leggere una riga dal file \n");
            return -1;
        }

    while(str[0] != '!') // procedi fino alla fine del file
    {
        p->next = (Plane_List) malloc (sizeof(Plane_List_El));
        if (p == NULL)
        {
            fprintf(OUTPUT, "Load_Planes: Impossibile allocare, uscita \n");
            return -1;
        }


        if (sscanf(str,"%lf %lf %lf %lf %lf %lf", &n[0], &n[1], &n[2], &n[3], &n[4], &n[5]) != 6)
        {
            fprintf(OUTPUT,"Load_Planes: non ho letto 6 double");
            return -1;
        }

        p->next->pl.pt.x = n[0];
        p->next->pl.pt.y = n[1];
        p->next->pl.pt.z = n[2];

        vect.x = n[3];
        vect.y = n[4];
        vect.z = n[5];

        vect=vett_scal(vect,1/eu_norm(vect));

        p->next->pl.n.x = vect.x;
        p->next->pl.n.y = vect.y;
        p->next->pl.n.z = vect.z;

        p->next->next = NULL;

        NUMPIANI++;

        p = p->next;

        fgets(str,80,loader_stream);
        if (str == NULL)
        {
            fprintf(OUTPUT, "Load_Planes: Impossibile leggere una riga dal file \n");
            return -1;
        }

    } // fine ciclo sulle righe



     return 0;
}

int genera_piani(n_piani)
{
    int i=0,k;
    FILE* fp;
    fp = fopen("dati_piani.txt","w");
    if(fp == NULL)
    {
        return -1;
    }
    srand(time(NULL));
    for(i=0;i<n_piani;i++)
    {
        if (i>0)
            fprintf(fp,"\n");
        for(k=0;k<3;k++){
            fprintf(fp,"%f ", 10*(float)rand()/RAND_MAX - 5 );
        }
        for(k=0;k<3;k++){
            fprintf(fp,"%f ", 2*(float)rand()/RAND_MAX - 1 );
        }
    }

    fclose(fp);
    return 1;

}


int load_all(char* punti, char* lati, char* facce, char* piani)
{
    if (load_data(punti) != 0)
    {
        fprintf(OUTPUT, "Errore nel leggere i punti! \n");
    }
    #ifdef DEBUG_H
    else
    {
        fprintf(OUTPUT,"Punti letti correttamente\n");
    }
    #endif

    if (load_edges(lati) != 0)
    {
        fprintf(OUTPUT, "Errore nel leggere i lati! \n");
    }
    #ifdef DEBUG_H
    else
    {
        fprintf(OUTPUT,"Lati letti correttamente\n");
    }
    #endif

    if (load_faces(facce) != 0)
    {
        fprintf(OUTPUT, "Errore nel leggere le facce! \n");
    }
    #ifdef DEBUG_H
    else
    {
        fprintf(OUTPUT,"Facce lette correttamente\n");
    }
    #endif
    if (load_planes(piani) != 0)
    {
        fprintf(OUTPUT,"Errore nel leggere i piani! \n");
    }
    #ifdef DEBUG_H
    else
    {
        fprintf(OUTPUT,"Piani letti correttamente\n");
    }
    #endif

    return 0;
}

