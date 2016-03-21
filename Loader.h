/*
    Programmazione e Calcolo Scientifico
    Header C del file Loader.c
    Usa la #include guard LOADER_H_INCLUDED
    Marco
*/

#ifndef LOADER_H_INCLUDED
#define LOADER_H_INCLUDED

int load_data(char*);

int load_edges(char*);

int load_faces(char*);

int load_solid();

int load_planes(char*);

int load_all(char* , char* , char*, char*);

int genera_piani(int);

void setstrings(char*);

#endif // LOADER_H_INCLUDED
