/****************************************************************************
|*  Renderer.h
|*
|*  Declaration of a Renderer object.
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/


#ifndef _SIMPLE_RENDERER_H
#define _SIMPLE_RENDERER_H

#include <stdlib.h>
#include <vector>

#include <Scene.h>
#include <rendererelements/Sampler/ISampler.h>
#include <rendererelements/Integrator/Integrator.h>
#include <rendererelements/Sampler/Sample.h>
#include <rendererelements/IntersectionData.h>
#include <utils/Point.h>
#include <utils/Matrix4.h>
#include <utils/IFunctionObservable.h>


class Renderer : public IFunctionObservable {
	
public:	
	Renderer(void);

	~Renderer(void);

	//main renderloop that raytraces the given scene
	virtual void render(Scene* scene);

	void setIntegrator( Integrator* integrator ){ m_integrator = integrator; }
	void setSampler(ISampler* sampler);

	void setRecursiondepth(unsigned int depth);

	double status();

protected:

private:

	ISampler* m_sampler;

	Integrator* m_integrator;

	unsigned long m_numOfAllPixels;
	unsigned long m_numOfRenderedPixels;
};

#endif //_SIMPLE_RENDERER_H
