/****************************************************************************
|*  Raytracer Framework
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/



#include "MeshVertex.h"

MeshVertex::MeshVertex() : position(Vector3(0,0,0)) {	}

MeshVertex::MeshVertex(const Vector3 &pos) : position(pos) {
	texture = 0;
	normal = 0;
}

MeshVertex::~MeshVertex() {
	if (texture) delete texture;
}

void MeshVertex::setNormal(Vector3 *normal) {
	this->normal = normal; 
}
Vector3* MeshVertex::getNormal() { 
	return normal; 
}

void MeshVertex::setTexture(Vector2 *texture) { 
	this->texture = texture; 
}

Vector2* MeshVertex::getTexture() { 
	return texture; 
}

Vector3* MeshVertex::getPosition(){
	return &position; 
}

const Vector3* MeshVertex::getPosition() const {
	return &position;
}


void MeshVertex::setPosition(Vector3 &point){
	position = point;
}