# include <stdlib.h>

#ifndef _STRUCTS_H_
#define _STRUCTS_H_

//Tenemos declaradas todas las estructuras que se ocupan dentro del programa 

typedef struct InfoHeader
{
	char bm[2];      // Bitmap
	int size;        // Tamaño
	int reserved;    // Reservado
	int offset;      // 
	int metadata;    // Tamaño de los metadados.
	int width;       // Ancho
	int height;      // Alto
	int planes;      // Planos de color
	int bpp;         // Bits por pixel
	int compress;    // Compresión
	int imgsize;     // Tamaño de los datos de la imagen
	int bpmx;        // Resolución X en bits por metro 
	int bpmy;        // Resolución Y en bits por metro
	int colors;      // Colores usados en la paleta 
	int imxtcolors;  // Colores importantes. 0 si son todos

} InfoHeader;

typedef struct Pixel
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;
} Pixel;

typedef struct Image
{
	int height;
	int width;
	int isNearlyBlack;
	Pixel** matrix;
	InfoHeader header;
} Image;

#endif