/****************************************************************************
|*  Material.h
|*
|*  Definition of a material description.
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/



#ifndef _MATERIAL_H
#define _MATERIAL_H


#include <utils/Vector4.h>


class Material {
public:
	// Matierial Methods
	Material(void) {
		emission = Vector4(0.0, 0.0, 0.0, 1.0);
		ambient = Vector4(0.0, 0.0, 0.0, 1.0);
		diffuse = Vector4(1.0, 1.0, 1.0, 1.0);
		specular = Vector4(1.0, 1.0, 1.0, 1.0);

		shininess = 10;
	}

	// Material Data
	Vector4 emission;
	Vector4 diffuse;
	Vector4 ambient;
	Vector4 specular;

	double shininess;
};


#endif //_MATERIAL_H
