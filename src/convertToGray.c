# include <stdio.h>
# include <stdlib.h>

# include "../utils/structs.h"

/* 2. Conversión a imagen en escala de grises
Entrada: Entra una imagen
Descripción: Convierto los componentes de la imagen a escala de grises 
Salida: Imagen en escala de grises
*/
Image* convertToGray(Image* myImage)
{
	int tmp, i, j;
	for (i = 0; i < myImage->height; i++)
		for (j = 0; j < myImage->width; j++)
		{
			// Y = R ∗ 0.3 + G ∗ 0.59 + B ∗ 0.11
			tmp = myImage->matrix[i][j].red * 0.3 + myImage->matrix[i][j].green * 0.59 + myImage->matrix[i][j].blue * 0.11;
			myImage->matrix[i][j].red = tmp;
			myImage->matrix[i][j].green = tmp;
			myImage->matrix[i][j].blue = tmp;
		}
	return myImage;
}

int main(int argc, char *argv[])
{
	
	//write(STDOUT_FILENO, myImage, 33);
	return 0;
}