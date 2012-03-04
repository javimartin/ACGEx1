/****************************************************************************
|*  Raytracer Framework
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/

#ifndef _MESH_TRIANGLE_H
#define _MESH_TRIANGLE_H

#include <sceneelements/geometry/Mesh.h>
#include <sceneelements/geometry/MeshVertex.h>
#include <sceneelements/IElement.h>
#include <utils/AABB.h>

class MeshTriangle : public IElement {

public:
	MeshTriangle();
	MeshTriangle(Mesh *pm, MeshVertex* v0, MeshVertex* v1, MeshVertex* v2);

	~MeshTriangle(void);

	virtual bool intersect(const Ray &ray, IntersectionData* iData);
	virtual bool fastIntersect(const Ray &ray);

	Material* getMaterial();
	void setMaterial( Material* material );

	double getArea();
	double computeArea();

	double projectedArea( const Vector3& nplane);

	void sample(IntersectionData& idata);

	MeshVertex* getVertex(int i);
	const MeshVertex* getVertex(int i) const;

	Vector3 getCentroid() const { return (
		*(v[0]->getPosition()) + 
		*(v[1]->getPosition()) + 
		*(v[2]->getPosition())
		) / 3; };
	AABB getBB() const; 

private:

	//computing the triangle area using herons formula
	double inline heron( const Vector3& v0, const Vector3& v1, const Vector3& v2 )
	{
		//Heron's formula
		double a= (v0-v1).length();
		double b= (v1-v2).length();
		double c= (v2-v0).length();
		double s=(a+b+c)/2.;
		return sqrt(s*(s-a)*(s-b)*(s-c));
	}

private:

	MeshVertex* v[3];
	Mesh* parentMesh;

private:

	void fillIntersectionData(double b1, double b2, IntersectionData& idata );

	// returns the minimum and maximum of the three parameters
	// Vector2.x = min.  Vector2.y = max.
	Vector2 minNmax(double a, double b, double c);


};

#endif