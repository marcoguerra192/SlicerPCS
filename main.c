
// #define DEBUG_H

#include "All.h"

int main(int argc, char** argv)
{
    Plane_List PlCurs;
    int n_piani = 0;
    unsigned long PlInd = 1; // Indice dei piani per il marker CausingPlane
                             // DEVE PARTIRE DA 1! (0 è il marker delle facce originali)
    tol = 0;
    RANDOMIZE = 1; // default
    GRAPHIC = 1;
    PREPRINT = 0;

    if (argc == 1) // nessun argomento
    {
        n_piani=25;
        tol = 0.00001;
        fprintf(OUTPUT, "No Argument, si esegue con default ( 25 piani random, tol = 10^-5) \n\n");
    }
    else if (argc == 2) //un arg -> n piani o l'help
    {
        if (argv[1][0] == 'h' || argv[1][0] == '?')
        {
            fprintf(OUTPUT, HELP);
            getchar();
            return 0;
        }
        else
        {
            tol = 0.00001;
            n_piani = atoi(argv[1]);
            if (n_piani == 0)
            {
                n_piani = 25;
                RANDOMIZE = 0;
            }
        }

    }
    else if (argc == 3) // due argomenti -> E' il numero di piani e tolleranza
    {

        n_piani = atoi(argv[1]);
        if (n_piani == 0)
            {
                n_piani = 25;
                RANDOMIZE = 0;
            }
        tol = atof(argv[2]);
        if (tol == 0)
        {
            tol = 0.00001;
            fprintf(OUTPUT, "\nImpossibile risolvere la tolleranza, si esegue con 10^-5\nContinua. . . \n");
            getchar();
        }
    }
    else // tre argomenti -> sono n piani, tolleranza e opzioni
    {
        n_piani = atoi(argv[1]);
        tol = atof(argv[2]);
        if (n_piani == 0)
        {
            n_piani = 25;
            RANDOMIZE = 0;
        }
        if (tol == 0)
        {
            tol = 0.00001;
            fprintf(OUTPUT, "\nImpossibile risolvere la tolleranza, si esegue con 10^-5\nContinua. . . \n");
            getchar();
        }
        if (argv[3][0] == '0') // SPEGNI GRAFICA
        {
            GRAPHIC = 0;
        }
        if (argv[3][1] == '1') //ACCENDI STAMPA PRIMA DEL RUN
        {
            PREPRINT = 1;
        }
    }

    if (RANDOMIZE)
    {
        fprintf(OUTPUT, "Esecuzione con %d piani RANDOM, ", n_piani);
    }
    else
    {
        fprintf(OUTPUT, "Esecuzione con piani DA FILE, ");
    }
    fprintf(OUTPUT, "con tol = %lf, stampa in modalita' ", tol);
    if (GRAPHIC)
    {
        fprintf(OUTPUT, "GRAFICA,  e pre-print ");
    }
    else
    {
        fprintf(OUTPUT, "TESTUALE,  e pre-print ");
    }
     if (PREPRINT)
    {
        fprintf(OUTPUT, "ATTIVATO.\n");
    }
    else
    {
        fprintf(OUTPUT, "DISATTIVATO.\n");
    }
    fprintf(OUTPUT, "\n> Invio per iniziare . . .");
    getchar();

    if (RANDOMIZE)
    {
        genera_piani(n_piani);
    }
    if (load_data("dati_punti.txt") != 0)
    {
        fprintf(OUTPUT, "Errore punti");
            getchar();
        return 1;
    }
    if(load_edges("dati_lati.txt")!= 0){
        fprintf(OUTPUT, "Errore segmenti");
        getchar();
        return 1;
    }
    if(load_faces("dati_facce.txt")!=0){
        fprintf(OUTPUT, "Errore facce");
        getchar();
        return 1;
    }
    if(load_solid()!=0){
        fprintf(OUTPUT, "Errore solidi");
        getchar();
        return 1;
    }
    if(load_planes("dati_piani.txt")!=0){
        fprintf(OUTPUT, "Errore piani");
        getchar();
        return 1;
    }

    fprintf(OUTPUT, "Loaded! \n");
    if (PREPRINT) // pre print
    {
        fprintf(OUTPUT, "Pre-stampa:  ");
        if (GRAPHIC)
        {
            Stampa(RESULT);
        }
        else
        {
            new_print_all(0);
        }

        getchar();
    }

    fprintf(OUTPUT,"\n\n\n\n           *           *          *\n");
    PlCurs=Pl;

    while (PlCurs!=NULL){

        shred();

        classifica(PlCurs->pl);

        #ifdef DEBUG_H
        controllo_errori();
        #endif

        splitting_segs(Sg, PlCurs->pl);

        #ifdef DEBUG_H
        controllo_errori();
        #endif

        slice_faces(Fc);

        #ifdef DEBUG_H
        controllo_errori();
        #endif

        chop_solids(Sol, PlCurs, PlInd);

        #ifdef DEBUG_H
        controllo_errori();
        #endif


        PlCurs=PlCurs->next; // Scorri avanti il cursore dei piani
        PlInd++; // Incrementa l'indice dei piani!
    }

    fprintf(OUTPUT, "\n\n\n\n______________________________________________________"

            "\nDone! \n");

    #ifdef DEBUG_H
    controllo_errori();
    #endif

    if (GRAPHIC)
    {
        Stampa(RESULT);
    }
    else
    {
        new_print_all(1);
    }


    getchar();


    return 0;
}

