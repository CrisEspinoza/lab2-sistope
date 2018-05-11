# include <stdio.h>
# include <stdlib.h>

# include "../utils/structs.h"

/*4. Análisis de propiedad

Entrada: Recibe como parametro un umbral y una imagen 
Descripción: EL umbral indica cuanto pocentaje de la imagen debe tener para ser clasificada nearly black y esta es evaluada en la imagen
Salida: -
*/
void classification (int umbral ,Image* myImage) 
{
	//1 = negro
	int black = 0;
	int i,j,percentage;
	for (i = 0; i < myImage->height; i++)
		for (j = 0; j < myImage->width; j++)
		{
			if (myImage->matrix[i][j].red == 255)
				black++;
		}
	percentage = (black * 100)/(myImage->height * myImage->width); 
	if(percentage >= umbral)
		myImage->isNearlyBlack = 1;
	else
		myImage->isNearlyBlack = 0;	
}

int main()
{
	
}