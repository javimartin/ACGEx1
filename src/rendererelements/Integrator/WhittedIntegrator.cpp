/****************************************************************************
|*  WhittedIntegrator.h
|*
|*  Performs classical recursive raytracing to determine a rays incoming radiance
|*
|*  Mario Deuss, mario.deuss@epfl.ch
\***********************************************************/


#include "WhittedIntegrator.h"

#include <Scene.h>

WhittedIntegrator::WhittedIntegrator()
{
	m_recursionDepth = 0;

}

Vector4 WhittedIntegrator::integrate( const Ray& ray )
{
	
	std::vector<double> refractionStack;
	return integrate(ray, refractionStack);

}

Vector4 WhittedIntegrator::integrate( const Ray& ray, std::vector<double>& refractionStack )
{
	Scene* scene = m_scene;

	Vector4 color;

	IntersectionData* iData = scene->intersect(ray);

	if (iData) { // successful intersection test

		int refractionStackSize = static_cast<int>(refractionStack.size());

		// determine refraction indices
		if (iData->rayEntersObject) {
			if(refractionStackSize > 0) {
				iData->refractionIndexOutside = refractionStack.back();
			}
			else {
				iData->refractionIndexOutside = scene->getRefractionIndex();
			}
		}
		else {
			if(refractionStackSize > 1) {       
				iData->refractionIndexOutside = refractionStack[refractionStackSize-2];
			}
			else {
				iData->refractionIndexOutside = scene->getRefractionIndex();
			}
		}

		// shade color
		if (ray.depth == m_recursionDepth) { //if this is the last recursion step shade object
			color = shade(iData, scene);
		}
		else {//depth < m_recursionDepth -> recursion

			//calculate weighted color from diffuse light
			//assumed: refractionPercentage + refractionPercentage <= 1
			assert(iData->reflectionPercentage + iData->refractionPercentage <= 1);
			color = shade(iData, scene)  * (1 - iData->reflectionPercentage - iData->refractionPercentage);

			//recDepth++;
			double cumulatedReflectionPercentage = iData->reflectionPercentage;

			//refraction
			if (iData->refractionPercentage != 0) {
				//get source direction (pointing towards the source)
				Vector3 sourceDir = -ray.direction;

				Vector3 normal = iData->shadingNormal;
				double n1, n2;
				int stackOperation = 0; //0->nothing, 1 push, -1 pop
				if (iData->rayEntersObject) {
					//normal needs to point into the medium where the ray comes from
					n1 = iData->refractionIndexOutside;
					n2 = iData->refractionIndexInside;
					refractionStack.push_back(n2);
					stackOperation = 1;
				}
				else {
					//normal needs to point into the medium where the ray comes from
					normal = -normal;
					n1 = iData->refractionIndexInside;
					n2 = iData->refractionIndexOutside;
					if(refractionStackSize > 0) {
						refractionStack.pop_back();
						stackOperation = -1;
					}
				}

				//theta1 will always be between 0 and PI/2 so the cos is unique
				double c_theta1 = sourceDir.dot(normal);
				double theta1 = acos(c_theta1);


				//check whether a total reflection occurs and compute target direction
				Vector3 targetDir;
				if (sin(theta1) > n2/n1) { //total reflection
					cumulatedReflectionPercentage += iData->refractionPercentage;
				}
				else { //regular refraction
					double c_theta2 = sqrt(1 - (n1/n2)*(n1/n2) * (1 - c_theta1*c_theta1));
					targetDir = sourceDir * (n1/n2) + normal * (c_theta2 - (n1/n2) * c_theta1);
					targetDir.normalize();
					targetDir = -targetDir;

					Ray refractedRay(iData->position, targetDir);
					refractedRay.depth = ray.depth + 1;
					refractedRay.min_t = Ray::epsilon_t;

					//send out refracted/total-reflected ray
					color += integrate( refractedRay, refractionStack ) * iData->refractionPercentage;
				}

				//restore stack
				if (stackOperation == 1) {
					refractionStack.pop_back();
				}
				else if (stackOperation == -1) {
					refractionStack.push_back(iData->refractionIndexInside);
				}

			}


			//reflection
			if (cumulatedReflectionPercentage != 0) {
				Vector3 sourceDir = -ray.direction;
				Vector3 normal = iData->shadingNormal;
				Vector3 targetDir = (normal*(normal.dot(sourceDir))*2 - sourceDir).normalize();

				Ray reflectedRay(iData->position, targetDir);
				reflectedRay.depth = ray.depth + 1;
				reflectedRay.min_t = Ray::epsilon_t;

				color += integrate( reflectedRay, refractionStack ) * cumulatedReflectionPercentage;
			}
		}

	}
	else {
		color = scene->getBackground(); // background color
	}


	// clean
	if (iData!=NULL){
		delete(iData);
	}

	return color.clamp01();

}

Vector4 WhittedIntegrator::shade( IntersectionData* iData, Scene* scene )
{
	if( m_shader == CONSTANT ){

		return iData->material->diffuse;

	}else if( m_shader == DIFFUSE ){

		Vector4 color_tmp;
		Vector3 lightDirection;
		std::vector<ILight*> lights =  scene->getNonOccludedLights(iData->position);
		for (unsigned int i=0; i<lights.size(); i++) {
			// light direction
			lightDirection = (lights[i]->getPosition() - iData->position).normalize();

			// orient normal towards look at
			Vector3 sourceDir = (iData->sourcePosition - iData->position).normalize();
			Vector3 orientedNormal = iData->shadingNormal;
			if(iData->shadingNormal.dot(sourceDir)<0) {
				orientedNormal = -orientedNormal;
			}

			double cos_th = (orientedNormal).dot(lightDirection);
			if (cos_th > 0 || iData->refractionPercentage > 0) { // look at and light are on same side of the tangent plane or object is refractive
				color_tmp += iData->material->diffuse.componentMul((lights[i]->getColor())*fabs(cos_th));
			}
		}
		lights.clear();
		return color_tmp.clamp01();

	}else if( m_shader == PHONG){

		//attenuation parameter
		double m_dC=1.;
		double m_dL=0.;
		double m_dQ=0.;

		Vector3 lightDir;
		double lightDist, cos_th, cos_rh, attenuation;

		// add Emission
		Vector4 color_tmp(0,0,0); 

		color_tmp	+= iData->material->emission;

		// add Ambient
		color_tmp += scene->getAmbient().componentMul(iData->material->ambient);

		// Compute for every light in the scene
		std::vector<ILight*> lights =  scene->getNonOccludedLights(iData->position);
		for (unsigned int i=0; i<lights.size(); i++) {

			// Diffuse + Specular
			lightDir = (lights[i]->getPosition() - iData->position).normalize();
			Vector3 sourceDir = (iData->sourcePosition - iData->position).normalize();
			Vector3 orientedNormal = iData->shadingNormal; // orient normal towards look at
			if(orientedNormal.dot(sourceDir)<0) {
				orientedNormal = -orientedNormal;
			}

			cos_th = (orientedNormal).dot(lightDir);
			if (cos_th > 0 || iData->refractionPercentage > 0) { // look at and light are on same side of the tangent plane or object is refractive

				// distance parameter	
				lightDist = (lights[i]->getPosition() - iData->position).length();
				attenuation = 1.0/(m_dC + m_dL*lightDist + m_dQ*lightDist*lightDist);

				// add Diffuse
				if (iData->texture!=0) { //(USES TEXTURE)
					Vector2 texc = iData->textureCoords;
					color_tmp += iData->texture->EvaluateTexture(iData->textureCoords).componentMul((lights[i]->getColor())*fabs(cos_th));
				}
				else {
					color_tmp += (iData->material->diffuse.componentMul((lights[i]->getColor())*fabs(cos_th))) * attenuation;
				}
				// add Specular
				Vector3 bouncedLightDir;
				if(cos_th > 0) { // reflective specularity
					bouncedLightDir = (orientedNormal*(cos_th*2) - lightDir).normalize(); // reflected light vector
				}
				else { // refractive specularity -> snells law for the light-source (not the camera-ray!) direction
					Vector3 normal = iData->shadingNormal;
					double n1, n2;
					if (lightDir.dot(normal) > 0) { // light ray enters object
						//normal needs to point into the medium where the light ray comes from
						n1 = iData->refractionIndexOutside;
						n2 = iData->refractionIndexInside;
					}
					else { // light ray leaves object
						//normal needs to point into the medium where the ray comes from
						normal = -normal;
						n1 = iData->refractionIndexInside;
						n2 = iData->refractionIndexOutside;
					}
					//theta1 will always be between 0 and PI/2 so the cos is unique
					double c_theta1 = lightDir.dot(normal);
					double theta1 = acos(c_theta1);
					//check whether a total reflection occurs and compute target direction
					if (sin(theta1) > n2/n1) { //total reflection
						bouncedLightDir = (orientedNormal*(cos_th*2) - lightDir).normalize(); // reflected light vector
					}
					else { //regular refraction
						double c_theta2 = sqrt(1 - (n1/n2)*(n1/n2) * (1 - c_theta1*c_theta1));
						bouncedLightDir = - ( lightDir * (n1/n2) + normal * (c_theta2 - (n1/n2) * c_theta1) );
						bouncedLightDir.normalize();
					}
				}
				cos_rh = sourceDir.dot(bouncedLightDir);
				if (cos_rh > 0) {
					color_tmp += (iData->material->specular.componentMul((lights[i]->getColor())*pow(cos_rh, iData->material->shininess))) * attenuation;
				}
			}
		}

		lights.clear();
		return color_tmp.clamp01();
	}else if ( m_shader == PHONGBUMP){

		//attenuation parameter
		double m_dC=1.;
		double m_dL=0.;
		double m_dQ=0.;

		Vector4 color_tmp(0,0,0,1); 


		// Preparate materials ////////////////////
		if (iData->material == 0) {
			iData->material = scene->getDefaultMaterial();
		}
		// Emission
		Vector4 Emission = iData->material->emission;
		// Ambient
		Vector4 Ambient = iData->material->ambient.componentMul(scene->getAmbient());
		// Diffuse
		Vector4 Diffuse = iData->material->diffuse; 
		// Specular
		Vector4 Specular = iData->material->specular;


		// Texturing
		if (iData->texture) {
			Diffuse = iData->texture->EvaluateTexture(iData->textureCoords);
		}


		// Prepare Normal ////////////////////
		Vector3 Normal = iData->shadingNormal;
		if (iData->bumpmap) {
			// read bumpmap
			Vector4 displace = iData->bumpmap->EvaluateTexture(iData->textureCoords);
			// map the [0,1] color to [-1,1] normal components R->x G->y B->z
			displace = displace * 2 + Vector4(-1,-1,-1,0);
			// use Normal from normal map instead
			double perturbationFactor = 1;
			Normal  = iData->localX * displace.x * perturbationFactor;
			Normal += iData->localY * displace.y * perturbationFactor;
			Normal += iData->localZ * displace.z * perturbationFactor;

			Normal.normalize();
		}


		// shade color /////////////////
		// add Emission
		color_tmp = Emission;

		// add ambient
		color_tmp += Ambient;

		// Compute for every light in the scene
		std::vector<ILight*> lights =  scene->getNonOccludedLights(iData->position);
		for (unsigned int i=0; i<lights.size(); i++) {

			// Diffuse + Specular
			Vector3 lightDir = (lights[i]->getPosition() - iData->position).normalize();

			double cos_th = (Normal).dot(lightDir);
			if (cos_th > 0) {

				// distance parameter	
				double lightDist = (lights[i]->getPosition() - iData->position).length();
				double d = 1.0/(m_dC + m_dL*lightDist + m_dQ*lightDist*lightDist);

				// add Diffuse
				color_tmp += Diffuse.componentMul((lights[i]->getColor())*cos_th)* d;



				// add Specular
				Vector3 lightRefl = (Normal*(cos_th*2) - lightDir).normalize(); // reflected light vector
				Vector3 camDir = (iData->sourcePosition - iData->position).normalize();
				double cos_rh = camDir.dot(lightRefl);
				if (cos_rh > 0) 
					color_tmp += Specular.componentMul((lights[i]->getColor())*pow(cos_rh, iData->material->shininess)) * d;
			}
		}

		lights.clear();
		return color_tmp.clamp01();

	}


	return Vector4(0.0,0.,0.,1.);
}
