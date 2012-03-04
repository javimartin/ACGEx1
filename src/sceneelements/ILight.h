/****************************************************************************
|*  ILight.h
|*
|*  Abstract Base Class of a light source.
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/


#ifndef _ILIGHT_H
#define _ILIGHT_H


#include <utils/Vector4.h>
#include <utils/Ray.h>


class ILight {

public:
	virtual ~ILight(void) {};

	virtual Ray generateRay(Vector3 endPoint) = 0;
	virtual Vector4 getColor(void) const = 0;
	virtual Vector3 getPosition(void) const = 0;
	
};


#endif //_ILIGHT_H
