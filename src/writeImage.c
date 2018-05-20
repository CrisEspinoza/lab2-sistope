# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>

# include "../utils/structs.h"
# include "../utils/bmplib.c"

# define READ 0
# define WRITE 1

/* 5. Escritura de resultados
Entrada: Recibe una bandera y una imagen y por ultimo el numero de la imagen
Descripción: Escribe el resultado de la imagen si es considerada nearly black, ademas escribe la imagen en formato creado anteriormente
Salida: -
*/
void response(char* image, Image* myImage, char* flag)
{
	if(myImage->isNearlyBlack == 1 && !strcmp(flag, "1"))
		printf("|  imagen_%s   |         yes       |\n", image);
	else 
		printf("|  imagen_%s   |         no        |\n", image);
}


int main(int argc, char *argv[])
{
	//perror("WRITE: Me ejecuto");
	Image* img = (Image*)malloc(sizeof(Image));

	read(STDIN_FILENO, &img->height, sizeof(int));
	read(STDIN_FILENO, &img->width, sizeof(int));
	read(STDIN_FILENO, &img->header, sizeof(InfoHeader));
	read(STDIN_FILENO, &img->isNearlyBlack, sizeof(int));
	img->matrix = (Pixel**)malloc(sizeof(Pixel*) * img->height);
	int i, j;

	for(i = 0; i < img->height; i++)
	{
		img->matrix[i] = (Pixel*)malloc(sizeof(Pixel) * img->width);
		for(j = 0; j < img->width; j++)
		{
			if(img->header.bpp == 32)
				read(STDIN_FILENO, &img->matrix[i][j].alpha, sizeof(unsigned char));
			read(STDIN_FILENO, &img->matrix[i][j].blue, sizeof(unsigned char));
			read(STDIN_FILENO, &img->matrix[i][j].green, sizeof(unsigned char));
			read(STDIN_FILENO, &img->matrix[i][j].red, sizeof(unsigned char));
		}
	}
	binToBmp(argv[7], img);
	response(argv[7], img, argv[5]);

}