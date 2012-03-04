/****************************************************************************
|*  ConfigParser.h
|*
|*  ConfigParser class. Parses the Ray Tracers Config file and generates an appropriate renderer.
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/

#ifndef _CONFIG_PARSER_H
#define _CONFIG_PARSER_H

#include <Renderer.h>

#include <string>
#include <sstream>
#include <map>

//Forward Declaration
class WhittedIntegrator;

class ConfigParser {

public:

	ConfigParser(void);

	~ConfigParser(void);

	//parse a config file and generate a Renderer
	Renderer* parse(std::string filename);

private://data

private://methods

	bool addRendererProperties(struct basicxmlnode * rendererNode, Renderer * renderer);
	bool addSampler(struct basicxmlnode * samplerNode, Renderer * renderer);
	bool addIntegrator(struct basicxmlnode * integratorNode, Renderer * renderer);

	//Special methods for whitted raytracing
	bool addShader(struct basicxmlnode * shaderNode, WhittedIntegrator* whittedIntegrator );


};

#endif //_CONFIG_PARSER_H
