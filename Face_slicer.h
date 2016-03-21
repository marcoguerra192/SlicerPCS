/*
    Progetto
    Header C per il file di scorrimento sulle facce Face_slicer.c
    Usa la #include guard FACE_SLICER_H_INCLUDED
*/

#ifndef FACE_SLICER_H_INCLUDED
#define FACE_SLICER_H_INCLUDED

int face_slicer(Face_List);

int slice_faces(Face_List);

int classifica_faccia(Face_List);

#endif // FACE_SLICER_H_INCLUDED
