/****************************************************************************
|*  IElement.cpp
|*
|*  Definition of a geometrical object in space.
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/


#include "IElement.h"


IElement::IElement(void) {
	m_color = Vector4(1.0, 1.0, 1.0, 1.0);
	m_material = 0;
	
	m_texture = 0;
	m_bumpmap = 0;
	
	m_reflectionPercentage = 0.0;
	m_refractionIndex = 1.0;
	m_refractionPercentage = 0.0;

	m_finite = false;
	
}
	
IElement::~IElement(void) {
}


// setter and getter for fields //////////////////////////////////////
void IElement::setColor(Vector4 color) {
	m_color = color;
}

Vector4 IElement::getColor(void) const{
	return m_color;
}



void IElement::setMaterial(Material* material) {
	m_material = material;
}
Material* IElement::getMaterial(void) const{
	return m_material;
}


void IElement::setTexture(ITexture* texture) {
	m_texture = texture;
}
ITexture* IElement::getTexture(void) const{
	return m_texture;
}



void IElement::setBumpmap(ITexture* bumpmap) {
	m_bumpmap = bumpmap;
}
ITexture* IElement::getBumpmap(void) const{
	return m_bumpmap;
}



void IElement::setReflectionPercentage(double reflectionPercentage) {
	if (reflectionPercentage > 1.0) m_reflectionPercentage = 1.0;
	else if (reflectionPercentage < 0.0) m_reflectionPercentage = 0.0;
	else m_reflectionPercentage = reflectionPercentage;

  if (m_refractionPercentage + m_reflectionPercentage >= 1) {
    double div = 1./(m_refractionPercentage + m_reflectionPercentage);
    m_refractionPercentage *= div;
    m_reflectionPercentage *= div;
  }
}

double IElement::getReflectionPercentage(void) const {
	return m_reflectionPercentage;
}

void IElement::setRefractionIndex(double index) {
	m_refractionIndex = index;
}
double IElement::getRefractionIndex() const {
	return m_refractionIndex;
}



void IElement::setRefractionPercentage(double percentage) {
	if (percentage > 1.0) m_refractionPercentage = 1.0;
	else if (percentage < 0.0) m_refractionPercentage = 0.0;
	else m_refractionPercentage = percentage;

  if (m_refractionPercentage + m_reflectionPercentage >= 1) {
    double div = 1./(m_refractionPercentage + m_reflectionPercentage);
    m_refractionPercentage *= div;
    m_reflectionPercentage *= div;
  }
}

double IElement::getRefractionPercentage() const {
	return m_refractionPercentage;
}

///////////////////////////////////////////////////////////////////////////////
