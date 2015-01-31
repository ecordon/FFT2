// FFT2.cpp
// Edwin Cordon
//
// Program for computing the FFT of an image.
// FFT2 takes a grayscale raw image and saves
// the magnitude of the frequency spectrum into
// a ppm image.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "Complex.h"
#include "Image.h"
#include "FileIO.h"

#define PI	3.14159265359

#define debug   0

using namespace std;

typedef unsigned char byte;

void DFT2(Complex **F, int N, int M, double **f, int width, int height);
void FFT2(Complex **F, int N, int M, double **f, int width, int height);

void getMagnitude(double **magnitude, Complex **F, int N, int M);
void shiftFFT2(double **magnitude, int width, int height);

uint getPaddedLength(uint length); 
void delete2DArray(double **A, uint32 height);
void delete2DArray(Complex **Z, uint32 height);

int trace(bool on, const char *format, ...);

int main(int argc, char *argv[]){

    char *fname, *sname;
    uint32 width, height, M, N;
    int i = 1;

    if (argc != 7){
        fprintf(stderr, "./FFT2 -i <input resolution> <input file> -o <FFT resolution> <output file>\n");
        return -1;
    }

    while (i < argc){

        if (argc - i >= 3){
        
            if ((0 == strcmp(argv[i], "-i"))){
    
                sscanf(argv[i+1], "%ux%u\n", &width, &height);
                
                fname = (char *)malloc(strlen(argv[i+2]));
                strcpy(fname, argv[i+2]);
                strcat(fname, ".raw");

                i += 3;
            }

            if ((0 == strcmp(argv[i], "-o"))){

                sscanf(argv[i+1], "%ux%u\n", &N, &M);

                sname = (char *)malloc(strlen(argv[i+2]));
                strcpy(sname, argv[i+2]);
                strcat(sname, "FFT.ppm");

                i += 3;
            }
        }
    }
                

    // Create images
    
    // Although the user specifies a certain resolution for the frequency
    // spectrum of the image, we modify these dimensions slightly so that 
    // the FFT can be calculated as fast as possible.
    N = getPaddedLength(N);
    M = getPaddedLength(M);

    printf("M = %u, N = %u\n", M, N);

    Image f(width, height);

    Complex **F =  new Complex *[M];
    for(uint32 i = 0; i < M; i++)
        F[i] = new Complex [N];
    
    Image AbsF(N, M);


    readImage(fname, f);

    // Calculate the 2D Fast Fourier transform of the input 
    // image and create another image of its magnitude
    // spectrum

    printf("Calculating FFT\n");
    FFT2(F, N, M, f.pixels, f.getWidth(), f.getHeight());
    printf("Getting FFT magnitude\n");
    getMagnitude(AbsF.pixels, F, N, M);
    printf("Scaling FFT magnitude\n");
    AbsF.scale();
    printf("Shifting FFT magnitude\n");
    shiftFFT2(AbsF.pixels, N, M);


    saveImage(sname, AbsF);

    for(uint32 i = 0; i < M; i++)
        delete [] F[i];
    delete [] F;

    free(fname);
    free(sname);

	return 0;
}

void DFT2(Complex **F, int N, int M, double **f, int width, int height){

	for(int U = 0; U < M; U++){
	    for(int V = 0; V < N; V++){

			for(int x = 0; x < height; x++){
				for(int y = 0; y < width; y++){

					Complex W(1.0, -2*PI*((x/(double)M)*U + (y/(double)N)*V));
                    F[U][V] += W*f[x][y];
				}
			}
		}
	}
}

void FFT2(Complex **F, int N, int M, double **f, int width, int height){

    trace(debug, "Starting %dx%d FFT\n", N, M);

    if(M == 1 && N == 1){ 
        F[0][0] = f[0][0];
    }
    else if(M%2 == 0 && N%2 == 0){
            
        Complex **EM_EN = new Complex *[M/2];
        Complex **EM_ON = new Complex *[M/2];
        Complex **OM_EN = new Complex *[M/2];
        Complex **OM_ON = new Complex *[M/2];

        double **em_en = new double *[height/2];
        double **em_on = new double *[height/2];
        double **om_en = new double *[height/2];
        double **om_on = new double *[height/2];

        for(int i = 0; i < ((M/2 > height/2) ? M/2: height/2); i++){
                
            if(i < M/2){ 
                
                EM_EN[i] = new Complex[N/2];
                EM_ON[i] = new Complex[N/2];
                OM_EN[i] = new Complex[N/2];
                OM_ON[i] = new Complex[N/2];
            }
            
            if(i < height/2){

                em_en[i] = new double[width/2];
                em_on[i] = new double[width/2];
                om_en[i] = new double[width/2];
                om_on[i] = new double[width/2];
            }
        }

        // Split all 4 quadrants of the image
        for(int m = 0; m < height/2; m++){
            for(int n = 0; n < width/2; n++){

                em_en[m][n] = f[2*m][2*n];
                em_on[m][n] = f[2*m][2*n+1];
                om_en[m][n] = f[2*m+1][2*n];
                om_on[m][n] = f[2*m+1][2*n+1];
            }
        }

        // Take the FFT of each quadrant
        FFT2(EM_EN, N/2, M/2, em_en, width/2, height/2);
        FFT2(EM_ON, N/2, M/2, em_on, width/2, height/2);
        FFT2(OM_EN, N/2, M/2, om_en, width/2, height/2);
        FFT2(OM_ON, N/2, M/2, om_on, width/2, height/2);

        delete2DArray(em_en, height/2);
        delete2DArray(em_on, height/2);
        delete2DArray(om_en, height/2);
        delete2DArray(om_on, height/2);
            
        // Do the FFT calculation
        for(int U = 0; U < M/2; U++){
            for(int V = 0; V < N/2; V++){
                
                Complex W1(1.0, -2*PI*((double)V/(double)N));
                Complex W2(1.0, -2*PI*((double)U/(double)M));
                Complex W3(1.0, -2*PI*(((double)U/(double)M) + ((double)V/(double)N)));

                // 0 <= U < M/2, 0 <= V < N/2
                F[U][V] = EM_EN[U][V] + W1*EM_ON[U][V] + W2*OM_EN[U][V] + W3*OM_ON[U][V];
                
                // 0 <= U < M/2, N/2 <= V < N
                F[U][V+N/2] = EM_EN[U][V] - W1*EM_ON[U][V] + W2*OM_EN[U][V] - W3*OM_ON[U][V];

                // M/2 <= U < M, 0 <= V < N/2
                F[U+M/2][V] = EM_EN[U][V] + W1*EM_ON[U][V] - W2*OM_EN[U][V] - W3*OM_ON[U][V];
                
                // M/2 <= U < M, N/2 <= V < N
                F[U+M/2][V+N/2] = EM_EN[U][V] - W1*EM_ON[U][V] - W2*OM_EN[U][V] + W3*OM_ON[U][V];
            }
        }

        delete2DArray(EM_EN, M/2);
        delete2DArray(EM_ON, M/2);
        delete2DArray(OM_EN, M/2);
        delete2DArray(OM_ON, M/2);
    }
}

void getMagnitude(double **magnitude, Complex **F, int N, int M){

	for(int U = 0; U < M; U++){
		for(int V = 0; V < N; V++){
           
            magnitude[U][V] = F[U][V].getMagnitude();
            trace(debug, "|F[%d][%d]| = %lf\n", U, V, magnitude[U][V]);
		}
	}
}

void shiftFFT2(double **magnitude, int width, int height){

    double temp;

    for(int x = 0; x < height/2; x++){
        for(int y = 0; y < width/2; y++){

            // swap top-left and bottom-right quadrants
            temp = magnitude[x][y]; 
            magnitude[x][y] = magnitude[x+height/2][y+width/2];
            magnitude[x+height/2][y+width/2] = temp;

            // swap bottom-left and top-right quadrants
            temp = magnitude[x][y+width/2];
            magnitude[x][y+width/2] = magnitude[x+height/2][y];
            magnitude[x+height/2][y] = temp;
        }
    }
}

uint32 getPaddedLength(uint length){

    uint32 i, mask, numBits;
    uint32 msb = 0, asserts = 0;

    // Look for most significant bit of the input length
    // to determine the proper padding.
    
    numBits = sizeof(uint32)*8;
    mask = (1 << (numBits - 1));

    for (i = 0; i < numBits; i++){
        
        if (((length << i) & mask)){
            
            if (msb == 0)
                msb = numBits - i - 1;

            asserts++;
        }
    }

    if (asserts <= 1)
        return length;
    else
        return (1 << (msb + 1));
}

void delete2DArray(double **A, uint32 height){

    for(uint32 i = 0; i < height; i++)
        delete [] A[i];
    delete [] A;
}

void delete2DArray(Complex **Z, uint32 height){

    for(uint32 i = 0; i < height; i++)
        delete [] Z[i];
    delete [] Z;
}

int trace(bool on, const char *format, ...){

    if (on){

        va_list arg;
        int status;

        va_start(arg, format);
        status = vfprintf(stdout, format, arg);
        va_end(arg);

        return status;
    }
    else
        return 0;
}

