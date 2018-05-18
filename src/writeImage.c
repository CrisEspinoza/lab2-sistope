# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/poll.h>

# include "../utils/structs.h"

# define READ 0
# define WRITE 1


void binToBmp(int image, Image* myImage)
{
	char* route;
	char* number;
	route = (char*)malloc(sizeof(char)*50);
	number = (char*)malloc(sizeof(char)*50);
	strcpy(route,"../imagesbin/imagenbin_");
	InfoHeader infoHeader = myImage->header;
	sprintf(number, "%d", image);
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

/* 5. Escritura de resultados
Entrada: Recibe una bandera y una imagen y por ultimo el numero de la imagen
Descripción: Escribe el resultado de la imagen si es considerada nearly black, ademas escribe la imagen en formato creado anteriormente
Salida: -
*/
void response(int image, Image* myImage, int flag)
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
}


int main(int argc, char *argv[])
{
	Image* img = (Image*)malloc(sizeof(Image));

	read(STDOUT_FILENO, &img->height, sizeof(int));
	read(STDOUT_FILENO, &img->width, sizeof(int));
	read(STDOUT_FILENO, &img->header, sizeof(InfoHeader));

	img->matrix = (Pixel**)malloc(sizeof(Pixel*) * img->height);
	int i, j;

	for(i = 0; i < img->height; i++)
	{
		img->matrix[i] = (Pixel*)malloc(sizeof(Pixel) * img->width);
		for(j = 0; j < img->width; j++)
		{
			if(img->header.bpp == 32)
				read(STDOUT_FILENO, &img->matrix[i][j].alpha, sizeof(unsigned char));
			read(STDOUT_FILENO, &img->matrix[i][j].blue, sizeof(unsigned char));
			read(STDOUT_FILENO, &img->matrix[i][j].green, sizeof(unsigned char));
			read(STDOUT_FILENO, &img->matrix[i][j].red, sizeof(unsigned char));
		}
	}
	perror("Llega sano y salvo a escribir la imagen");
	binToBmp(1, img);
	response(1, img, 1);

}