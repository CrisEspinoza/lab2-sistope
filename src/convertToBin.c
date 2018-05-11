
# include <stdio.h>
# include <stdlib.h>

# include "../utils/structs.h"


/* 3. Binarización de imagen
Entrada: Entra el umbral ingresado por pantalla y una imagen 
Descripción: Convierte los pixeles segun una clasificación ingresada por pantalla y revisa si le corresponde 0 o 255
Salida: Entraga la imagen con sus pixeles modificados
*/
Image* convertToBin (int umbral, Image* myImage)
{

	int i,j;
	for (i = 0; i < myImage->height; i++)
	{
		for (j = 0; j < myImage->width; j++)
		{
			if (myImage->matrix[i][j].red > umbral)
			{
				myImage->matrix[i][j].red = 255;
				myImage->matrix[i][j].blue = 255;
				myImage->matrix[i][j].green = 255;
			}
			else
			{
				myImage->matrix[i][j].red = 0;
				myImage->matrix[i][j].blue = 0;
				myImage->matrix[i][j].green = 0;
			}
		}
	}
	return myImage;
}

int main() 
{
	
}