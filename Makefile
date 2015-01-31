# Makefile for FFT2 program

CC = g++
CFLAGS = -c -Wall
OBJECTS = FFT2.o Complex.o FileIO.o Image.o
EXECUTABLE = FFT2

all: $(OBJECTS) $(EXECUTABLE)
clean: 
	rm $(OBJECTS) $(EXECUTABLE)

FFT2: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE)

Image.o: Image.cpp Image.h
	$(CC) $(CFLAGS) Image.cpp

FileIO.o: FileIO.cpp FileIO.h Image.h
	$(CC) $(CFLAGS) FileIO.cpp

Complex.o: Complex.cpp Complex.h Image.h
	$(CC) $(CFLAGS) Complex.cpp

FFT2.o: FFT2.cpp Complex.h FileIO.h Image.h
	$(CC) $(CFLAGS) FFT2.cpp

FFT2orig.o: FFT2orig.cpp Complex.h FileIO.h Image.h
	$(CC) $(CFLAGS) FFT2orig.cpp
