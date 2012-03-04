/****************************************************************************
|*  PointLight.h
|*
|*  Declaration of the class PointLight.
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/

#ifndef _POINTLIGHT_H
#define _POINTLIGHT_H


#include <sceneelements/ILight.h>
#include <utils/Vector4.h>


class PointLight : public ILight{

public:
	PointLight(void);


	PointLight(Vector3 position, Vector4 color);


	~PointLight(void);


	Ray generateRay(Vector3 endPoint);


	void setColor(Vector4 color);
	virtual Vector4 getColor(void) const;
	virtual Vector3 getPosition(void) const;
	
	void setPosition(Vector3 position);
	Vector3 getPosition(void);


private:

	Vector3 m_position;
	Vector4 m_color;

};


#endif //_POINTLIGHT_H
