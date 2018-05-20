# include <stdlib.h>
# include <stdio.h>
# include <string.h>

# include "structs.h"
# include "bmplib.h"

// fread(void *puntero, size_t tamanyo, size_t nmemb, FILE *stream); 
// fseek(FILE *stream, long int desplazamiento, int origen);

/*
Entrada: Un archivo.
Descripción: Lo que hace es validar el formato de la imagen.
Salida: Entero que representa booleano.
*/
int isBMP(FILE* myFile, InfoHeader infoh)
{
	char* type = (char*)malloc(sizeof(char) * 3);
	fseek(myFile, 0, 0);
	fread(type, 1, 2, myFile);
	type[2] = '\0';
	if(strcmp(type, "BM") || infoh.bpp != 24 || infoh.bpp != 32)
	{
		return 0;
	}
	return 1;
}

/*
Entrada: Archivo
Descripción: Lee la información que corresponde al header (esto se hace con el fin de obtener metadatos).
Salida: Estructura que respresenta al header..
*/
InfoHeader readInfoHeader(FILE* myFile)
{
	InfoHeader infoHeader;
	// Hay que ir moviendo el indicador del archivo, para ir calculando los datos que necesitamos:
	fseek(myFile, 0, SEEK_SET);
	// Bitmap
	fread(&infoHeader.bm, sizeof(char), 2, myFile);
	// Size
	fread(&infoHeader.size, 4, 1, myFile);
	// Reservado
	fread(&infoHeader.reserved, 4, 1, myFile);
	// Offset
	fread(&infoHeader.offset, 4, 1, myFile);
	// Tamaño metadatos
	fread(&infoHeader.metadata, 4, 1, myFile);
	// Ancho en pixeles
	fread(&infoHeader.width, 4, 1, myFile);
	// Alto en pixeles
	fread(&infoHeader.height, 4, 1, myFile);
	// Planos de color
	fread(&infoHeader.planes, 2, 1, myFile);
	// Bits por pixel
	fread(&infoHeader.bpp, 2, 1, myFile);
	// Tipo de comprension
	fread(&infoHeader.compress, 4, 1, myFile);
	// Tamaño de la imagen (bytes)
	fread(&infoHeader.imgsize, 4, 1, myFile);
	// Resolución X en bits por metro
	fread(&infoHeader.bpmx, 4, 1, myFile);
	// Resolución Y en bits por metro
	fread(&infoHeader.bpmy, 4, 1, myFile); 
	// Colores usados en la paleta 
	fread(&infoHeader.colors, 4, 1, myFile);
	// Colores importantes. 0 si son todos
	fread(&infoHeader.imxtcolors, 4, 1, myFile);
	return infoHeader;
}

/*
Entrada: (int) que corresponde a qué imagen se está guardando
		 (int**) corresponde a la matriz binaria.
		 (InfoHeader) es la estructura que contiene la información para el header.
Descripción: Transforma la matriz binarizada a imagen BMP. Escribe solo una imagen.
Salida: -
*/
void binToBmp(char* image, Image* myImage)
{
	char* route;
	char* number;
	route = (char*)malloc(sizeof(char)*50);
	number = (char*)malloc(sizeof(char)*50);
	strcpy(route,"../imagesbin/imagenbin_");
	InfoHeader infoHeader = myImage->header;
	sprintf(number, "%s", image);
	strcat(route, number);
    strcat(route, ".bmp");
	FILE* myFile =fopen(route, "wb");
	if(!myFile)
	{
		perror("## NO SE PUEDE CREAR EL ARCHIVO DE SALIDA PARA LA IMAGEN ##");
		exit(0);
	}
	// Se escribe la cabecera de la imagen
	fseek(myFile,0, SEEK_SET);
	fwrite(&infoHeader.bm, sizeof(char), 2, myFile);
	fwrite(&infoHeader.size, 4, 1, myFile);
	fwrite(&infoHeader.reserved, 4, 1, myFile);
	fwrite(&infoHeader.offset, 4, 1, myFile);
	fwrite(&infoHeader.metadata, 4, 1, myFile);
	fwrite(&infoHeader.width, 4, 1, myFile);
	fwrite(&infoHeader.height, 4, 1, myFile);
	fwrite(&infoHeader.planes, 2, 1, myFile);
	fwrite(&infoHeader.bpp, 2, 1, myFile);
	fwrite(&infoHeader.compress, 4, 1, myFile);
	fwrite(&infoHeader.imgsize, 4, 1, myFile);
	fwrite(&infoHeader.bpmx, 4, 1, myFile);
	fwrite(&infoHeader.bpmy, 4, 1, myFile); 
	fwrite(&infoHeader.colors, 4, 1, myFile);
	fwrite(&infoHeader.imxtcolors, 4, 1, myFile);
	// Se escribe la imagen binarizada
	int i, j;
	for(i = 0; i < infoHeader.height; i++)
		for(j = 0; j < infoHeader.width; j++)
		{
			fwrite(&myImage->matrix[i][j].red, 1, 1, myFile);
			fwrite(&myImage->matrix[i][j].green, 1, 1, myFile);
			fwrite(&myImage->matrix[i][j].blue, 1, 1, myFile);
			if(myImage->header.bpp == 32)
 				fwrite(&myImage->matrix[i][j].alpha, 1, 1, myFile);
		}
	fclose(myFile);
	free(route);
	free(number);
}