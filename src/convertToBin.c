# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/poll.h>

# include "../utils/structs.h"

# define READ 0
# define WRITE 1
 

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

pid_t toWrite(int* pipe)
{
	pid_t pid = fork();
	if(pid < 0)
	{
		perror("Fallo de fork() en binarizacion.");
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

		execlp("./classification", "-", NULL);
		perror("Fallo de execlp()");
		exit(-1);
	}
}

int main(int argc, char *argv[])
{
	perror("BIN: Me ejecuto");
	int myPipeToClass[2];
	pipe(myPipeToClass);

	Image* img = (Image*)malloc(sizeof(Image));

	read(STDIN_FILENO, &img->height, sizeof(int));
	read(STDIN_FILENO, &img->width, sizeof(int));
	read(STDIN_FILENO, &img->header, sizeof(InfoHeader));
	perror("BIN: Leo el header");

	img->matrix = (Pixel**)malloc(sizeof(Pixel*) * img->height);
	perror("BIN: Asigno memoria");
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
	perror("BIN: Lei toda la imagen");

	pid_t pidToClass = toWrite(myPipeToClass);

	img = convertToBin(10 ,img);
	write(myPipeToClass[WRITE], &img->height, sizeof(int));
	write(myPipeToClass[WRITE], &img->width, sizeof(int));
	write(myPipeToClass[WRITE], &img->header, sizeof(InfoHeader));
	for(i = 0; i < img->height; i++)
		for(j = 0; j < img->width; j++)
		{
			if(img->header.bpp == 32)
				write(myPipeToClass[WRITE], &img->matrix[i][j].alpha, sizeof(unsigned char));
			write(myPipeToClass[WRITE], &img->matrix[i][j].blue, sizeof(unsigned char));
			write(myPipeToClass[WRITE], &img->matrix[i][j].green, sizeof(unsigned char));
			write(myPipeToClass[WRITE], &img->matrix[i][j].red, sizeof(unsigned char));
		}
	perror("BIN: Escribi lso datos para mi hijo");
	wait(&pidToClass);
}