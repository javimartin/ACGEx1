/****************************************************************************
|*  IntersectionData.h
|*
|*  Intersection Data represents the information about the intersection 
|*  point of a Ray and an IElement.
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/


#ifndef _INTERSECTIONDATA_H
#define _INTERSECTIONDATA_H

#include <utils/Vector2.h>
#include <utils/Vector3.h>
#include <utils/Vector4.h>
#include <utils/Material.h>
#include <utils/textures/ITexture.h>


//Forward Declaration
class IElement;

class IntersectionData {

public:
	IntersectionData();
	~IntersectionData();

	IntersectionData(const IntersectionData &iD);

	void operator=(const IntersectionData &iD);

	void clear(void);

public: //DATA FIELDS

	// hit object
	IElement* shape;
	
	// geometric information
	Vector3 position;			// collision position
	double t;					// parameter t of the ray-element intersection
	Vector3 surfaceNormal;		// (geometric) surface normal at intersection point in world coordinates
	Vector3 shadingNormal;		// normal used for shading, usually different than the surface normal in case of triangle vertex normal interpolation or bump mapping
	Vector3 sourcePosition;		// startpoint of the ray that hit

	// surface
	Material* material;			// containts diffuse and emissive color and other parameter

	// texturing
	ITexture* texture;
	Vector2 textureCoords;

	// bumpmapping
	ITexture* bumpmap;
	Vector3 localX;
	Vector3 localY;
	Vector3 localZ;
	
	// reflection
	double reflectionPercentage;  
	
	// refraction
	double refractionIndexInside;	//formerly refractionIndex - filled by objects
	double refractionIndexOutside;	//new: filled by renderer!
	double refractionPercentage;
	bool rayEntersObject;
	
};


#endif //_INTERSECTIONDATA_H
