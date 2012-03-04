/****************************************************************************
|*  Exporter.h
|*
|*  Declaration of an Exporter class to export Images.
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/

#ifndef _EXPORTER_H
#define _EXPORTER_H


class Exporter {

public:

	Exporter(void);

	~Exporter(void);
	
	// export an rgb image buffer to file
	bool exportImage(float * buffer, int width, int height); 

private:
	bool write24BitBmpFile(char *filename, unsigned int width, unsigned int height, unsigned char *image);
};

#endif //_EXPORTER_H
