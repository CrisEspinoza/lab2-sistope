# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/poll.h>

# include "../utils/structs.h"

# define READ 0
# define WRITE 1
 
int strToInt(char* number)
{
	int i;
	int response = 0;
	for(i = 0; number[i] != '\0'; i++)
	{
		response = response * 10;
		if(number[i] == '1')
			response = response + 1;
		if(number[i] == '2')
			response = response + 2;
		if(number[i] == '3')
			response = response + 3;
		if(number[i] == '4')
			response = response + 4;
		if(number[i] == '5')
			response = response + 5;
		if(number[i] == '6')
			response = response + 6;
		if(number[i] == '7')
			response = response + 7;
		if(number[i] == '8')
			response = response + 8;
		if(number[i] == '9')
			response = response + 9;
	}
	return response;
}


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

pid_t toWrite(int* pipe, char* nblack, char* umbral, char* flag, char* image)
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

		execlp("./classification", "-n", nblack, "-u", umbral, "-f", flag, "-i", image, NULL);
		perror("Fallo de execlp()");
		exit(-1);
	}
}

int main(int argc, char *argv[])
{
	//perror("BIN: Inicio");
	int myPipeToClass[2];
	pipe(myPipeToClass);

	Image* img = (Image*)malloc(sizeof(Image));

	read(STDIN_FILENO, &img->height, sizeof(int));
	read(STDIN_FILENO, &img->width, sizeof(int));
	read(STDIN_FILENO, &img->header, sizeof(InfoHeader));

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

	pid_t pidToClass = toWrite(myPipeToClass, argv[1], argv[3], argv[5], argv[7]);

	img = convertToBin(strToInt(argv[3]) ,img);

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
	wait(&pidToClass);
	//perror("BIN: Fin");
}