/****************************************************************************
|*  PathTracer.h
|*
|*  Mario Deuss, mario.deuss@epfl.ch
|*  Version: Spring 2012
\***********************************************************/

#include "PathTracer.h"

#include <Scene.h>
#include <utils/Ray.h>
#include <utils/Vector4.h>
#include <utils/MonteCarloUtilities.h>

Vector4 PathTracer::integrate( const Ray& ray )
{
	Vector4 L(0.5,0.5,0.5,1.);
	
	return L;
}
