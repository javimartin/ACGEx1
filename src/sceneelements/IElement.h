/****************************************************************************
|*  IElement.h
|*
|*  Base Class definition of a geometrical object.
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/


#ifndef _IELEMENT_H
#define _IELEMENT_H


#include <utils/Ray.h>
#include <rendererelements/IntersectionData.h>
#include <utils/AABB.h>
#include <utils/textures/ITexture.h>


class IElement {

public:
	IElement(void);

	~IElement(void);

	//intersect element with a ray, store information in iData and return true if
	// an intersection occured
	virtual bool intersect(const Ray &ray, IntersectionData* iData) = 0;
	
	//test whether the ray will intersect the element (faster than intersect)
	virtual bool fastIntersect(const Ray &ray) = 0;

	virtual void sample(IntersectionData& idata ) = 0;

	// a pointer to the bounding box
	virtual AABB getBB() const { return AABB(); }

	// setter and getter for fields
	virtual void setColor(Vector4 color);
	virtual Vector4 getColor(void) const;

	virtual void setMaterial(Material* material);
	virtual Material* getMaterial(void) const;

	virtual void setTexture(ITexture* texture);
	virtual ITexture* getTexture(void) const;

	virtual void setBumpmap(ITexture* bumpmap);
	virtual ITexture* getBumpmap(void) const;

	virtual void setReflectionPercentage(double reflectionPercentage);
	double getReflectionPercentage(void) const;

	virtual void setRefractionIndex(double index);
	virtual double getRefractionIndex() const;

	virtual void setRefractionPercentage(double percentage);
	virtual double getRefractionPercentage() const;

	virtual bool finite() const {return m_finite;}

	virtual Vector3 getCentroid() const { return Vector3(); };

protected: // fields every geometrical object has

	Vector4 m_color;
	Material* m_material;

	ITexture* m_texture;
	ITexture* m_bumpmap;

	double m_reflectionPercentage;
	double m_refractionIndex;
	double m_refractionPercentage;

	bool m_finite;
};


#endif //_IELEMENT_H
