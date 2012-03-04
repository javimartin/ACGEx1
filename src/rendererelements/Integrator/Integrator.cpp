/****************************************************************************
|*  Integrator.h
|*
|*  Abstract Base Class definition of an Integrator.
|*
|*
|*  Mario Deuss, mario.deuss@epfl.ch
\***********************************************************/

#include "Integrator.h"

#include <utils/Vector2.h>
#include <utils/Vector3.h>
#include <utils/Vector4.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <algorithm>

void Integrator::setScene(Scene* scene) {
	m_scene=scene;
}
