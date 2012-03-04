/****************************************************************************
|*  DirectLighting.h
|*
|*  A integrator only considering direct lighting and triangle meshes
|*
|*  Mario Deuss, mario.deuss@epfl.ch
\***********************************************************/

#ifndef _IDIRECTLIGHTING_H
#define _IDIRECTLIGHTING_H

#include "Integrator.h"
#include <vector>

//Forward Declaration
class Mesh;
class MeshTriangle;
class IntersectionData;
class Scene;
class Matrix4;
class Vector3;

class DirectLighting : public Integrator {

public:

	DirectLighting();

	virtual ~DirectLighting( void ){};

	//Computes the radiance (color) incoming along the ray 
	virtual Vector4 integrate( const Ray& ray );
	Vector4 integrateConstant(const Ray& ray);
	Vector4 integrateSamplingBRDF( const Ray& ray );
	virtual void setScene( Scene* scene);

	void setNSamples( unsigned int nSamples ){ m_nSamples = nSamples; }

protected:

	unsigned int m_nSamples;
};


#endif //_IDIRECTLIGHTING_H