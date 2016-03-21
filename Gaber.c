#include "Standard.h"
#include "Tipi.h"
#include "Strutture.h"
#include "Operazioni_Liste.h"


char dxsx(Point P, Plane Piano)
{
    Point charlie;
    double proiezione;

    charlie.x = P.x - Piano.pt.x;
    charlie.y = P.y - Piano.pt.y;
    charlie.z = P.z - Piano.pt.z;

    proiezione = dot_prod(charlie,Piano.n);

    if ((-tol <= proiezione) && (proiezione <= tol))
    {
        return '0';
    }
    if (proiezione > 0)
    {
        return '+';
    }
    else
    {
        return '-';
    }

}

int classifica(Plane Piano)
{
    Point_List curs;
    curs = Pt;
    while (curs != NULL)
    {
        curs->P.side = dxsx(curs->P, Piano);
        curs = curs->next;
    }
    return 0;
}
