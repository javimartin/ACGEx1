/****************************************************************************
|*  Raytracer Framework
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/

#include <utils/Material.h>
#include <utils/Vector3.h>

#include "Mesh.h"
#include <sceneelements/geometry/MeshTriangle.h>
#include <sceneelements/geometry/MeshVertex.h>

#include <vector>
#include <string>
#include <fstream>
#include <sstream>


Mesh::Mesh(int n_vertices, int n_triangles) {
	vertices.clear(); vertices.resize(n_vertices);
	triangles.clear(); triangles.resize(n_triangles);
	m_area = -1.;
	m_material = NULL;
}

Mesh::~Mesh(void) {
	for (unsigned int i=0; i < vertices.size(); i++) 
		delete vertices[i];
	vertices.clear();

	for( unsigned int i=0; i<normals.size(); i++)
		delete normals[i];
	normals.clear();

	// triangles are added directly to the scene as well, so they will be deleted there
	triangles.clear();	
}


MeshVertex* Mesh::getVertex(int i) {
	return vertices[i];
}


void Mesh::addTriangle(MeshTriangle *m) {
	triangles.push_back(m);
}

void Mesh::addVertex(MeshVertex *v) {
	v->setIndex(static_cast<unsigned int>(vertices.size()));
	vertices.push_back(v);
}

void Mesh::sample( IntersectionData& idata )
{
	if(m_area <= 0.)
		m_area=computeArea();

	double r=static_cast<double>(rand())/static_cast<double>(RAND_MAX);
	assert(r>=0. && r<=1.);
	
	double a=r*m_area;
	double sum_a=0.;
	std::vector<MeshTriangle*>::iterator it=triangles.begin();
	for( ; it!=triangles.end(); ++it){
		sum_a+=(*it)->computeArea();
		if(a<=sum_a)
			return (*it)->sample( idata );
	}

	return (triangles.back())->sample( idata );
}

void Mesh::setMaterial( Material* material )
{
	m_material = material;
}

Material* Mesh::getMaterial()
{
	return m_material;
}

double Mesh::computeArea()
{
	double area=0.;
	std::vector<MeshTriangle*>::iterator it=triangles.begin();
	for( ; it!=triangles.end(); ++it){
		//area+=(*it)->getArea();
		area+=(*it)->computeArea();
	}

	return area;
}

