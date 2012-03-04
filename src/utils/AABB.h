/****************************************************************************
|*  Raytracer Framework
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/

#ifndef _AABB_H
#define _AABB_H

#include <utils/Vector3.h>

struct AABB {
	AABB() {};
	AABB(Vector3 l, Vector3 u) { corners[0]=l; corners[1]=u; };


	//lower and higher corners
	Vector3 corners[2];
};

#endif
