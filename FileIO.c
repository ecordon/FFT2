// FileIO.c
// Edwin Cordon

#include <stdio.h>
#include "FileIO.h"

typedef unsigned char byte;

void header( int row, int col, unsigned char head[32] );

IMAGE *readImage(char fname[], uint32 width, uint32 height){

	FILE *fp;
	uint32 i, j;
	IMAGE *image;

	image = createImage(width, height);

	if(!(fp = fopen(fname, "r"))){
		fprintf(stderr, "ERROR: Could not open %s for reading!\n", fname);
		return NULL;
	}

	for(j = 0; j < image->height; j++){
		for(i = 0; i < image->width; i++){
			setPixel(image, i, j, ((double) fgetc(fp))/255);
		}
	}

	fclose(fp);
	return image;
}	

int saveImage(char sname[], IMAGE *image){

	FILE *fp;
	uint32 i, j;
	byte head[32];

	if(!(fp = fopen(sname, "w"))){
		fprintf(stderr, "ERROR: Could not open %s for writing\n!", sname);
		return -1;	
	}

	header(image->height, image->width, head);
	fwrite(head, 4, 8, fp);
	for(j = 0; j < image->height; j++){
		for(i = 0; i < image->width; i++){
			//printf("%u\n", (byte) (255*getPixel(image, i, j)));
			fputc((byte) (255*getPixel(image, i, j)), fp);
		}
	}
	
	fclose(fp);
	return 0;
}

void header( int row, int col, unsigned char head[32] )
{
	int *p = (int *)head;
	char *ch;
	int num = row * col;

	/* Choose little-endian or big-endian header depending on the machine. Don't modify this */
	/* Little-endian for PC */
	
	*p = 0x956aa659;
	*(p + 3) = 0x08000000;
	*(p + 5) = 0x01000000;
	*(p + 6) = 0x0;
	*(p + 7) = 0xf8000000;

	ch = (char*)&col;
	head[7] = *ch;
	ch ++; 
	head[6] = *ch;
	ch ++;
	head[5] = *ch;
	ch ++;
	head[4] = *ch;

	ch = (char*)&row;
	head[11] = *ch;
	ch ++; 
	head[10] = *ch;
	ch ++;
	head[9] = *ch;
	ch ++;
	head[8] = *ch;
	
	ch = (char*)&num;
	head[19] = *ch;
	ch ++; 
	head[18] = *ch;
	ch ++;
	head[17] = *ch;
	ch ++;
	head[16] = *ch;
	
/*
	// Big-endian for unix
	*p = 0x59a66a95;
	*(p + 1) = col;
	*(p + 2) = row;
	*(p + 3) = 0x8;
	*(p + 4) = num;
	*(p + 5) = 0x1;
	*(p + 6) = 0x0;
	*(p + 7) = 0xf8;
*/
}

