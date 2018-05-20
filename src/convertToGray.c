# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/poll.h>

# include "../utils/structs.h"

# define READ 0
# define WRITE 1

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

pid_t toBin(int* pipe)
{
	pid_t pid = fork();
	if(pid < 0)
	{
		perror("Fallo de fork() en conversion a gris.");
		exit(-1);
	}
	else if(pid > 0)
	{
		close(pipe[READ]);
		return pid;
	}
	else
	{
		close(pipe[WRITE]);
		dup2(pipe[READ], STDIN_FILENO);

		execlp("./convertToBin", "-", NULL);
		perror("Fallo de execlp()");
		exit(-1);
	}
}

int main(int argc, char *argv[])
{
	perror("GRAY: Me ejecuto");
	int myPipeToBin[2];
	pipe(myPipeToBin);
	pid_t pidToBin = toBin(myPipeToBin);
	
	Image* img = (Image*)malloc(sizeof(Image));
	perror("GRAY: Creo estructura de datos");
	read(STDIN_FILENO, &img->height, sizeof(int));
	read(STDIN_FILENO, &img->width, sizeof(int));
	read(STDIN_FILENO, &img->header, sizeof(InfoHeader));
	img->matrix = (Pixel**)malloc(sizeof(Pixel*) * img->height);
	perror("GRAY: Matriz");
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
	perror("GRAY: Creo el proceso para binarizar");
	img = convertToGray(img);
	perror("GRAY: Escribo los datos de la imagen en el pipe");
	write(myPipeToBin[WRITE], &img->height, sizeof(int));
	write(myPipeToBin[WRITE], &img->width, sizeof(int));
	write(myPipeToBin[WRITE], &img->header, sizeof(InfoHeader));
	for(i = 0; i < img->height; i++)
		for(j = 0; j < img->width; j++)
		{
			if(img->header.bpp == 32)
				write(myPipeToBin[WRITE], &img->matrix[i][j].alpha, sizeof(unsigned char));
			write(myPipeToBin[WRITE], &img->matrix[i][j].blue, sizeof(unsigned char));
			write(myPipeToBin[WRITE], &img->matrix[i][j].green, sizeof(unsigned char));
			write(myPipeToBin[WRITE], &img->matrix[i][j].red, sizeof(unsigned char));
		}
	perror("GRAY: Espero a mi hijo");
	wait(&pidToBin);
	perror("GRAY: Termino");
	return 0;
}