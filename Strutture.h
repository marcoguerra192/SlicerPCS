/*
    Progetto - Programmazione e Calcolo Scientifico
    HEADER C per la definizione delle STRUTTURE DATI
    Le variabili che vengono definite qui sono VARIABILI GLOBALI!
    Utilizza la #include guard STRUTTURE_H_INCLUDED  !!

*/

#ifndef STRUTTURE_H_INCLUDED
#define STRUTTURE_H_INCLUDED


#include "Tipi.h" //Includo i tipi


// LISTA PRINCIPALE DI PUNTI

Point_List Pt; /* Variabile di tipo Lista di Punti, cioè definita come puntatore ad un elemento della lista.
                  Contiene i campi: Pt->P che contiene una struttura punto (il punto corrente).
                                    Pt->next che punta al prossimo elemento della lista.  */

// LISTA PRINCIPALE DI SEGMENTI

Seg_List Sg; /* Variabile di tipo Lista di Segmenti, cioè definita come puntatore ad un elemento della lista.
                Contiene i campi: Sg->S che contiene una struttura segmento
                                  Sg->next che punta al prossimo elemento della lista.  */

// LISTA PRINCIPALE DI FACCE

Face_List Fc;  /* Variabile di tipo Lista di Facce, cioè definita come puntatore ad un elemento della lista.
                  Contiene i campi: Fc->F che contiene una struttura faccia
                                    Fc->next che punta al prossimo elemento della lista.  */

// LISTA PRINCIPALE DI SOLIDI

Solid_List Sol; /* Variabile di tipo Lista di Solidi, cioè definita come puntatore ad un elemento della lista.
                   Contiene i campi: Sol->So che contiene una struttura solido
                                     Sol->next che punta al prossimo elemento della lista.  */

Plane_List Pl; // Lista di piani


double tol; // double per l'asegnazione della TOLLERANZA.


//    DEFINISCO LO STREAM DI OUTPUT!
#define OUTPUT stdout


// Contatori dei pt, seg, facce, sol e facce originali
long NUMPUNTI;
long NUMSEG;
long NUMFACCE;
long NUMSOL;
long NUMPIANI;
long NUMFACCE_ORIG;

// Comandi shell per la stampa del risultato su MATLAB/Octave

#ifdef _WIN32 // se compili sotto Windows
    #define PLOT_RESULT_COMM "MATLAB -nodisplay -nosplash -nodesktop -r run('.\\Risultato.m');"
#else
    #ifdef __linux__ // se compili sotto linux
        #define PLOT_RESULT_COMM "/usr/local/MATLAB/R2015a/bin/matlab < ./Risultato.m"
    #else
        #define PLOT_RESULT_COMM "echo \" Impossibile determinare il sistema operativo \" ;"
    #endif
#endif

#define PLOT_RESULT_COMM "/usr/local/MATLAB/R2015a/bin/matlab < ./Risultato.m" // Non riesco a trovare la macro __linux__, quindi la definisco hard-coded per linux in attesa di risolvere

// OPZIONI varie per la generazione di piani random/fissa da file, stampa in modalità grafica/testuale, stampa del solido prima del run

short RANDOMIZE;
short GRAPHIC;
short PREPRINT;


// STRINGA HELP
#define HELP "Sintassi: .\\Progetto.exe [<N di Piani>] [<Tolleranza>] [ab] \n> \
 \n  a = 1 : Stampa in formato grafico con MATLAB - a = 0 : Stampa in formato testo su file \n \
 \n  b = 1 : Stampa il solido prima del run - b = 0 : Stampa solo alla fine (Stampa secondo la modalita' a) \n"

// NOME FILE PER IL RISULTATO
#define RESULT "Risultato.m"


// STRUTTURE DATI PER IL RETRO LINK FRA FACCE ORIGINALI E FACCE FINALI, E FRA FRATTURE E FACCE FINALI

Face_PointerList* FigliOriginali ; // vettore di Face_PointerList, k-esimo elemento è la lista di facce discendenti dalla faccia originale k.

Face_PointerList* GeneratiFrattura ; // vettore di Face_PointerList, k-esimo elemento è la lista facce generate dal k-esimo piano.


#endif // STRUTTURE_H_INCLUDED
