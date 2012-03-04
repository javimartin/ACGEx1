/****************************************************************************
|*  ISampler.h
|*
|*  Abstract Base Class definition of a sampler. A sampler picks samples
|*  of the screen for which the renderer should compute the color.
|*   
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/


#ifndef _ISAMPLER_H
#define _ISAMPLER_H


#include <rendererelements/Sampler/Sample.h>


class ISampler {

public:
	virtual ~ISampler(void) {};

	virtual void init(int res_x, int res_y) = 0;

	//generate the sample s in the sequence of all samples
	virtual bool getSample( int s, Sample* sample) = 0;

	//returns the number of total samples
	virtual int getNumberOfSamples()=0;

};


#endif //_ISAMPLER_H
