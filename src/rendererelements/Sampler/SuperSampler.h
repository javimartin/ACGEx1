/****************************************************************************
|*  Raytracer Framework
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/

#ifndef _SUPERSAMPLER_H
#define _SUPERSAMPLER_H

#include "ISampler.h"
#include <rendererelements/Sampler/Sample.h>

class SuperSampler : public ISampler {
public:
	SuperSampler(int samples_per_pixel, bool jitter);

	~SuperSampler(void);

	void init(int res_x, int res_y);

	//generate the sample s in the sequence of all samples
	bool getSample( int s, Sample* sample);

	//returns the number of total samples
	int getNumberOfSamples();

private:
	int sample_per_pixel;
	bool jitter;

	int m_resolutionX;
	int m_resolutionY;
};

#endif