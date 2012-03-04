/****************************************************************************
|*  Raytracer Framework
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/

#include "SuperSampler.h"
#include <iostream>

SuperSampler::SuperSampler(int samples_per_pixel, bool jit){
	sample_per_pixel = samples_per_pixel;
	jitter = jit;

	m_resolutionX = 0;
	m_resolutionY = 0;
}	

SuperSampler::~SuperSampler(void){
}


void SuperSampler::init(int res_x, int res_y) {
	m_resolutionX = res_x;
	m_resolutionY = res_y;
}

int SuperSampler::getNumberOfSamples(){
	return m_resolutionX*m_resolutionY*sample_per_pixel;
}


//generate the sample s in the sequence of all samples
bool SuperSampler::getSample(int s, Sample* sample){


	int xy = static_cast<int>(floor(static_cast<double>(s)/static_cast<double>(m_resolutionX*m_resolutionY)));
	s -= xy * (m_resolutionX*m_resolutionY);

	int y  = static_cast<int>(floor(static_cast<double>(s)/static_cast<double>(m_resolutionX)));
	s -= y * (m_resolutionX);

	int x  = s;

	sample->setPosX(x);
	sample->setPosY(y);
	sample->setSize(1);
	sample->setColor(0.0, 0.0, 0.0, 1.0);
	sample->setRenderWeight(1.0);
	sample->setOldColorRenderWeight(1.);

	Vector2 offset(.5,.5);
	if (jitter) {
		float rand1 = (float)(rand()/(float(RAND_MAX)+1)-0.5);
		float rand2 = (float)(rand()/(float(RAND_MAX)+1)-0.5);
		offset += Vector2(rand1, rand2);
	}
	sample->setOffset(offset);

	return true;
}