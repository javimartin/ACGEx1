/****************************************************************************
|*  PathTracer.h
|*
|*  Mario Deuss, mario.deuss@epfl.ch
|*  Version Spring 2012
\***********************************************************/

#ifndef _PATHTRACER_H
#define _PATHTRACER_H

#include "Integrator.h"
#include <vector>
#include <rendererelements/Integrator/DirectLighting.h>
#include <cassert>

//Forward Declaration
class Mesh;
class MeshTriangle;
class Material;
class IntersectionData;
class Scene;
class Matrix4;
class Vector3;

class PathTracer: public DirectLighting {

public:

	PathTracer(){m_sampleDepth = 3; m_maxDepth =3; m_continueProb=0.5;}

	Vector4 integrate( const Ray& ray );

	//The path length up to which paths are traced for sure
	void setSampleDepth(unsigned int d){m_sampleDepth = d;}

	//The maximum length of a path, set to very high for truly unbiased rendering
	void setMaxDepth(unsigned int d){m_maxDepth = d;}

	//The probability of continuing a path (russian roulette)
	void setContinueProbabilty( double p ){assert(p>=0. && p<=1.); m_continueProb = p;}

protected:

	double m_continueProb;
	unsigned int m_sampleDepth;
	unsigned int m_maxDepth;
};

#endif //_PATHTRACER_H