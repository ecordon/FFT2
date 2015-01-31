// FileIO.h
// Edwin Cordon

#ifndef FILEIO_H
#define FILEIO_H

#include "FileIO.h"
#include "Image.h"

int readImage1(char fname[], double **image, uint32 width, uint32 height);
int saveImage1(char sname[], double **image, uint32 width, uint32 height); 

// Read .raw image 
int readImage(char fname[], Image &image);
// Save image as .ppm file
int saveImage(char sname[], Image &image);

#endif

