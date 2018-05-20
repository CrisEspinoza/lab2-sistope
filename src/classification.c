# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/poll.h>

# include "../utils/structs.h"
# include "../utils/bmplib.c"

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

pid_t toWrite(int* pipe, char* nblack, char* umbral, char* flag, char* image)
{
	pid_t pid = fork();
	if(pid < 0)
	{
		perror("Fallo de fork() en la clasificacion de la imagen.");
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

		execlp("./writeImage", "-n", nblack, "-u", umbral, "-f", flag, "-i", image, NULL);
		perror("Fallo de execlp()");
		exit(-1);
	}
}

int main(int argc, char *argv[])
{
	//perror("CLASS: Inicio");
	int myPipeToWrite[2];
	pipe(myPipeToWrite);

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

	pid_t pidToWrite = toWrite(myPipeToWrite, argv[1], argv[3], argv[5], argv[7]);

	classification(strToInt(argv[3]), img);

	write(myPipeToWrite[WRITE], &img->height, sizeof(int));
	write(myPipeToWrite[WRITE], &img->width, sizeof(int));
	write(myPipeToWrite[WRITE], &img->header, sizeof(InfoHeader));
	write(myPipeToWrite[WRITE], &img->isNearlyBlack, sizeof(int));
	for(i = 0; i < img->height; i++)
	{
		for(j = 0; j < img->width; j++)
		{
			if(img->header.bpp == 32)
				write(myPipeToWrite[WRITE], &img->matrix[i][j].alpha, sizeof(unsigned char));
			write(myPipeToWrite[WRITE], &img->matrix[i][j].blue, sizeof(unsigned char));
			write(myPipeToWrite[WRITE], &img->matrix[i][j].green, sizeof(unsigned char));
			write(myPipeToWrite[WRITE], &img->matrix[i][j].red, sizeof(unsigned char));
		}
	}
	wait(&pidToWrite);
	//perror("CLASS: Fin");
	
}