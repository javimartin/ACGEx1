/****************************************************************************
|*  Integrator.h
|*
|*  Abstract Base Class definition of an Integrator.
|*
|*
|*  Mario Deuss, mario.deuss@epfl.ch
|*  Version: Spring 2012
\***********************************************************/


#ifndef _INTEGRATOR_H
#define _INTEGRATOR_H

//Forward Declaration
class Scene;
class Vector2;
class Vector3;
class Vector4;
class Ray;

class Integrator {

public:
	virtual ~Integrator(void){};

	//Computes the radiance (color) incoming along the ray 
	virtual Vector4 integrate( const Ray& ray ) = 0;

	virtual void setScene(Scene* scene);
protected:
	Scene* m_scene;
};


#endif //_INTEGRATOR_H