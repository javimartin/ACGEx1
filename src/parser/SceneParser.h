/****************************************************************************
|*  SceneParser.h
|*
|*  SceneParser class. Parses a scene description file and construct scene.
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/

#ifndef _SCENE_PARSER_H
#define _SCENE_PARSER_H


#include <string>
#include <map>
#include <stdlib.h>
#include <vector>

#include <Scene.h>
#include <utils/Vector3.h>
#include <utils/Vector4.h>
#include <utils/Material.h>

#include <limits>

#include <utils/Image.h>

class SceneParser {

public:

	SceneParser(void);

	~SceneParser(void);

	//parse a scene description file and generate a Scene
	Scene* parse(const char* filename);

private://data

private://methods
	bool addSceneProperties(struct basicxmlnode * sceneNode, Scene * scene);
	bool addCamera(struct basicxmlnode * cameraNode, Scene * scene);
	bool addLight(struct basicxmlnode * lightNode, Scene * scene);
	bool addElement(struct basicxmlnode * elementNode, Scene * scene);
	bool addGlobalMaterial(struct basicxmlnode * materialNode, Scene * scene);
	bool addGlobalTexture(struct basicxmlnode * textureNode, Scene * scene);

	// reads the reference of the material and returns a pointer. If no valid reference
	// is found it returns a pointer to the default material
	Material* getMaterialReference(struct basicxmlnode * materialNode, Scene* scene);

	// reads the reference of the texture and returns a pointer. If no valid reference
	// is found it returns NULL
	ITexture* getTextureReference(struct basicxmlnode* textureNode, Scene* scene);

	bool addTriangleMesh(struct basicxmlnode * elementNode, Scene * scene);

	// helper that removes whitespace on front and back of string
	std::string removeWhiteSpaceFromString(std::string s) {
		return s.substr(s.find_first_not_of(' '), s.find_last_not_of(' ') - s.find_first_not_of(' ') + 1);
	}

  // relative directory path of scene description file
  std::string directory;


};

#endif //_SCENE_PARSER_H
