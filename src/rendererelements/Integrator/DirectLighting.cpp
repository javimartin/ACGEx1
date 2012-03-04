/****************************************************************************
|*  DirectLighting.cpp
|*
|*  A integrator only considering direct lighting
|*
\***********************************************************/

#include "DirectLighting.h"

#include <utils/Vector4.h>
#include <Scene.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <utils/MonteCarloUtilities.h>
#include <sceneelements/geometry/MeshTriangle.h>

DirectLighting::DirectLighting(){}

Vector4 DirectLighting::integrate( const Ray& ray )
{
	//return integrateConstant(ray);
	return integrateSamplingBRDF(ray);
}

Vector4 DirectLighting::integrateConstant(const Ray& ray) {
	
	
	
	Vector4 color(0.5, 0.5, 0.5, 1.0);
	return color;
}
Vector4 DirectLighting::integrateSamplingBRDF( const Ray& ray )
{
// Exercise 1.6
	Scene* scene = m_scene;

	Vector4 color;

	IntersectionData* p = scene->intersect(ray);

	if (p) { // successful intersection test

	//IntersectionData* p = this->m_scene->intersect(ray);
	Vector3 si = MonteCarloUtilities::uniformSampleHemisphere(p->surfaceNormal);
	Ray newRay = Ray(p->position, si);
	newRay.min_t = newRay.epsilon_t;

	IntersectionData* q = scene->intersect(newRay);
	Vector4 ld;

	if(q)
		ld = q->material->emission;
	
	//if ( q->surfaceNormal.dot(p->position) < 0 )
		//return Vector4(0.0,0.0,0.0,0.0);
	//else
		return ld;

	}
	
	// clean
	if (p!=NULL){
		delete(p);
	}

	return color.clamp01();

	//Vector4 color (0.5, 0.5, 0.5, 1.0);
	//return color;
}

void DirectLighting::setScene( Scene* scene )
{
	m_scene  = scene;
}
