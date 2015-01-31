// Image.h
// Edwin Cordon

#ifndef IMAGE_H
#define IMAGE_H

typedef unsigned int uint32;

class Image {

	public:

		Image(uint32 width, uint32 height);
		~Image();

		uint32 getWidth();
		uint32 getHeight();

		void scale();
		double min();
		double max();

        double **pixels;

	private:
		uint32 width;
		uint32 height;
};


#endif

