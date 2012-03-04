/****************************************************************************
|*  ITexture.h
|*
|*  Abstract Base Class definition of a texture. 
|*   
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/


#ifndef _ITEXTURE_H
#define _ITEXTURE_H


#include <utils/Vector2.h>
#include <utils/Vector4.h>


class ITexture {

public:
	virtual ~ITexture(void) {};
	
	virtual Vector4 EvaluateTexture(Vector2 uv) = 0;
};


#endif //_ITEXTURE_H
