# include <stdlib.h>

# include "bmplib.h"
# include "bmplib.c"

# ifndef _IMAGELIB_H_
# define _IMAGELIB_H_

// Estas son las cabeceras de las funciones que se ocupan dentro del programa


Image* initImage(int height, int width);
Image* loadImage(FILE* myFile, InfoHeader header);
Image* open(char* myFile);
Image* convertToBin (int umbral, Image* myImage);
Image* readImg(char* name);
Image* convertToGray(Image* myImage);

void classification (int umbral ,Image* myImage);
void writeImg(int image, Image* myImage, int flag);
void pipeline (int quantityImages ,int flag, int umbralClassification, int umbralImages);
void freeImage (Image* myImage);

#endif