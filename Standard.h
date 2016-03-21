/*
    HEADER C che "raccoglie" le inclusioni di tutte le librerie standard necessarie
*/

#ifndef STANDARD_H_INCLUDED
#define STANDARD_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#ifdef _WIN32 // Inclusione della libreria di Windows
    #include <windows.h>
#else
    #ifdef __linux__
        #include <linux.h> // Inclusione della libreria linux
    #endif
#endif


#endif // STANDARD_H_INCLUDED
