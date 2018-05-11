# include <stdio.h>
# include <stdlib.h>

# include "../utils/structs.h"

/* 5. Escritura de resultados
Entrada: Recibe una bandera y una imagen y por ultimo el numero de la imagen
Descripción: Escribe el resultado de la imagen si es considerada nearly black, ademas escribe la imagen en formato creado anteriormente
Salida: -
*/
void writeImg(int image, Image* myImage, int flag)
{
	if (flag == 1)
	{
		printf("\n._________________________________.\n");
		printf("|     image   |    nearly black   |\n");
		printf("|---------------------------------|\n");
	}
	if(myImage->isNearlyBlack == 1 && flag == 1)
		printf("|  imagen_%i   |         yes       |\n", image);
	else if (flag == 1 )
		printf("|  imagen_%i   |         no        |\n", image);
	//binToBmp(image, myImage); // Funcion que transforma la matriz binarizada a BMP.

	if (flag == 1)
		printf("|_________________________________|\n");
	printf("Proceso terminado\n");
}


int main()
{
	
}