/****************************************************************************
|*  ConstantShader.h
|*
|*  Declaration of the class ImageTexture. Inherits from ITexture.
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/


#ifndef _IMAGETEXTURE_H
#define _IMAGETEXTURE_H


#include <math.h>

#include <utils/Image.h>
#include <utils/Point.h>
#include <utils/textures/ITexture.h>



class ImageTexture : public ITexture  {

public:
	ImageTexture(Image* image);
	~ImageTexture(void);
	
	virtual Vector4 EvaluateTexture(Vector2 uv);

private:
	Image* m_image;

};


#endif //_IMAGETEXTURE_H
