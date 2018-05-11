all:
	gcc src/processHandler.c -o obj/processHandler -Wall
	gcc src/imageReader.c -o obj/imageReader -Wall
	gcc src/convertToGray.c -o obj/convertToGray -Wall
	gcc src/convertToBin.c -o obj/convertToBin -Wall
	gcc src/classification.c -o obj/classification -Wall
	gcc src/writeImage.c -o obj/writeImage -Wall