/****************************************************************************
|*  Exporter.cpp
|*
|*  Exporter class to export Images.
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

#include "Exporter.h"


Exporter::Exporter(void) {
}


Exporter::~Exporter(void) {
}


// export an rgb image buffer to file
bool Exporter::exportImage(float * buffer, int width, int height) {
	unsigned char *image = new unsigned char[height*width*3];

	//scale pixel values to [0;255] and flip y axis
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int c = 0; c < 3; c++) {
				image[(height-1-y)*width*3 + x*3 + c] = (unsigned char) (255 * buffer[y*width*3 + x*3 + c]);
			}
		}
	}

	//write file
	bool ret = write24BitBmpFile("exportImage.bmp", (unsigned int) width, (unsigned int) height, image);

	delete(image);
	return ret;
}


//this code has been taken from http://home.comcast.net/~greg_slabaugh/personal/c/bmpwrite.html
bool Exporter::write24BitBmpFile(char *filename, unsigned int width, unsigned int height, unsigned char *image) {

	#ifdef _WIN32

	BITMAPINFOHEADER bmpInfoHeader;
	BITMAPFILEHEADER bmpFileHeader;
	FILE *filep;

	unsigned int row, column;
	unsigned int extrabytes, bytesize;
	unsigned char *paddedImage = NULL, *paddedImagePtr, *imagePtr;


	/* The .bmp format requires that the image data is aligned on a 4 byte boundary.  For 24 - bit bitmaps,
	   this means that the width of the bitmap  * 3 must be a multiple of 4. This code determines
	   the extra padding needed to meet this requirement. */
   extrabytes = (4 - (width * 3) % 4) % 4;


	// This is the size of the padded bitmap
	bytesize = (width * 3 + extrabytes) * height;


	// Fill the bitmap file header structure
	bmpFileHeader.bfType = 'MB';   // Bitmap header
	bmpFileHeader.bfSize = 0;      // This can be 0 for BI_RGB bitmaps
	bmpFileHeader.bfReserved1 = 0;
	bmpFileHeader.bfReserved2 = 0;
	bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


	// Fill the bitmap info structure
	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoHeader.biWidth = width;
	bmpInfoHeader.biHeight = height;
	bmpInfoHeader.biPlanes = 1;
	bmpInfoHeader.biBitCount = 24;            // 8 - bit bitmap
	bmpInfoHeader.biCompression = BI_RGB;
	bmpInfoHeader.biSizeImage = bytesize;     // includes padding for 4 byte alignment
	bmpInfoHeader.biXPelsPerMeter = 0;
	bmpInfoHeader.biYPelsPerMeter = 0;
	bmpInfoHeader.biClrUsed = 0;
	bmpInfoHeader.biClrImportant = 0;


	// Open file
	if ((filep = fopen(filename, "wb")) == NULL) {
		printf("Error opening file %s\n", filename);
		return false;
	}


	// Write bmp file header
	if (fwrite(&bmpFileHeader, 1, sizeof(BITMAPFILEHEADER), filep) < sizeof(BITMAPFILEHEADER)) {
		printf("Error writing bitmap file header\n");
		fclose(filep);
		return false;
	}


	// Write bmp info header
	if (fwrite(&bmpInfoHeader, 1, sizeof(BITMAPINFOHEADER), filep) < sizeof(BITMAPINFOHEADER)) {
		printf("Error writing bitmap info header\n");
		fclose(filep);
		return false;
	}


	// Allocate memory for some temporary storage
	paddedImage = (unsigned char *)calloc(sizeof(unsigned char), bytesize);
	if (paddedImage == NULL) {
		printf("Error allocating memory \n");
		fclose(filep);
		return false;
	}


	/* This code does three things.  First, it flips the image data upside down, as the .bmp
	   format requires an upside down image.  Second, it pads the image data with extrabytes 
		number of bytes so that the width in bytes of the image data that is written to the
		file is a multiple of 4.  Finally, it swaps (r, g, b) for (b, g, r).  This is another
		quirk of the .bmp file format. */

	for (row = 0; row < height; row++) {
		imagePtr = image + (height - 1 - row) * width * 3;
		paddedImagePtr = paddedImage + row * (width * 3 + extrabytes);
		for (column = 0; column < width; column++) {
			*paddedImagePtr = *(imagePtr + 2);
			*(paddedImagePtr + 1) = *(imagePtr + 1);
			*(paddedImagePtr + 2) = *imagePtr;
			imagePtr += 3;
			paddedImagePtr += 3;
		}
	}


	// Write bmp data
	if (fwrite(paddedImage, 1, bytesize, filep) < bytesize) {
		printf("Error writing bitmap data\n");
		free(paddedImage);
		fclose(filep);
		return false;
	}


	// Close file
	fclose(filep);
	free(paddedImage);
	return true;

#else
	std::cout << "Image export not supported on non-windows systems." << std::endl;
	return false;
#endif
}
