/****************************************************************************
|*  ICamera.h
|*
|*  Abstract Base Class definition of a camera.
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/


#ifndef _ICAMERA_H
#define _ICAMERA_H


#include <rendererelements/Sampler/Sample.h>
#include <utils/Ray.h>
#include <utils/Point.h>
#include <utils/Vector3.h>


class ICamera {

public:
	virtual ~ICamera(void) {};

	//generate ray out of a image sample
	virtual Ray generateRay(const Sample& sample) = 0;

	virtual Point getResolution(void) = 0;

	virtual Vector3 getPosition(void) = 0;

	//write sample to image buffer
	virtual void setSample(const Sample& s) = 0;

	//get color on image pixel
	virtual Vector3 getPixelColor(int pos_x, int pos_y) = 0;

	//get image buffer
	virtual float * getFilm(void) = 0;

	// clear image buffer
	virtual void initFilm(void) = 0;

};


#endif //_ICAMERA_H
