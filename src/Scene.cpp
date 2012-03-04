/****************************************************************************
|*  Scene.cpp
|*
|*  Represents a scene. This class represents a data structure for
|*  a camera, lights and elements. It also computes collisions
|*  of a ray with all the objects.
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/

#include "Scene.h"

unsigned long Scene::m_numOfIntersectionTests = 0;

Scene::Scene(void) {
	m_camera = 0;
	m_backgroundColor = Vector4(0.0, 0.0, 0.0, 1.0);
	m_refractionIndex = 1;

#ifdef USE_KD_TREE
	m_useKDTree = true;
	m_rootNode = NULL;
	setKDTreeMaxElementsInALeaf(1);
	setKDTreeDepth(15);

	m_kdMaterials[0] = new Material();
	m_kdMaterials[0]->diffuse = Vector4(0,0,0,1);
	m_kdMaterials[0]->emission = Vector4(1,0,0,1);
	m_kdMaterials[1] = new Material();
	m_kdMaterials[1]->diffuse = Vector4(0,0,0,1);
	m_kdMaterials[1]->emission = Vector4(1,0,0,1);
	m_kdMaterials[2] = new Material();
	m_kdMaterials[2]->diffuse = Vector4(0,0,0,1);
	m_kdMaterials[2]->emission = Vector4(1,0,0,1);

#else
	m_useKDTree = false;
#endif

	Material* default_material = new Material();
	this->addMaterial("default_material", default_material);
}

Scene::~Scene(void){
#ifdef USE_KD_TREE
	delete m_kdMaterials[0];
	delete m_kdMaterials[1];
	delete m_kdMaterials[2];
#endif

	unsigned int i;
	// delete elements
	std::list<IElement*>::iterator elementListIt;
	for (elementListIt = m_elementList.begin(); elementListIt != m_elementList.end(); elementListIt++) {
		delete(*elementListIt);
	}

	// delete lights
	for (i=0; i<m_lightList.size(); i++)
		delete(m_lightList[i]);
	m_lightList.clear();

	// delete meshes
	for (i=0; i<m_meshList.size(); i++) 
		delete(m_meshList[i]);
	m_meshList.clear();

	// delete materials
	std::map<std::string, Material*>::iterator it;
	for (it=m_materialList.begin(); it!=m_materialList.end(); it++)
		delete(it->second);
	m_materialList.clear();

	// delete textures
	std::map<std::string, ITexture*>::iterator it_t;
	for (it_t=m_textureList.begin(); it_t!=m_textureList.end(); it_t++)
		delete(it_t->second);
	m_textureList.clear();

	// delete camera aswell
	delete(m_camera);

}


void Scene::setCamera(ICamera*camera) {
	m_camera = camera;
}
ICamera* Scene::getCamera(void) {
	return m_camera;
}
	
void Scene::addElement(IElement* element) {
	m_elementList.push_back(element);
}

void Scene::addLight(ILight* light) {
	m_lightList.push_back(light);
}

//intersect scene with a ray
IntersectionData* Scene::intersect(const Ray &ray) const {

	IntersectionData* current = 0;

#ifdef USE_KD_TREE
	//check if a kd tree for intersection is available
	if (m_useKDTree) {
		bool intersected = false;

		//find minT, maxT for root node
		double minT, maxT;
		if ( rayBBIntersection(ray, m_rootNode->boundingBox, minT, maxT) ) { //if ray hits bb of root node
			if (minT < ray.min_t)
				minT = ray.min_t;
			if (maxT > ray.max_t)
				maxT = ray.max_t;

			current = intersectKDTree(ray, m_rootNode, minT, maxT);
		}

		// set current nearest t
		if (current != NULL) {
			intersected = true;
		}
		else
		{
			intersected = false;
			current = new IntersectionData();
		}
		// test intersection with objects
		if (m_elementList.size() > 0) {
			std::list<IElement*>::const_iterator element;
			for (element = m_elementList.begin(); element != m_elementList.end(); element++) {
				++m_numOfIntersectionTests;
				if ((*element)->intersect(ray,current)) {
					intersected = true;
				}
			}
		}

		if (intersected) {
			return current;
		}
		else {
			delete current;
			current = NULL;
			return 0;
		}
	}
	else{
#endif

		bool intersected = false;

		IntersectionData* iData = new IntersectionData();
		// test intersection with objects
		if (m_elementList.size() > 0) {
			std::list<IElement*>::const_iterator element;
			for (element = m_elementList.begin(); element != m_elementList.end(); element++) {
				if ((*element)->intersect(ray,iData)) intersected = true;
			}
		}

		// see if iData is valid
		if (intersected) {
			return iData;
		} else {
			delete iData;
			iData = NULL;
			return 0;
		}
#ifdef USE_KD_TREE
	}
#endif
}

//test whether the ray will intersect any element in the scene (faster than intersect)
bool Scene::fastIntersect(const Ray &ray) const {
	bool intersection = false;

#ifdef USE_KD_TREE
	//check if a kd tree for intersection is available
	if (m_useKDTree) {
		//find minT, maxT for root node
		double minT, maxT;
		if ( rayBBIntersection(ray, m_rootNode->boundingBox, minT, maxT) ) {//if ray hits bb of root node
			if (minT < ray.min_t)
				minT = ray.min_t;
			if (maxT > ray.max_t)
				maxT = ray.max_t;
			intersection = fastIntersectKDTree(ray, m_rootNode, minT, maxT);
		}


		//if we already found an intersection return true
		if (intersection)
			return true;

		//otherwise test all elements in m_elementList
		if (m_elementList.size() > 0) {
			std::list<IElement*>::const_iterator element;
			for (element = m_elementList.begin(); element != m_elementList.end(); element++) {
				if ((*element)->fastIntersect(ray)) {
					// intersection found
					return true;
				}
			}
		}

		return false;
	}
	else
	{
#endif
		if (m_elementList.size() > 0) {
			std::list<IElement*>::const_iterator element;
			for (element = m_elementList.begin(); element != m_elementList.end(); element++) {
				if ((*element)->fastIntersect(ray)) {
					// intersection found
					return true;
				}
			}
		}
		return false;
#ifdef USE_KD_TREE
	}
#endif
}

//get the list of lights that are visible at the specified point
std::vector<ILight*> Scene::getNonOccludedLights(const Vector3 &point) const{
  //return m_lightList; //noshadowhack
  std::vector<ILight*> nonOccludedLights;

	for (unsigned long i = 0; i < m_lightList.size(); i++) {
		// shadow ray
		Ray lightRay = m_lightList[i]->generateRay(point);
		if ( !fastIntersect(lightRay) )
			nonOccludedLights.push_back(m_lightList[i]);
	}

	return nonOccludedLights;
}

std::vector<ILight*> Scene::getLights(void) const{
	return m_lightList;
}


void Scene::setSample(const Sample sample) {
	m_camera->setSample(sample);
}


void Scene::setBackground(Vector4 bgcolor) {
	m_backgroundColor = bgcolor;
}
Vector4 Scene::getBackground(void) const {
	return m_backgroundColor;
}


void Scene::setAmbient(Vector4 ambient){
	m_ambient = ambient;
}

Vector4 Scene::getAmbient(void) const{
	return m_ambient;
}

void Scene::setRefractionIndex(double index) {
	m_refractionIndex = index;
}

double Scene::getRefractionIndex() const {
	return m_refractionIndex;
}


// global lists
///////////////////////////////////////////////////////////////////

// material list
void Scene::addMaterial(std::string name, Material* material) {
	m_materialList[name] = material;
}

Material* Scene::getMaterial(std::string name){
	return m_materialList[name];
}

Material* Scene::getDefaultMaterial(void){
	return m_materialList["default_material"];
}


// texture list

void Scene::addTexture(std::string name, ITexture* texture) {
	m_textureList[name] = texture;
}

ITexture* Scene::getTexture(std::string name) {
	return m_textureList[name];
}

void Scene::addMesh(Mesh* mesh) {
	m_meshList.push_back(mesh);
}

std::vector<Mesh*> Scene::getMeshes( void ) const
{
	return m_meshList;
}

// kdtree

int Scene::buildKDTree() {
#ifdef USE_KD_TREE
	std::cout << "building kd tree..." << std::endl;

	initKDTree();
	recursivelySplitCell(m_rootNode);

	//print leftmost depth and #elements in leaf
	KDTreeNode *node = m_rootNode;
	while (node->leftChild != NULL)
		node = node->leftChild;
	std::cout << "kd tree depth at the very left: " << node->level << std::endl;
	std::cout << "elements in the very left leaf: " << node->elementList.size() << std::endl;

	return 0;
#else
	return 0;
#endif
}

#ifdef USE_KD_TREE

///////////////////////////////////////////////////////
// Kd tree construction
//

void Scene::initKDTree() {
	//create root node of tree
	delete(m_rootNode); //just to be safe
	m_rootNode = new KDTreeNode;

	//set level
	m_rootNode->level = 0;

	//set first splitting axis of tree
	m_rootNode->splittingAxis = X;

	//move all finite scene elements to root node of kd tree
	std::list<IElement*>::iterator element = m_elementList.begin();
	while ( element != m_elementList.end() ) {
		if ( (*element)->finite() ) {
			m_rootNode->elementList.push_back(*element); //copy pointer to element into root node
			element = m_elementList.erase(element);	//remove element from ordinary element list and obtain pointer to next element in list
		}
		else
			element++;
	}
	
	m_rootNode->boundingBox = computeBB(m_rootNode->elementList);
}

AABB Scene::computeBB(const std::list<IElement*> elementList) {
	AABB globalBB;
	AABB bb;

	//loop over primitive list
	for (std::list<IElement*>::const_iterator element = elementList.begin(); element != elementList.end(); element++) {
		//get local bb of primitive
		bb = (*element)->getBB();
		
		//expand global bounding box of primitveList so it includes the bb of the current element
		for (int i = 0; i < 3; i++) {
			if (bb.corners[0][i] < globalBB.corners[0][i])
				globalBB.corners[0][i] = bb.corners[0][i];

			if (bb.corners[1][i] > globalBB.corners[1][i])
				globalBB.corners[1][i] = bb.corners[1][i];
		}
	}

	return globalBB;
}

void Scene::recursivelySplitCell(KDTreeNode *node) {
	if( !terminateConstruction(node) ) {
		//compute splitting coordinate of current node
		if ( !computeSplittingPlanePosition(node) )
			//if the split operation would divide the cell at its border (zero volume for one child)
			return;

		// create children of current node
		node->leftChild = new KDTreeNode();
		node->rightChild = new KDTreeNode();	

		// initialize children
		node->leftChild->level = node->level + 1; //set level (0,1,2...)
		node->rightChild->level = node->level + 1; //set level (0,1,2...)
		node->leftChild->boundingBox = computeBB(node->boundingBox, node->splittingAxis, node->splittingCoordinate, LEFT); 
		node->rightChild->boundingBox = computeBB(node->boundingBox, node->splittingAxis, node->splittingCoordinate, RIGHT); 
		node->leftChild->splittingAxis = node->splittingAxis; 
		node->rightChild->splittingAxis = node->splittingAxis;
		nextAxis(node->leftChild); 
		nextAxis(node->rightChild); 

		// move elements to children
		moveElementsIntoChildCells(node);

#ifdef SHOW_SPLITS
		// Add a visible splitting plane, built with 2 triangles
		int axis2 = (node->splittingAxis + 1) % 3;
		int axis3 = (node->splittingAxis + 2) % 3;
		Triangle* triangle1 = new Triangle(m_kdMaterials[node->splittingAxis]);
		triangle1->setPoint1(node->leftChild->boundingBox.corners[1]);
		triangle1->setPoint2(node->rightChild->boundingBox.corners[0]);
		Vector3 point3 = Vector3();
		point3[node->splittingAxis] = node->splittingCoordinate;
		point3[axis2] = node->boundingBox.corners[0][axis2];
		point3[axis3] = node->boundingBox.corners[1][axis3];
		triangle1->setPoint3(point3);
		triangle1->setRefractionPercentage(0.85);
		Triangle* triangle2 = new Triangle(m_kdMaterials[node->splittingAxis]);
		triangle2->setPoint1(node->leftChild->boundingBox.corners[1]);
		triangle2->setPoint2(node->rightChild->boundingBox.corners[0]);
		point3[axis2] = node->boundingBox.corners[1][axis2];
		point3[axis3] = node->boundingBox.corners[0][axis3];
		triangle2->setPoint3(point3);
		triangle2->setRefractionPercentage(0.85);
		m_elementList.push_back(triangle1);
		m_elementList.push_back(triangle2);
#endif

		//next recursion step
		recursivelySplitCell(node->leftChild);
		recursivelySplitCell(node->rightChild);
	}
}

AABB Scene::computeBB(const AABB bb, const axis splittingAxis, const double splittingPoint, const branchLocation branch) {
	AABB newBB = bb;

	//split the new bb along the splitting axis and choose the split result according to branch
	switch (branch) {
		case LEFT:
			newBB.corners[1][splittingAxis] = splittingPoint;
			break;
		case RIGHT:
			newBB.corners[0][splittingAxis] = splittingPoint;
			break;
	}

	return newBB;
}

void Scene::nextAxis(KDTreeNode *node) {
	axis next;

	switch (1) {
		case 1:
			{
				//first option: cycle through X,Y,Z
				next = (axis) ((node->splittingAxis + 1) % 3);
				break;
			}
		case 2:
			{
				//second option: choose dimension with largest extend
				next = X;
				double maxSize = node->boundingBox.corners[1][X] - node->boundingBox.corners[0][X];
				for (axis a = Y; a <= Z; a=(axis)(a+1)) {
					if (node->boundingBox.corners[1][a] - node->boundingBox.corners[0][a] > maxSize) {
						next = a;
						maxSize = node->boundingBox.corners[1][a] - node->boundingBox.corners[0][a];
					}
				}
				break;
			}
	}

	node->splittingAxis = next;
}

bool Scene::computeSplittingPlanePosition(KDTreeNode *node) {
	axis splittingAxis = node->splittingAxis;
	double splitPosition = 0.0;
	
	switch (1) {
		case 1:
			{
				//first option: choose center of bb (along splitting axis) as the splitting coordinate
				AABB bb = node->boundingBox;
				splitPosition = (bb.corners[0][splittingAxis] + bb.corners[1][splittingAxis]) / 2;
				
				//check if the split produces a cell with zero volume
				if (splitPosition == bb.corners[0][splittingAxis])
					return false;

				break;
			}
		case 2:
			{
				//second option: choose median of element centroids as the splitting coordinate
				//get positions of centroids along splittingAxis
				std::vector<double> centroidPositions;
				unsigned long N = (unsigned long) node->elementList.size();
				std::list<IElement*>::iterator element;
				for (element = node->elementList.begin(); element != node->elementList.end(); element++) {
					centroidPositions.push_back( (*element)->getCentroid()[splittingAxis] );
				}
				//sort centroids
				std::sort(centroidPositions.begin(), centroidPositions.end());
				//get median value
				if ((N/2)*2 == N) //N is even (take the mean of the two center values)
					splitPosition = (centroidPositions[N/2 - 1] + centroidPositions[N/2]) / 2;
				else //N is uneven (take the center value)
					splitPosition = centroidPositions[(N-1)/2];

				//check if the split produces a cell with zero volume
				if (splitPosition <= node->boundingBox.corners[0][splittingAxis] ||
					splitPosition >= node->boundingBox.corners[1][splittingAxis])
					return false;

				break;
			}
		case 3:
			{
				//third option: choose mean of element centroids as the splitting coordinate
				unsigned long N = (unsigned long) node->elementList.size();
				std::list<IElement*>::iterator element;
				for (element = node->elementList.begin(); element != node->elementList.end(); element++) {
					splitPosition += (*element)->getCentroid()[splittingAxis];
				}
				if (N != 0)
					splitPosition /= N;

				//check if the split produces a cell with zero volume
				if (splitPosition <= node->boundingBox.corners[0][splittingAxis] ||
					splitPosition >= node->boundingBox.corners[1][splittingAxis])
					return false;
			}
	}

	node->splittingCoordinate = splitPosition;
	return true;
}


bool Scene::terminateConstruction(const KDTreeNode *node) {
	if (node->elementList.size() <= m_maxElementsInALeaf)
		return true;

	if (node->level >= m_maxRecursionDepth)
		return true;

	return false;
}


void Scene::moveElementsIntoChildCells(KDTreeNode *node) {
	std::list<IElement*>::iterator element = node->elementList.begin();
	while ( element != node->elementList.end() ) {
		if ( bbOverlap(node->leftChild->boundingBox, (*element)->getBB()) ) {
			node->leftChild->elementList.push_back(*element); //copy element into left child node
		}
		if ( bbOverlap(node->rightChild->boundingBox, (*element)->getBB()) ) {
			node->rightChild->elementList.push_back(*element); //copy element into right child node
		}
		
		//remove element from parent list if it is not the root node
		//(the root will store all pointers to safely delete all elements during destruction)
		if (node->level != 0)
			element = node->elementList.erase(element);
		else
			element++;
	}
}


bool Scene::bbOverlap(const AABB bb1, const AABB bb2) {
	//bbs have to overlap in every dimension
	for (int i = 0; i < 3; i++) {
		if (bb1.corners[0][i] > bb2.corners[1][i] || bb1.corners[1][i] < bb2.corners[0][i])
			return false;
	}

	return true;
}

///////////////////////////////////////////////////////
// Use of kd tree
//

IntersectionData *Scene::intersectKDTree(const Ray &ray, KDTreeNode *node, double minT, double maxT) const
{
	//if the current node is a leaf but empty
	if (node->leftChild == NULL && node->elementList.empty())
		return NULL;

	//if the current node is a leaf go through element list of node and return closest intersection
	if (node->leftChild == NULL) {

		IntersectionData* nearest = new IntersectionData();

		bool intersected = false;

		// test intersection with objects

		std::list<IElement*>::iterator element;
		Ray new_ray(ray.point, ray.direction);
		new_ray.max_t = maxT;
		new_ray.min_t = minT;
		for (element = node->elementList.begin(); element != node->elementList.end(); element++) {
			++m_numOfIntersectionTests;
			if ((*element)->intersect(new_ray,nearest)) {
				intersected = true;
			}
		}

		if (intersected) {
			return nearest;
		}
		else{
			delete nearest; // memory leak avoidance
			nearest = NULL;
			return 0;
		}
	}
	//if current node is not a leaf: compute t_split
	axis splitAxis = node->splittingAxis;
	double t_split;
	if (ray.direction[splitAxis] != 0) //if the ray intersects the splitting plane
		t_split = (node->splittingCoordinate - ray.point[splitAxis]) / ray.direction[splitAxis];
	else if (ray.point[splitAxis] <= node->splittingCoordinate) //if the ray has no intersection with the splitting plane (or lies on it) and the origin of the ray lies in the left child cell
		t_split = 3.4e38; //set t_split  to "infinity"
	else //if the ray has no intersection with the splitting plane and the origin of the ray lies in the right child cell
		t_split = -3.4e38; //set t_split  to "-infinity"


	//find near and far node of child nodes
	KDTreeNode *nearNode = NULL;
	KDTreeNode *farNode = NULL;
	if ( (ray.direction[splitAxis] >= 0 && t_split >= minT) ||
		(ray.direction[splitAxis] <  0 && t_split <  minT) ) { //if the ray runs from left to right and split plane is in front of ray segment
			//OR ray runs from right to left and split plane is behind ray segment
			nearNode = node->leftChild;
			farNode = node->rightChild;
	}
	else {														//if the ray runs from right to left and split plane is in front of ray segment
		//OR ray runs from left to right and split plane is behind ray segment
		nearNode = node->rightChild;
		farNode = node->leftChild;
	}


	if (t_split > maxT || t_split < minT) //if t_split is not on the current ray segment only treat the nearNode
		return intersectKDTree(ray, nearNode, minT, maxT);
	else { //if t_split is on the current ray segment treat the nearNode first and if no intersection is found check the farNode
		IntersectionData *inter = intersectKDTree(ray, nearNode, minT, t_split);
		if (inter != NULL)
			return inter;
		else{
			delete inter;
			inter = NULL;
			return intersectKDTree(ray, farNode, t_split, maxT);
		}
	}
}


bool Scene::fastIntersectKDTree(const Ray &ray, KDTreeNode *node, double minT, double maxT) const {
	//if the current node is a leaf but empty
	if (node->leftChild == NULL && node->elementList.empty())
		return false;


	//if the current node is a leaf go through element list of node and return closest intersection
	if (node->leftChild == NULL) {
		// test intersection with objects
		std::list<IElement*>::iterator element;
		for (element = node->elementList.begin(); element != node->elementList.end(); element++) {
			if ((*element)->fastIntersect(ray))
				return true;
		}
		return false;
	}


	//if current node is not a leaf: compute t_split
	axis splitAxis = node->splittingAxis;
	double t_split;

	if (ray.direction[splitAxis] != 0) //if the ray intersects the splitting plane
		t_split = (node->splittingCoordinate - ray.point[splitAxis]) / ray.direction[splitAxis];
	else if (ray.point[splitAxis] <= node->splittingCoordinate) //if the ray has no intersection with the splitting plane (or lies on it) and the origin of the ray lies in the left child cell
		t_split = 3.4e38; //set t_split  to "infinity"
	else //if the ray has no intersection with the splitting plane (or lies on it) and the origin of the ray lies in the right child cell
		t_split = -3.4e38;

	//find near and far node of child nodes
	KDTreeNode *nearNode = NULL;
	KDTreeNode *farNode = NULL;
	if ( (ray.direction[splitAxis] >= 0 && t_split >= minT) ||
		 (ray.direction[splitAxis] <  0 && t_split <  minT) ) { //if the ray runs from left to right and split plane is in front of ray segment
																//OR ray runs from right to left and split plane is behind ray segment
		nearNode = node->leftChild;
		farNode = node->rightChild;
	}
	else {														//if the ray runs from right to left and split plane is in front of ray segment
																//OR ray runs from left to right and split plane is behind ray segment
		nearNode = node->rightChild;
		farNode = node->leftChild;
	}


	if (t_split > maxT || t_split < minT) //if t_split is not on the current ray segment only treat the nearNode
		return fastIntersectKDTree(ray, nearNode, minT, maxT);
	else { //if t_split is on the current ray segment treat the nearNode first and if no intersection is found check the farNode
		bool inter = fastIntersectKDTree(ray, nearNode, minT, t_split);
		if (inter)
			return true;
		else
			return fastIntersectKDTree(ray, farNode, t_split, maxT);
	}
}


bool Scene::rayBBIntersection(const Ray &ray, const AABB &bb, double &minT, double &maxT) const {

	/*
 *      Amy Williams, Steve Barrus, R. Keith Morley, and Peter Shirley
 *      "An Efficient and Robust Ray-Box Intersection Algorithm"
 *      Journal of graphics tools, 10(1):49-54, 2005
 *
 */

	Vector3 inv_direction(1/ray.direction[0], 1/ray.direction[1], 1/ray.direction[2]);
	  int sign[3];
      sign[0] = (inv_direction[0] < 0);
      sign[1] = (inv_direction[1] < 0);
      sign[2] = (inv_direction[2] < 0);

	double tmin, tmax, tymin, tymax, tzmin, tzmax;

  tmin = (bb.corners[sign[0]][0] - ray.point[0]) * inv_direction[0];
  tmax = (bb.corners[1-sign[0]][0] - ray.point[0]) * inv_direction[0];
  tymin = (bb.corners[sign[1]][1] - ray.point[1]) * inv_direction[1];
  tymax = (bb.corners[1-sign[1]][1] - ray.point[1]) * inv_direction[1];
  if ( (tmin > tymax) || (tymin > tmax) ) 
    return false;
  if (tymin > tmin)
    tmin = tymin;
  if (tymax < tmax)
    tmax = tymax;
  tzmin = (bb.corners[sign[2]][2] - ray.point[2]) * inv_direction[2];
  tzmax = (bb.corners[1-sign[2]][2] - ray.point[2]) * inv_direction[2];
  if ( (tmin > tzmax) || (tzmin > tmax) ) 
    return false;
  if (tzmin > tmin)
    tmin = tzmin;
  if (tzmax < tmax)
    tmax = tzmax;

  //return ( (tmin < t1) && (tmax > t0) );
  minT=tmin;
  maxT=tmax;
  return true;
}


#endif