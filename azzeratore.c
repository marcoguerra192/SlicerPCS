/*
    Funzione che azzera i flag side delle facce, da richiamare ad ogni nuovo piano

*/


#include "Standard.h"
#include "Tipi.h"
#include "Strutture.h"

void shred()
{
    Face_List temp;
    temp = Fc; // assegno il cursore alla lista principale di facce
    while (temp != NULL)
    {
        temp->F.side = 0; // imposto non operare
        temp = temp->next;
    }

    return;
}
