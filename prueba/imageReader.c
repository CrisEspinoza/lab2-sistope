#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

int openImage(int image)
{
	return image;
}


int main(int argc, char *argv[])
{
	Image* image = (Image*)malloc(sizeof(Image));
	image->height = 1;
	write(STDOUT_FILENO, "HOLA MUNDO", 33);
	return 0;
}