/****************************************************************************
|*  Raytracer Framework
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/

#include "MeshTriangle.h"

#include <iostream> 

MeshTriangle::MeshTriangle() : parentMesh(0) { 
	v[0] = 0;
	v[1] = 0;
	v[2] = 0;

	m_finite = true;
}

MeshTriangle::MeshTriangle(Mesh *pm, MeshVertex* v0, MeshVertex* v1, MeshVertex* v2) : parentMesh(pm) {
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;
	
	m_finite = true;
}

MeshTriangle::~MeshTriangle(void) {
}

bool MeshTriangle::intersect(const Ray &ray, IntersectionData* iData) {
	
	const Vector3& v0(*(getVertex(0)->getPosition()));
	const Vector3& v1(*(getVertex(1)->getPosition()));
	const Vector3& v2(*(getVertex(2)->getPosition()));
	const Vector3 e1 = v1 - v0;
	const Vector3 e2 = v2 - v0;
	const Vector3 s1 = ray.direction.cross(e2);

	double div = s1.dot(e1);
	if (div==0) return false;  // no intersection
	
	double inv = 1.0/div;

	// compute first barycentric coordinate
	const Vector3 dist = ray.point - v0;
	double b1 = dist.dot(s1) * inv;
	
	if ((b1<0.0) || (b1>1.0)) return false; // no intersection

	// compute second barycentric coordinate
	const Vector3 s2 = dist.cross(e1);
	double b2 = ray.direction.dot(s2) * inv;
	if ((b2<0.0) || (b1+b2>1.0)) return false; // no intersection

	double t = e2.dot(s2) * inv;
	if ((t<ray.min_t) || (t>ray.max_t)) return false; // no intersection either

	if (t < iData->t) { // if intersection point is nearer than the old one
		iData->clear();
		iData->t=t;
		fillIntersectionData(b1,b2,*iData);
	
		if (iData->surfaceNormal.dot(ray.direction) < 0) // ray enters the object
			iData->rayEntersObject = true;
		else
			iData->rayEntersObject = false; // ray leaves the object
		iData->sourcePosition = ray.point;

		return true;
	} else { // nope, intersection is further away than the old one
		return false;
	}
}


bool MeshTriangle::fastIntersect(const Ray &ray) { 
	const Vector3& v0(*(getVertex(0)->getPosition()));
	const Vector3& v1(*(getVertex(1)->getPosition()));
	const Vector3& v2(*(getVertex(2)->getPosition()));
	const Vector3 e1 = v1 - v0;
	const Vector3 e2 = v2 - v0;
	const Vector3& s1 = ray.direction.cross(e2);

	double div = s1.dot(e1);
	if (div==0) return false;  // no intersection
	
	double inv = 1.0/div;

	// compute first barycentric coordinate
	const Vector3 dist = ray.point - v0;
	double b1 = dist.dot(s1) * inv;
	
	if ((b1<0.0) || (b1>1.0)) return false; // no intersection

	// compute second barycentric coordinate
	const Vector3 s2 = dist.cross(e1);
	double b2 = ray.direction.dot(s2) * inv;
	if ((b2<0.0) || (b1+b2>1.0)) return false; // no intersection

	double t = e2.dot(s2) * inv;
	if ((t<ray.min_t) || (t>ray.max_t)) return false; // no intersection either

	return true;
}

MeshVertex* MeshTriangle::getVertex(int i) {
	return v[i];
}

const MeshVertex* MeshTriangle::getVertex(int i) const{
	return v[i];
}


Vector2 MeshTriangle::minNmax(double a, double b, double c) {
	double tmp[3];
	tmp[0] = a;
	tmp[1] = b;
	tmp[2] = c;
	
	double m;

	for (int j=0; j<2; j++) {
		for (int i=0; i<(2-j); i++) {
			if (tmp[i] > tmp[i+1]) {
				m = tmp[i+1];
				tmp[i+1] = tmp[i];
				tmp[i] = m;
			}
		}
	}
	return Vector2(tmp[0], tmp[2]);
}

AABB MeshTriangle::getBB() const {

	const Vector3& m_p1(*(getVertex(0)->getPosition()));
	const Vector3& m_p2(*(getVertex(1)->getPosition()));
	const Vector3& m_p3(*(getVertex(2)->getPosition()));

	//initialize corners of BB with p1
	Vector3 lower = m_p1, upper = m_p1;

	//check components of p2
	for (int i = 0; i < 3; i++) {
		if (lower[i] > m_p2[i])
			lower[i] = m_p2[i];

		if (upper[i] < m_p2[i])
			upper[i] = m_p2[i];
	}

	//check components of p3
	for (int i = 0; i < 3; i++) {
		if (lower[i] > m_p3[i])
			lower[i] = m_p3[i];

		if (upper[i] < m_p3[i])
			upper[i] = m_p3[i];
	}

	return AABB(lower, upper);
}

double MeshTriangle::computeArea()
{
	return heron(*(getVertex(0)->getPosition()),*(getVertex(1)->getPosition()),*(getVertex(2)->getPosition()));
}

double MeshTriangle::getArea()
{
	//Here one could cache some values
	return computeArea();
}

void MeshTriangle::sample( IntersectionData& idata )
{
	double r1=static_cast<double>(rand())/static_cast<double>(RAND_MAX);
	double r2=static_cast<double>(rand())/static_cast<double>(RAND_MAX);
	double sr1 = sqrt(r1);

	//Sample the first 2 barycentric coordinates
	double ub = 1. - sr1;
	double vb = r2 * sr1;
	double wb = 1. - ub - vb;

	fillIntersectionData(vb,wb,idata);
}

void MeshTriangle::fillIntersectionData( double b1, double b2, IntersectionData& idata )
{
	Vector3& v0(*(getVertex(0)->getPosition()));
	Vector3& v1(*(getVertex(1)->getPosition()));
	Vector3& v2(*(getVertex(2)->getPosition()));
	Vector3 e1 = v1 - v0;
	Vector3 e2 = v2 - v0;

	// now we have a valid intersection
	Vector3& n0 = *getVertex(0)->getNormal();
	Vector3& n1 = *getVertex(1)->getNormal();
	Vector3& n2 = *getVertex(2)->getNormal();

	Vector3 interpolNormal = n0*(1-b1-b2) + n1*b1 + n2*b2;
	interpolNormal.normalize();

	// compute texture coords
	Vector2 interpolTexCoords = Vector2(0.0,0.0);
	if (m_texture || m_bumpmap) {
		Vector2 texture0 = *getVertex(0)->getTexture();
		Vector2 texture1 = *getVertex(1)->getTexture();
		Vector2 texture2 = *getVertex(2)->getTexture();	

		interpolTexCoords = texture0*(1-b1-b2) + texture1*b1 + texture2*b2;
		idata.textureCoords = interpolTexCoords;
	}


	// create intersection data
	idata.shape = this;
	idata.position = v0*(1-b1-b2) + v1*b1 + v2*b2;
	idata.material = getMaterial();

	Vector3 surfaceNormal = (e1.cross(e2)).normalize();
	if(surfaceNormal.dot(interpolNormal)< 0.)
		surfaceNormal = -surfaceNormal;

	idata.surfaceNormal = surfaceNormal;
	idata.shadingNormal = interpolNormal;

	// reflection and refraction
	idata.reflectionPercentage = getReflectionPercentage();
	idata.refractionIndexInside = getRefractionIndex();
	idata.refractionPercentage = getRefractionPercentage();

	// texturing
	if (m_texture) {
		idata.texture = m_texture;
	}else{
		idata.texture = NULL;
	}

	// bumpmapping
	if (m_bumpmap) {
		Vector2& texture0 = *getVertex(0)->getTexture();
		Vector2& texture1 = *getVertex(1)->getTexture();
		Vector2& texture2 = *getVertex(2)->getTexture();

		Vector2 t1 = texture1 - texture0;
		Vector2 t2 = texture2 - texture0;

		Vector3 local_x = (e1 * t2.y - e2 * t1.y) / (t2.y * t1.x - t2.x * t1.y);
		local_x = (local_x - interpolNormal * interpolNormal.dot(local_x)).normalize();
		Vector3 local_y = (e1 * t2.x - e2 * t1.x) / (t2.x * t1.y - t2.y * t1.x);
		local_y = (local_y - interpolNormal * interpolNormal.dot(local_y)).normalize();
		Vector3 local_z = local_x.cross(local_y).normalize();

		idata.localX = local_x;
		idata.localY = local_y;
		idata.localZ = local_z;

		idata.bumpmap = m_bumpmap;
	}

}

Material* MeshTriangle::getMaterial()
{
	return parentMesh->getMaterial();
}

void MeshTriangle::setMaterial( Material* material )
{
	IElement::setMaterial( material );
	parentMesh->setMaterial( material );
}

double MeshTriangle::projectedArea( const Vector3& nplane )
{
	Vector3& v0(*(getVertex(0)->getPosition()));
	Vector3& v1(*(getVertex(1)->getPosition()));
	Vector3& v2(*(getVertex(2)->getPosition()));

	//Project vertices to plane with normal nplane going throught the world center
	v0 -= nplane*(nplane.dot(v0));
	v1 -= nplane*(nplane.dot(v1));
	v2 -= nplane*(nplane.dot(v2));

	return heron(v0,v1,v2);
}
