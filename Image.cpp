// Image.c
// Edwin Cordon

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Image.h"

#define INFINITY 2147483648

Image::Image(uint32 width, uint32 height)
    :width(width), height(height){

    pixels = new double *[height];

    for (uint32 i = 0; i < height; i++)
        pixels[i] = new double [width];

}

Image::~Image(){

    for (uint32 i = 0; i < height; i++)    
            delete [] pixels[i];
    delete [] pixels;
}

uint32 Image::getWidth(){
	return width;
}

uint32 Image::getHeight(){
	return height;
}

void Image::scale(){

	double max = -INFINITY, min = INFINITY;
    uint32 x, y, count = 0;

    for (x = 0; x < height; x++){
        for (y = 0; y < width; y++){

            if (pixels[x][y] > max){

                // cap the magitude of pixel intensities to prevent huge ranges
                if (pixels[x][y] <= 3500) 
                    max = pixels[x][y];
                else
                    pixels[x][y] = 3500;
            }

            if (pixels[x][y] < min)
                min = pixels[x][y];
        }
    }

	for (x = 0; x < height; x++){
		for (y = 0; y < width; y++){
            if (pixels[x][y] > 1000){
                count++;    
            }
        }
    }
    
    // If the minimum intensity is negative, shift up the brightness of the image 
    // so that the smallest intensity is 0.
	
	if (min < 0){

		for (x = 0; x < height; x++){
			for (y = 0; y < width; y++){
				pixels[x][y] -= min;	// subtract negative min to add 			
			}
		}
	}
	
	// If the max intensity is greater than 1, scale the image so that the brightest 
    // pixels have an intensity of 1.

	if (max > 1){

		for (x = 0; x < height; x++){
			for (y = 0; y < width; y++){
                pixels[x][y] /= max;
			}
		}
	}


}

double Image::max(){

	double max = -10000;

	for (uint32 x = 0; x < height; x++){
		for (uint32 y = 0; y < width; y++){
			
			if (pixels[x][y] > max)
				max = pixels[x][y];		
		}
	}

	return max;
}

double Image::min(){

	double min = 10000;

	for (uint32 x = 0; x < height; x++){
		for (uint32 y = 0; y < width; y++){
			
			if (pixels[x][y] < min)
				min = pixels[x][y];		
		}
	}

	return min;
}

