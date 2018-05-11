# include <stdlib.h>

#ifndef _BMPLIB_H_
#define _BMPLIB_H_


// Estas son las cabeceras de las funciones que se ocupan dentro del programa

int isBMP(FILE* myFile, InfoHeader infoh);
InfoHeader readInfoHeader(FILE* myFile);
void binToBmp(int image, Image* myImage);


#endif