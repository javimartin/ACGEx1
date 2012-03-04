/****************************************************************************
|*  Ray.h
|*
|*  Declaration of the class Ray consisting of a point and a direction.
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/


#ifndef _RAY_H
#define _RAY_H


#include <utils/Vector3.h>


class Ray {

public:

	Ray(Vector3 _point, Vector3 _direction);

	Ray(void);

	~Ray(void);

	Vector3 getPointOnRay(double t) const;

	Vector3 point;
	Vector3 direction; //should be normalized

	double min_t;
	double max_t;

	//The recursion depth of this ray
	unsigned int depth;

	// epsilon t to avoid self intersection due to rounding errors
	// if the ray starts exactly on an element
	static const double epsilon_t;

};


#endif //_RAY_H