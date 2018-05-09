# include <stdio.h>
# include <stdlib.h>

# include "structs.h"
# include "imagelib.h"


void controlerTheProces (int quantityImages ,int flag, int umbralClassification, int umbralImages)
{
	int i ;
	Image* img;
	char* name;
	char* number;

	for (i = 0; i < quantityImages; i++)
	{
		//printf("Comienzo de procesamiento de imagen %d \n", (i+1));
		name = (char*)malloc(sizeof(char)*50);
		number = (char*)malloc(sizeof(char)*50);
		img = (Image*)malloc(sizeof(Image));

	    freeImage(img);
	    free(name);
	    free(number);

	}
}

void freeImage (Image* myImage)
{
	int i;
	for (i = 0; i < myImage->height; i++)
	{	
		free(myImage->matrix[i]);
	}
	free(myImage->matrix);
	free(myImage);
}