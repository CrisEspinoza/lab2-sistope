# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/poll.h>
# include "../utils/structs.h"
# include "../utils/bmplib.c"

# define READ 0
# define WRITE 1

/*
Entrada: Enteros que representan la anchura y altura de la imagen.
Descripción: Reserva memoria para la representación de una imagen.
Salida: Estructura Image con memoria reservada.
*/
Image* initImage(int height, int width)
{
	Image* myImage = (Image*)malloc(sizeof(Image));
 	myImage->height = height;
 	myImage->width = width;
 	myImage->isNearlyBlack = -1;
 	myImage->matrix = (Pixel**) malloc(sizeof(Pixel*) * height);
 	if(!myImage->matrix)
 	{
 		perror("## NO HAY SUFICIENTE MEMORIA PARA LA ASIGNACION ##");
		exit(0);
 	}
 	int i;
 	for (i = 0; i < height; i++)
 	{
 		myImage->matrix[i] = (Pixel*)malloc(width * sizeof(Pixel*));
 		if(!myImage->matrix[i])
 		{
 			perror("## NO HAY SUFICIENTE MEMORIA PARA LA ASIGNACION ##");
			exit(0);
 		}
 	}
 	return myImage;
}
/*
Entrada: Archivo y dimensiones de la imagen.
Descripción: Se pide memoria para la imagen, posteriormente se almacenan en una matriz los valores RGB de cada pixel.
Salida: Estructura Image con matriz de pixeles listos.
*/
Image* loadImage(FILE* myFile, InfoHeader header)
{
 	int i, j;
 	int height = header.height;
 	int width = header.width;
 	Image* myImage = initImage(height, width);
 	myImage->header = header;
 	Pixel myPixel;
 	for(i = 0; i < myImage->height; i++)
 		for(j = 0; j < myImage->width; j++)
 		{
 			fread(&myPixel.red, sizeof(char), 1, myFile);
 			fread(&myPixel.green, sizeof(char), 1, myFile);
 			fread(&myPixel.blue, sizeof(char), 1, myFile);
 			if(myImage->header.bpp == 32)
 				fread(&myPixel.alpha, sizeof(char), 1, myFile);
 			myImage->matrix[i][j] = myPixel; 			
 		}
 	return myImage;
}
/*
Entrada: Nombre de un fichero.
Descripción: Abre un archivo. Luego hace llamado a funciones que reservan memoria y almacenan la imagen.
Salida: Estructura Image con todos sus valores almacenados.
*/
Image* open(char* myFile)
{
 	FILE* file = fopen(myFile, "rb"); // Abrir el archivo en modo binario
	if(!file)
		exit(0);
	InfoHeader infoh = readInfoHeader(file);
	Image* myImage = loadImage(file, infoh);
	fclose(file);
	return myImage;
}

/* 1. Lectura de imagen RGB
Entrada: Nombre del archivo
Descripción: Abre la imagen, repleta la matriz RGB y almacena header.
Salida: Estructura Image con todos sus valores correspondientes.
*/
Image* readImg(char* name)
{
	char route[60];

	strcat(route, "../images/imagen_");
	strcat(route, name);
    strcat(route, ".bmp");

    Image* img = open(route);
	return img;
}

pid_t toGray(int* pipeIn)
{
	pid_t pid = fork();
	if(pid < 0)
	{
		printf("Fallo de fork() en apertura de imagen.\n");
		exit(-1);
	}
	else if(pid > 0)
	{
		//close(pipeIn[READ]);
		return pid;
	}
	else
	{
		close(pipeIn[WRITE]);
		dup2(pipeIn[READ], STDOUT_FILENO);

		execlp("./convertToGray", "-", NULL);
		perror("Fallo de execl()");
		exit(-1);
	}
}



int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		perror("## CANTIDAD DE ARGUMENTOS INVALIDA PARA LA APERTURA DE LA IMAGEN ##");
		exit(0);
	}

	int myPipeToGray[2];
	pipe(myPipeToGray);

	pid_t pidToGray = toGray(myPipeToGray);

	close(myPipeToGray[READ]);
	Image* img = readImg(argv[1]);
	
	write(myPipeToGray[WRITE], &img->height, sizeof(int));
	write(myPipeToGray[WRITE], &img->width, sizeof(int));
	write(myPipeToGray[WRITE], &img->header, sizeof(InfoHeader));

	int i, j;
	for(i = 0; i < img->height; i++)
		for(j = 0; j < img->width; j++)
		{
			write(myPipeToGray[WRITE], &img->matrix[i][j].red, sizeof(unsigned char));
			write(myPipeToGray[WRITE], &img->matrix[i][j].blue, sizeof(unsigned char));
			write(myPipeToGray[WRITE], &img->matrix[i][j].green, sizeof(unsigned char));
			write(myPipeToGray[WRITE], &img->matrix[i][j].alpha, sizeof(unsigned char));
		}

	wait(&pidToGray);

	perror("Conversion a grises hecho....");

	write(STDOUT_FILENO, "Funca", 10);

	return 0;
}