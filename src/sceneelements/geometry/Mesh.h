/****************************************************************************
|*  Raytracer Framework
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/

#ifndef _MESH_H
#define _MESH_H

#include <vector>
#include <utils/Vector2.h>
#include <utils/Vector3.h>
#include <utils/Vector4.h>
#include <utils/Material.h>

class MeshVertex;
class MeshTriangle;
class IntersectionData;

class Mesh {

public:
	Mesh(int n_vertices, int n_triangles);
	~Mesh(void);

	void addTriangle(MeshTriangle *m);
	void addVertex(MeshVertex *v);
	void addNormal(Vector3* n){normals.push_back(n);}

	void sample( IntersectionData& idata );

	double getArea(){return computeArea();}
	double computeArea();

	unsigned int numberOfVertices()
	{
		return (unsigned int)vertices.size();
	}

	unsigned int numberOfFaces()
	{
		return (unsigned int)triangles.size();
	}

	unsigned int numberOfNormals(){ return (unsigned int)normals.size();}

	MeshVertex *getVertex(int i);
	MeshTriangle * getFace(int i){return triangles[i];}
	Vector3* getNormal(int i){return normals[i];}



	Material* getMaterial();
	void setMaterial( Material* material );

	void setTexture(Vector2 *texture) { 
	this->texture = texture; 
}

private:
	Mesh(void) {};
	std::vector<MeshTriangle*> triangles;
	std::vector<MeshVertex*> vertices;
	std::vector<Vector3*> normals;
	Vector2* texture;

	double m_area;
	Material* m_material;
};

#endif