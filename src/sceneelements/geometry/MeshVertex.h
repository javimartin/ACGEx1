/****************************************************************************
|*  Raytracer Framework
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/

#ifndef _MESH_VERTEX_H
#define _MESH_VERTEX_H

#include <utils/Vector3.h>
#include <utils/Vector2.h>

class MeshVertex {
public:
	MeshVertex();

	MeshVertex(const Vector3 &pos);

	~MeshVertex();

	void setNormal(Vector3 *normal);
	Vector3* getNormal();

	void setTexture(Vector2 *texture);
	Vector2* getTexture();

	void setPosition(Vector3& point);
	Vector3* getPosition();
	const Vector3* getPosition() const;

	unsigned int getIndex()
	{
		return index;
	}

	void setIndex(unsigned int i)
	{
		index = i;
	}



private:

	Vector3 position;
	Vector3* normal;
	Vector2* texture;
	unsigned int index;
};

#endif
