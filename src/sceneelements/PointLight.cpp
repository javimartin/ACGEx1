/****************************************************************************
|*  PointLight.cpp
|*
|*  Definition of a point light source.
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/


#include "PointLight.h"


PointLight::PointLight(void) {
	m_position = Vector3(0.0, 0.0, 0.0);
	m_color = Vector4(1.0, 1.0, 1.0, 1.0);
}


PointLight::PointLight(Vector3 position, Vector4 color) 
	: m_position(position), m_color(color) {
}


PointLight::~PointLight(void) {
	
}


Ray PointLight::generateRay(Vector3 endPoint) {
	Vector3 dir = endPoint - m_position;
	double max_t = dir.length();
	
	if (max_t != 0) { //position not the same
		dir.normalize();
	} 

	Ray r = Ray(m_position, dir);
	r.depth = 0;
	r.max_t = max_t - Ray::epsilon_t;
	r.min_t = 0; 
	return r;
}


void PointLight::setColor(Vector4 color) {
	m_color = color;
}


Vector4 PointLight::getColor(void) const {
	return m_color;
}


void PointLight::setPosition(Vector3 position) {
	m_position = position;
}


Vector3 PointLight::getPosition(void) const {
	return m_position;
}
