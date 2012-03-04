/****************************************************************************
|*  Ray.cpp
|*
|*  Definition of the class Ray consisting of a point and a direction.
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/


#include "Ray.h"


Ray::Ray(Vector3 _point, Vector3 _direction) 
	:point(_point), direction(_direction.normalize()){
	
	depth = 0;

	min_t = 0.f;
	max_t = 3.4e38;

}

Ray::Ray(void) {
	point = Vector3(0.0, 0.0, 0.0);
	direction = Vector3(0.0, 0.0, 1.0);

	depth = 0;

	min_t = 0;
	max_t = 3.4e38;
}

Ray::~Ray(void){};

Vector3 Ray::getPointOnRay(double t) const {
	return point + (direction*t);
}

// epsilon t to avoid self intersection due to rounding errors
// if the ray starts exactly on an element
const double Ray::epsilon_t = 0.001;
