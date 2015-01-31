// FileIO.c
// Edwin Cordon

#include <stdio.h>
#include <iostream>
#include <string.h>

#include "FileIO.h"

using namespace std;

typedef unsigned char byte;

void header( int row, int col, unsigned char head[32] );

int readImage1(char fname[], double **image, uint32 width, uint32 height){

    FILE *fp;
    uint32 x, y;

    try{
        if(!(fp = fopen(fname, "r"))){
            throw 1;
        }
    }
    catch(int e){
        fprintf(stderr, "ERROR: Could not open %s for reading!\n", fname);
        return e;
    }

    for(x = 0; x < height; x++){
        for(y = 0; y < width; y++){
            image[x][y] = ((double)fgetc(fp))/255;
        }
    }

    return 0;
}

int saveImage1(char sname[], double **image, uint32 width, uint32 height){

    FILE *fp;
    uint32 x, y;
	byte head[32];

    try{
        if(!(fp = fopen(sname, "w"))){
            throw 1;
        }
    }
    catch(int e){
        fprintf(stderr, "ERROR: Could not open %s for reading!\n", sname);
        return e;
    }

	header(height, width, head);
	fwrite(head, 4, 8, fp);
    for(x = 0; x < height; x++){
        for(y = 0; y < width; y++){
            fputc((unsigned char)(image[x][y]*255), fp);
        }
    }

    return 0;
}

int readImage(char fname[], Image &image){

    FILE *fp;
    
    if(!(fp = fopen(fname, "r"))){
        fprintf(stderr, "Cannot open %s!\n", fname);
        return -1;
    }

	for(uint32 x = 0; x < image.getHeight(); x++){
		for(uint32 y = 0; y < image.getWidth(); y++){
			image.pixels[x][y] = ((double) fgetc(fp))/255;
        }
	}
    printf("%s read successfully!\n", fname);

	fclose(fp);
    return 0;
}

int saveImage(char sname[], Image &image){

	FILE *fp;
    unsigned char pixel;

	if(!(fp = fopen(sname, "w"))){
		fprintf(stderr, "Cannot save %s!\n", sname);
		return -1;
	}

	fprintf(fp, "P6\n");
	fprintf(fp, "%u %u\n", image.getWidth(), image.getHeight());
	fprintf(fp, "255\n");	

	for(uint32 x = 0; x < image.getHeight(); x++){
		for(uint32 y = 0; y < image.getWidth(); y++){			
            
            pixel = (unsigned char) (255*image.pixels[x][y]);

            fputc(pixel, fp);
            fputc(pixel, fp);
            fputc(pixel, fp);
        }
	}

	printf("%s saved successfully!\n", sname);

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

