/****************************************************************************
|*  ConfigParser.h
|*
|*  ConfigParser class. Parses the Ray Tracers Config file and generates an appropriate renderer.
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/

#include <iostream>

#include <Renderer.h>
#include <utils/string_conversions.h>
#include <parser/SimpleXMLNode.h>

#include <rendererelements/Sampler/ISampler.h>
#include <rendererelements/Integrator/Integrator.h>
#include <rendererelements/Integrator/WhittedIntegrator.h>
#include <rendererelements/Integrator/DirectLighting.h>
#include <rendererelements/Integrator/PathTracer.h>

#include <rendererelements/Sampler/SuperSampler.h>



#include "ConfigParser.h"


ConfigParser::ConfigParser(void){
}


ConfigParser::~ConfigParser(void){
}


//parse a config file and generate a Renderer
Renderer* ConfigParser::parse(std::string filename){
	
	std::cout << "ConfigParser::parse config file... " << filename <<" \n";

	struct basicxmlnode * rootNode = NULL;

	//open file
	FILE * fp = fopen(filename.c_str(), "rt");
	if (!fp) {
		std::cerr << "ConfigParser - Error: Failed opening file " << filename << "\n";
		return NULL;
	}

	rootNode = readbasicxmlnode(fp);
	fclose(fp);
	if (!rootNode) {
		std::cerr << "ConfigParser - Error: Failed reading file " << filename << ". Maybe an XML syntax error?\n";
		return NULL;
	}

	//create renderer
	Renderer* renderer = new Renderer();

	//read renderer properties
	if (!addRendererProperties(rootNode, renderer)) {
		std::cerr << "ConfigParser - Error: Failed reading renderer properties in " << filename << "\n";
		deletebasicxmlnode(rootNode);
		delete(renderer);
		return NULL;
	}

	//read sampler
	struct basicxmlnode * samplerNode = getchildnodebyname(rootNode, "Sampler");
	if (!addSampler(samplerNode, renderer)) {
		std::cerr << "ConfigParser - Error: Failed reading sampler description in " << filename << "\n";
		deletebasicxmlnode(rootNode);
		delete(renderer);
		return NULL;
	}
	
	//read integrator
	struct basicxmlnode * integratorNode = getchildnodebyname(rootNode, "Integrator");
	if (!addIntegrator(integratorNode, renderer)) {
		std::cerr << "ConfigParser - Error: Failed reading integrator description in " << filename << "\n";
		deletebasicxmlnode(rootNode);
		delete(renderer);
		return NULL;
	}

	//free xml memory
	deletebasicxmlnode(rootNode);

	std::cout << "[done]\n\n";

	return renderer;
}

bool ConfigParser::addRendererProperties(struct basicxmlnode * rendererNode, Renderer * renderer) {

	if (!rendererNode) {
		std::cout << "ConfigParser::addRendererProperties: empty renderer node\n";
		return false;
	}

	if (std::string(rendererNode->tag) != "Renderer") {
		std::cout << "ConfigParser::addRendererProperties: config file has to have toplevel Renderer node\n";
		return false;
	}

	//add loading of specific renderer settings here

	return true;
}

bool ConfigParser::addSampler(struct basicxmlnode * samplerNode, Renderer * renderer){
	if (!samplerNode) {
		std::cout << "ConfigParser::addSampler: empty sampler node\n";
		return false;
	}

	if (std::string(samplerNode->tag) != "Sampler") {
		std::cout << "ConfigParser::addSampler: wrong sampler tag name\n";
		return false;
	}

	// read sampler type
	std::string type = getattributevaluebyname(samplerNode, "type");

	 if (type == "SuperSampler") {
		double sample_pixel=1;
		char* attributeValue;
		if (attributeValue = getattributevaluebyname(samplerNode, "mSubsamples")) {
			if (!stringToNumber<double>(sample_pixel, attributeValue)) {
				return false;
			}
		}
		bool jitter = false;
		if (attributeValue = getattributevaluebyname(samplerNode, "jitter")) {
			std::string jittervalue = getattributevaluebyname(samplerNode, "jitter");
			jitter = (jittervalue == "yes") ? true : false;
		}

		ISampler* s = new SuperSampler((int)sample_pixel, jitter);
		renderer->setSampler(s);
	}
	else 
	{
		std::cout << "ConfigParser::addSampler: unknown sampler specified\n";
		return false;
	}

	return true;
}

bool ConfigParser::addIntegrator( struct basicxmlnode * integratorNode, Renderer * renderer )
{
	if (!integratorNode) {
		std::cout << "ConfigParser::addIntegrator: empty integrator node\n";
		return false;
	}

	if (std::string(integratorNode->tag) != "Integrator") {
		std::cout << "ConfigParser::addIntegrator: wrong integrator tag name\n";
		return false;
	}

	std::string type = getattributevaluebyname(integratorNode, "type");
	if (type == "WhittedIntegrator") {
		struct basicxmlnode * shaderNode = getchildnodebyname(integratorNode, "Shader");
		WhittedIntegrator* integrator = new WhittedIntegrator();
		if(addShader(shaderNode,integrator)){
			renderer->setIntegrator( integrator );
		}else{
			return false;
		}

		//read recursion depth
		unsigned int recursionDepth;
		char * attributeValue;
		if (attributeValue = getattributevaluebyname(integratorNode, "recursionDepth")) {
			if (!stringToNumber<unsigned int>(recursionDepth, attributeValue)) {
				return false;
			}
			integrator->setRecursionDepth(recursionDepth);
		}
	}else if (type == "DirectLighting") {
		DirectLighting* integrator = new DirectLighting();
		renderer->setIntegrator(integrator);

		//read number of samples
		unsigned int nSamples;
		char * attributeValue;
		if (attributeValue = getattributevaluebyname(integratorNode, "nSamples")) {
			if (!stringToNumber<unsigned int>(nSamples, attributeValue)) {
				return false;
			}
			integrator->setNSamples(nSamples);
		}
	}else if (type == "PathTracer") {
		PathTracer* integrator = new PathTracer();
		renderer->setIntegrator(integrator);

		//read number of samples
		unsigned int nSamples;
		char * attributeValue;
		if (attributeValue = getattributevaluebyname(integratorNode, "nSamples")) {
			if (!stringToNumber<unsigned int>(nSamples, attributeValue)) {
				return false;
			}
			integrator->setNSamples(nSamples);
		}

		//read max. depth
		unsigned int maxDepth;
		if (attributeValue = getattributevaluebyname(integratorNode, "maxDepth")) {
			if (!stringToNumber<unsigned int>(maxDepth, attributeValue)) {
				return false;
			}
			integrator->setMaxDepth(maxDepth);
		}

		//read sample depth
		unsigned int sampleDepth;
		if (attributeValue = getattributevaluebyname(integratorNode, "sampleDepth")) {
			if (!stringToNumber<unsigned int>(sampleDepth, attributeValue)) {
				return false;
			}
			integrator->setSampleDepth(sampleDepth);
		}

		//read continue probabilty
		double p;
		if (attributeValue = getattributevaluebyname(integratorNode, "pContinue")) {
			if (!stringToNumber<double>(p, attributeValue)) {
				return false;
			}
			integrator->setContinueProbabilty(p);
		}
	}
	//Add loading of other integrator types here

	return true;

}


bool  ConfigParser::addShader(struct basicxmlnode * shaderNode, WhittedIntegrator* whittedIntegrator ){
	if (!shaderNode) {
		std::cout << "ConfigParser::addShader: empty shader node\n";
		return false;
	}

	if (std::string(shaderNode->tag) != "Shader") {
		std::cout << "ConfigParser::addShader: wrong shader tag name\n";
		return false;
	}

	// read shader type
	std::string type = getattributevaluebyname(shaderNode, "type");
	if (type == "ConstantShader") {
		whittedIntegrator->setShader(WhittedIntegrator::CONSTANT );
	}
	else if (type == "DiffuseLightingShader") {
		whittedIntegrator->setShader(WhittedIntegrator::DIFFUSE);
	}
	else if (type == "PhongLightingShader") {
		whittedIntegrator->setShader(WhittedIntegrator::PHONG);
	}
#ifdef READ_BUMPMAP_FLAG 
	else if (type == "PhongBumpShader") {
		whittedIntegrator->setShader(WhittedIntegrator::PHONGBUMP);
	}
#endif
	else {
		std::cout << "ConfigParser::addShader: unknown shader specified\n";
		return false;
	}

	return true;
}



