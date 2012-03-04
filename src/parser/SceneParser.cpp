/****************************************************************************
|*  SceneParser.h
|*
|*  SceneParser class. Parses a scene description file and construct scene.
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/

#define READ_TEXTURES_FLAG 1

#include <stdio.h>
#include <iostream>

// geometry //
/*
#include <sceneelements/geometry/Triangle.h>
#include <sceneelements/geometry/Plane.h>
#include <sceneelements/geometry/CirclePlane.h>
#include <sceneelements/geometry/Sphere.h>
#include <sceneelements/geometry/Quadric.h>
#include <sceneelements/geometry/Torus.h>
*/
#include <sceneelements/geometry/Mesh.h>
#include <sceneelements/geometry/MeshTriangle.h>
#include <sceneelements/geometry/MeshVertex.h>

// \geometry //

#include <parser/SimpleXMLNode.h>
#include <sceneelements/SimpleCamera.h>
#include <sceneelements/PointLight.h>
#include <utils/string_conversions.h>

#include "parser/SceneParser.h"

#include <triangleMeshReader/OBJFileReader.h>

#ifdef READ_TEXTURES_FLAG
#include <utils/textures/ImageTexture.h>
#endif

SceneParser::SceneParser(void){
}


SceneParser::~SceneParser(void){
}


//parse a scene description file and generate a Scene
Scene* SceneParser::parse(const char* filename){
	std::cout << "SceneParser::parse parses scene... " << filename <<" \n";

	struct basicxmlnode * rootNode = NULL;

	//open file
	FILE * fp = fopen(filename, "rt");
	if (!fp) {
		std::cerr << "SceneParser - Error: Failed opening file " << filename << "\n";
		return NULL;
	}

  std::string filenameString = std::string(filename);
  size_t index = filenameString.find_last_of("/");
  if (index == std::string::npos) {
    index = filenameString.find_last_of("\\");
  }

  if (index == std::string::npos) {
    directory = "";
  }
  else {
    directory = filenameString.substr(0,index+1);
  }

	//read xml tree
	rootNode = readbasicxmlnode(fp);
	fclose(fp);
	if (!rootNode) {
		std::cerr << "SceneParser - Error: Failed reading file " << filename << ". Maybe an XML syntax error?\n";
		return NULL;
	}
	
	//construct scene
	Scene* scene = new Scene();

	//read scene properties
	if (!addSceneProperties(rootNode, scene)) {
		std::cerr << "SceneParser - Error: Failed to read Scene Properties in " << filename << "\n";
		deletebasicxmlnode(rootNode);
		delete(scene);
		return NULL;
	}

	//read camera
	struct basicxmlnode * cameraNode = getchildnodebyname(rootNode, "Camera");
	if (!addCamera(cameraNode, scene)) {
		std::cerr << "SceneParser - Error: Failed reading camera description in " << filename << "\n";
		deletebasicxmlnode(rootNode);
		delete(scene);
		return NULL;
	}

	//read lights
	struct basicxmlnode * lightsNode = getchildnodebyname(rootNode, "Lights");
	if (!lightsNode) {
		std::cout << "SceneParser - Warning: No Lights specified in " << filename << "\n";
	}
	else if (!lightsNode->children[0]) {
		std::cout << "SceneParser - Warning: No Lights specified in " << filename << "\n";
	}
	else {
		for(int lightsIndex = 0; lightsNode->children[lightsIndex]; lightsIndex++) {
			if(!addLight(lightsNode->children[lightsIndex], scene)) {
				std::cerr << "SceneParser - Error: Failed reading light description in " << filename << "\n";
				deletebasicxmlnode(rootNode);
				delete(scene);
				return NULL;
			}
		}
	}


	//read materials
	struct basicxmlnode * materialsNode = getchildnodebyname(rootNode, "Materials");
	if (!materialsNode) {
		std::cout << "SceneParser - No global Materials specified in " << filename << "\n";
	}
	else if (!materialsNode->children[0]) {
		std::cout << "SceneParser - Empty Materials node in " << filename << "\n";
	} else {
		for(int materialsIndex = 0; materialsNode->children[materialsIndex]; materialsIndex++) {
			if(!addGlobalMaterial(materialsNode->children[materialsIndex], scene)) {
				std::cerr << "SceneParser - Error: Failed reading global material description in " << filename << "\n";
				deletebasicxmlnode(rootNode);
				delete(scene);
				return NULL;
			}
		}
	}

	//read textures
	struct basicxmlnode* texturesNode = getchildnodebyname(rootNode, "Textures");
	if (!texturesNode) {
		std::cout << "SceneParser - No global Textures specified in " << filename << "\n";
	}
	else if (!texturesNode->children[0]) {
		std::cout << "SceneParser - Empty Textures node in " << filename << "\n";
	}
	else {
		for(int texturesIndex = 0; texturesNode->children[texturesIndex]; texturesIndex++) {
			if(!addGlobalTexture(texturesNode->children[texturesIndex], scene)) {
				std::cerr << "SceneParser - Error: Failed reading global texture description in " << filename << "\n";
				deletebasicxmlnode(rootNode);
				delete(scene);
				return NULL;
			}
		}
	}

	
	//read elements
	struct basicxmlnode * elementsNode = getchildnodebyname(rootNode, "Elements");
	if (!elementsNode) {
		std::cout << "SceneParser - Warning: No Elements specified in " << filename << "\n";
	}
	else if (!elementsNode->children[0]) {
		std::cout << "SceneParser - Warning: No Elements specified in " << filename << "\n";
	}
	for(int elementsIndex = 0; elementsNode->children[elementsIndex]; elementsIndex++) {
		if(!addElement(elementsNode->children[elementsIndex], scene)) {
			std::cerr << "SceneParser - Error: Failed reading element description in " << filename << "\n";
			deletebasicxmlnode(rootNode);
			delete(scene);
			return NULL;
		}
	}

	
	//free xml memory
	deletebasicxmlnode(rootNode);

	std::cout << "[done]\n\n";

	return scene;
}

bool SceneParser::addSceneProperties(struct basicxmlnode * sceneNode, Scene * scene){
	if (!sceneNode) {
		std::cout << "SceneParser::addSceneProperties: empty scene node\n";
		return false;
	}

	if (std::string(sceneNode->tag) != "Scene") {
		std::cout << "SceneParser::addSceneProperties: scene description file has to have toplevel Scene node\n";
		return false;
	}

	// read scene properties
	Vector4 color;
	double refractionIndex;

	char * attributeValue;
	if (attributeValue = getattributevaluebyname(sceneNode, "background")) {
		if (!stringToVector4<double>(color, attributeValue)) {
			return false;
		}
		scene->setBackground(color);
	}
	if (attributeValue = getattributevaluebyname(sceneNode, "ambient")) {
		if (!stringToVector4<double>(color, attributeValue)) {
			return false;
		}
		scene->setAmbient(color);
	}
	if (attributeValue = getattributevaluebyname(sceneNode, "refractionIndex")) {
		if (!stringToNumber<double>(refractionIndex, attributeValue)) {
			return false;
		}
		scene->setRefractionIndex(refractionIndex);
	}

	return true;
}

bool SceneParser::addCamera(struct basicxmlnode * cameraNode, Scene * scene){
	if (!cameraNode) {
		std::cout << "SceneParser::addCamera: empty camera node\n";
		return false;
	}

	if (std::string(cameraNode->tag) != "Camera") {
		return false;
	}

	// read resolution (mandatory attributes!)
	int resolutionX;
	int resolutionY;
	char * attributeValue;
	if (attributeValue = getattributevaluebyname(cameraNode, "resolutionX")) {
		if (!stringToNumber<int>(resolutionX, attributeValue)) {
			return false;
		}
	}
	else {
		std::cerr << "SceneParser::addCamera: no resolution provided!" << "\n";
		return false;
	}
	if (attributeValue = getattributevaluebyname(cameraNode, "resolutionY")) {
		if (!stringToNumber<int>(resolutionY, attributeValue)) {
			return false;
		}
	}
	else {
		std::cerr << "SceneParser::addCamera: no resolution provided!" << "\n";
		return false;
	}

	// instanciate camera
	SimpleCamera* camera = new SimpleCamera(resolutionX, resolutionY);
	Vector3 position;
	Vector3 direction;
	Vector3 up;
	float angle = 70;

	// read user specified values
	if (attributeValue = getattributevaluebyname(cameraNode, "position")) {
		if (!stringToVector3<double>(position, attributeValue)) {
			delete(camera);
			return false;
		}
        camera->setPosition(position);
	}

	if (attributeValue = getattributevaluebyname(cameraNode, "direction")) {
		if (!stringToVector3<double>(direction, attributeValue)) {
			delete(camera);
			return false;
		}
		else if (attributeValue = getattributevaluebyname(cameraNode, "up")) {
			if (!stringToVector3<double>(up, attributeValue)) {
				delete(camera);
				return false;
			}
			camera->setOrientation(direction, up);
		}
		else {
			std::cout << "SceneParser::addCamera: direction specified without up\n";
			delete(camera);
			return false;
		}
	}
	else if (attributeValue = getattributevaluebyname(cameraNode, "up")) {
		std::cout << "SceneParser::addCamera: up specified without direction\n";
		delete(camera);
		return false;
	}
	
	if (attributeValue = getattributevaluebyname(cameraNode, "angle")) {
		if (!stringToNumber<float>(angle, attributeValue)) {
			delete(camera);
			return false;
		}
		camera->setOpeningAngle(angle);
	}
	

	// add camera to scene
	scene->setCamera(camera);

	std::cout << "SceneParser::addCamera: added camera\n";

	return true;
}

bool SceneParser::addLight(struct basicxmlnode * lightNode, Scene * scene){
	if (!lightNode) {
		std::cout << "SceneParser::addLight: empty light node \n";
		return false;
	}

	if (std::string(lightNode->tag) != "Light") {
		return false;
	}

	// create light
	PointLight * light = new PointLight();
	Vector3 position;
	Vector4 color;

	// read user specified values
	char * attributeValue;
	if (attributeValue = getattributevaluebyname(lightNode, "position")) {
		if (!stringToVector3<double>(position, attributeValue)) {
			delete(light);
			return false;
		}
		light->setPosition(position);
	}
	if (attributeValue = getattributevaluebyname(lightNode, "color")) {
		if (!stringToVector4<double>(color, attributeValue)) {
			delete(light);
			return false;
		}
		light->setColor(color);
	}

	// add light to scene
	scene->addLight(light);

	std::cout << "SceneParser::addLight: added Light\n";

	return true;
}



bool SceneParser::addElement(struct basicxmlnode * elementNode, Scene * scene){
	if (!elementNode) {
		std::cout << "SceneParser::addElement: empty element node \n";
		return false;
	}

	if (std::string(elementNode->tag) == "TriangleMesh") {
		return addTriangleMesh(elementNode, scene);
	}
	else {
		std::cout << "SceneParser::addElement: do not know how to generate \"" << elementNode->tag << "\"\n";
		return false;
	}
}

// parses material in materialNode and stores it in the scene
bool SceneParser::addGlobalMaterial(struct basicxmlnode * materialNode, Scene * scene){
	if (!materialNode) {
		std::cout << "SceneParser::addGlobalMaterial: empty node \n";
		return false;
	}

	std::string materialName = materialNode->tag;

	// create Material
	Material* new_material = new Material();


	Vector4 vectorValue;
	double shininess;

	// read user specified values
	char * attributeValue;
	if (attributeValue = getattributevaluebyname(materialNode, "emission")) {
		if (!stringToVector4<double>(vectorValue, attributeValue)) {
			delete(new_material);
			return false;
		}
		new_material->emission = vectorValue;
	}
	if (attributeValue = getattributevaluebyname(materialNode, "ambient")) {
		if (!stringToVector4<double>(vectorValue, attributeValue)) {
			delete(new_material);
			return false;
		}
		new_material->ambient = vectorValue;
	}
	if (attributeValue = getattributevaluebyname(materialNode, "diffuse")) {
		if (!stringToVector4<double>(vectorValue, attributeValue)) {
			delete(new_material);
			return false;
		}
		new_material->diffuse = vectorValue;
	}
	if (attributeValue = getattributevaluebyname(materialNode, "specular")) {
		if (!stringToVector4<double>(vectorValue, attributeValue)) {
			delete(new_material);
			return false;
		}
		new_material->specular = vectorValue;
	}
	if (attributeValue = getattributevaluebyname(materialNode, "shininess")) {
		if (!stringToNumber<double>(shininess, attributeValue)) {
			delete(new_material);
			return false;
		}
		new_material->shininess = shininess;
	}

	// add pointer to materiallist of the scene
	scene->addMaterial(materialName, new_material);

	std::cout << "SceneParser::addGlobalMaterial: added global material " << materialName <<"\n";
	return true;
}


// reads the reference of the material and returns a pointer. If no valid reference
// is found it returns a pointer to the default material
Material* SceneParser::getMaterialReference(struct basicxmlnode * materialNode, Scene* scene){
	Material* material = scene->getDefaultMaterial();
	
	
	if (!materialNode) {
		std::cout << "SceneParser::addElementMaterial: empty node \n";
		return material;
	}

	char * attributeValue;

	// see if it is a reference to a global material
	if (attributeValue = getattributevaluebyname(materialNode, "ref")) {
		if (scene->getMaterial(attributeValue)) {
			material = scene->getMaterial(attributeValue);
			std::cout << "SceneParser::getMaterialReference: added global material " << attributeValue << " to element\n";
			return material;
		}
		else {
			std::cout << "SceneParser::getMaterialReference: reffered material not defined\n";
			return material;
		}
	}

	// material ref not found. Use default
	std::cout << "SceneParser::addElementMaterial: material ref not found. Using default.\n";
	
	return material;
}


// tries to load the texture referenced by textureNode and stores it in the scene
bool SceneParser::addGlobalTexture(struct basicxmlnode * textureNode, Scene * scene) {
	if (!textureNode) {
		std::cout << "SceneParser::addGlobalTexture: empty node \n";
		return false;
	}

	std::string textureName = textureNode->tag;
	char* filename;
	if (filename = getattributevaluebyname(textureNode, "file")) {
		std::string filenameS = getattributevaluebyname(textureNode, "file");

#ifdef READ_TEXTURES_FLAG
		// create Texture
		std::string path = directory;

			Image* i = Image::LoadTGA(path.append(filename).c_str());
		if (i) {
			scene->addTexture(textureName, new ImageTexture(i));
			std::cout << "SceneParser::addGlobalTexture: added texture " << textureName <<"\n";
			return true;
		} else {
			std::cout << "SceneParser::addGlobalTexture: couldnt open file " << textureName <<"\n";
			return false;
		}
#else
		return true;
#endif
	}
	else {
		std::cout << "SceneParser::addGlobalTexture: no 'file' field found in " << textureName <<"\n";
		return false;
	}
}

// reads the reference of the texture and returns a pointer. If no valid reference
// is found it returns NULL
ITexture* SceneParser::getTextureReference(struct basicxmlnode* textureNode, Scene* scene) {
	if (!textureNode) {
		std::cout << "SceneParser::addElementMaterial: empty node \n";
		return 0;
	}

	char * attributeValue;

	// see if it is a reference to a global texture
	if (attributeValue = getattributevaluebyname(textureNode, "ref")) {
		if (scene->getTexture(attributeValue)) {
			std::cout << "SceneParser::getTextureReference: added global texture " << attributeValue << " to element\n";
			return scene->getTexture(attributeValue);;
		}
		else {
			std::cout << "SceneParser::getTextureReference: reffered texture not defined\n";
			return 0;
		}
	}

	// texture ref not found. 
	std::cout << "SceneParser::addElementMaterial: material ref not found. Using default.\n";
	
	return 0;
}


bool SceneParser::addTriangleMesh(struct basicxmlnode * elementNode, Scene * scene){
	if (!elementNode) {
		std::cout << "SceneParser::addTriangleMesh: empty node \n";
		return false;
	}

	if (std::string(elementNode->tag) != "TriangleMesh") {
		return false;
	}
	
	// read filename
	char * cobjFileName;
	if (!(cobjFileName = getattributevaluebyname(elementNode, "OBJFileName"))) {
		std::cerr << "SceneParser::addTriangleMesh: no file specified!" << "\n";
		return false;
	}

	// build obj object
  std::string objFileName(cobjFileName);


	bool doTransform = false;
	char * attributeValue;
	Vector3 translate;
	translate[0]=0.0;translate[1]=0.0;translate[2]=0.0;
	if (attributeValue = getattributevaluebyname(elementNode, "translateFromOrigin")) {
		if (!stringToVector3<double>(translate, attributeValue)) {
			return false;
		}else{
			doTransform=true;
		}
	}

	Vector3 rotate;
	if (attributeValue = getattributevaluebyname(elementNode, "rotate")) {
		if (!stringToVector3<double>(rotate, attributeValue)) {
			return false;
		}else{
			doTransform=true;
		}
	}


  Vector3 scale(1.,1.,1.);
	if (attributeValue = getattributevaluebyname(elementNode, "scale")) {
		if (!stringToVector3<double>(scale, attributeValue)) {
			return false;
		}else{
			doTransform=true;
		}
	}

	Material* material;
	struct basicxmlnode * materialNode = getchildnodebyname(elementNode, "Material");
	if(materialNode) {
		material = getMaterialReference(materialNode, scene);
	}

	// set Texture
	struct basicxmlnode * textureNode = getchildnodebyname(elementNode, "Texture");
	ITexture* textureImage = getTextureReference(textureNode, scene);

	// set Bumpmap
	struct basicxmlnode * bumpmapNode = getchildnodebyname(elementNode, "Bumpmap");
	ITexture* bumpmap = getTextureReference(bumpmapNode, scene);
	

	Vector4 color = Vector4(1,0,0,1);
	double reflectionPercentage = 0;
	if (attributeValue = getattributevaluebyname(elementNode, "color")) {
		if (!stringToVector4<double>(color, attributeValue)) {
			return false;
		}
	}
	if (attributeValue = getattributevaluebyname(elementNode, "reflectionPercentage")) {
		if (!stringToNumber<double>(reflectionPercentage, attributeValue)) {
			return false;
		}
	}

  double refractionPercentage = 0;
  double refractionIndex = 1;
  if (attributeValue = getattributevaluebyname(elementNode, "refractionPercentage")) {
		if (!stringToNumber<double>(refractionPercentage, attributeValue)) {
			return false;
		}
	}
  if (attributeValue = getattributevaluebyname(elementNode, "refractionIndex")) {
		if (!stringToNumber<double>(refractionIndex, attributeValue)) {
			return false;
		}
	}

	Mesh *m = new Mesh(0,0);
	m->setMaterial(material);
	
	bool readVertexNormals = false;
	bool readTexture = false;
	std::vector<Vector3 * > vertexNormalList;

	if(!readOBJFile(objFileName,m,scene,readVertexNormals,vertexNormalList,readTexture)){
		delete m;
		return false;
	}

	if(!readVertexNormals)
	{
		calcVertexNormals(m,vertexNormalList);
	}

	//preprocessing(m,newRadius, scale, translate,doScale, normalize);
	if(doTransform)
		preprocessing(m,rotate,scale,translate,vertexNormalList);
	
	for(unsigned int i=0;i<m->numberOfFaces();i++)
	{
		m->getFace(i)->setTexture(textureImage);
		m->getFace(i)->setBumpmap(bumpmap);
		m->getFace(i)->setColor(color);
		m->getFace(i)->setMaterial(material);
		m->getFace(i)->setReflectionPercentage(reflectionPercentage);
		m->getFace(i)->setRefractionPercentage(refractionPercentage);
		m->getFace(i)->setRefractionIndex(refractionIndex);
	}

	scene->addMesh(m);
	return true;
}
