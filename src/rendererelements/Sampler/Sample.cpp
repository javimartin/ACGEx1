/****************************************************************************
|*  Sample.cpp
|*
|*  Represents a pixel of the cameras film. It contains information about
|*  the position and color of this pixel.
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/



#include "Sample.h"


Sample::Sample(int x, int y) 
	: m_x(x), m_y(y) {
	m_size = 1;
	m_renderWeight = 1.0;
  m_oldColorRenderWeight = 0.0;
}


Sample::Sample(int x, int y, Vector4 color)
: m_x(x), m_y(y), m_color(color.clamp01()) {
	m_size = 1;
  m_renderWeight = 1.0;
  m_oldColorRenderWeight = 0.0;
}


Sample::~Sample(void){
}

	
void Sample::setPosX(int x) {
	m_x = x;
}
int Sample::getPosX(void) const {
	return m_x;	
}


void Sample::setPosY(int y) {
	m_y = y;
}
int Sample::getPosY(void) const {
	return m_y;
}


void Sample::setSize(unsigned long size) {
	m_size = size;
}


unsigned long Sample::getSize(void) const {
	return m_size;
}


void Sample::setColor(Vector4 color) {
	m_color = color;
}
void Sample::setColor(double r, double g, double b, double a) {
	Vector4 c = Vector4(r,g,b,a);
	m_color = c.clamp01();
}
Vector4 Sample::getColor(void) const {
	return m_color;
}


void Sample::setColR(double r) {
	if (r>1.f) r=1.f;
	else if (r<0.f) r=0.f;
	m_color.x = r;
}
double Sample::getColR(void) const {
	return m_color.x;
}


void Sample::setColG(double g) {
	if (g>1.f) g=1.f;
	else if (g<0.f) g=0.f;
	m_color.y = g;
}
double Sample::getColG(void) const {
	return m_color.y;
}


void Sample::setColB(double b) {
	if (b>1.f) b=1.f;
	else if (b<0.f) b=0.f;
	m_color.z = b;
}
double Sample::getColB(void) const {
	return m_color.z;
}


void Sample::setAlpha(double a) {
	if (a>1.f) a=1.f;
	else if (a<0.f) a=0.f;
	m_color.w = a;
}
double Sample::getAlpha(void) const {
	return m_color.w;
}


Vector2 Sample::getOffset() const  {
	return offset;
}

void Sample::setOffset(Vector2 _offset) {
	offset = _offset;
}


void Sample::setRenderWeight(double w) {
	m_renderWeight = w;
}

double Sample::getRenderWeight() const {
	return m_renderWeight;
}

void Sample::setOldColorRenderWeight(double w){
  m_oldColorRenderWeight = w;
}

double Sample::getOldColorRenderWeight() const {
 return m_oldColorRenderWeight;
}


void Sample::set(Vector2 _offset, Vector4 _color) {
	offset = _offset;
	m_color = _color;
}

