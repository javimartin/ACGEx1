/****************************************************************************
|*  Raytracer Framework
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/


#ifndef _KDTREENODE_H
#define _KDTREENODE_H

#include <list>
#include <sceneelements/IElement.h>
#include <utils/AABB.h>

enum axis {X, Y, Z};
enum branchLocation {LEFT, RIGHT};


typedef struct KDTreeNode_{
	KDTreeNode_() {
		splittingAxis = X;
		splittingCoordinate = 0;
		level = 0;

		leftChild = NULL;
		rightChild = NULL;
	}

	~KDTreeNode_() {
		if (level == 0) { //if this is the root node delete all elements on the heap in kd tree
			for (std::list<IElement*>::iterator element = elementList.begin(); element != elementList.end(); element++) {
				delete(*element);
			}
		}
		elementList.clear();

		delete(leftChild);
		delete(rightChild);
		leftChild = NULL;
		rightChild = NULL;
	}

	//bounding box
	AABB boundingBox;

	//splitting info
	axis splittingAxis;
	double splittingCoordinate;

	//list to scene elements
	std::list<IElement*> elementList;

	//recursion level
	unsigned short level;

	//child nodes
	struct KDTreeNode_ *leftChild;
	struct KDTreeNode_ *rightChild;
} KDTreeNode;

#endif
