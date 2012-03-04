/****************************************************************************
|*  WhittedIntegrator.h
|*
|*  Performs classical recursive backward raytracing to determine a rays incoming radiance
|*
|*  Mario Deuss, mario.deuss@epfl.ch
\***********************************************************/


#ifndef _WHITTEDINTEGRATOR_H
#define _WHITTEDINTEGRATOR_H

#include "Integrator.h"
#include <rendererelements/IntersectionData.h>
#include <vector>

class WhittedIntegrator : public Integrator {

public:

	enum shader{ CONSTANT, DIFFUSE, PHONG, PHONGBUMP };

	WhittedIntegrator( );

	~WhittedIntegrator( void ) {};

	Vector4 integrate( const Ray& ray );

	Vector4 integrate( const Ray& ray, std::vector<double>& refractionStack );
	
	void setRecursionDepth( unsigned int recursionDepth ){ m_recursionDepth = recursionDepth; }

	void setShader( shader s ){ m_shader=s;}

protected:

	Vector4 shade(IntersectionData* iData, Scene* scene);

private:

	shader m_shader;

	unsigned int m_recursionDepth;
	//std::vector<double> m_refractionStack;
};


#endif //WHITTEDINTEGRATOR_H