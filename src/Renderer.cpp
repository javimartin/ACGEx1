/****************************************************************************
|*  Renderer.cpp
|*
|*  Definition of a Simple renderer. The renderer takes a pointer to a
|*  scene and renders it. The resulting image is stored in the camera
|*	of the scene itself. 
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/


#include <Renderer.h>

//#define PARALLELIZATION
//#define NUMBER_OF_THREADS 16
//#include <omp.h>

Renderer::Renderer() {
  m_sampler = 0;
}

Renderer::~Renderer(void){
  if (m_sampler) {
	  delete(m_sampler);
  }
}

//main renderloop that raytraces the given scene
void Renderer::render(Scene* scene){
	if (scene) {
		// Initialize Sampler
		Point p = scene->getCamera()->getResolution();
		m_sampler->init(p.x, p.y);

		// Initialize fields
		scene->getCamera()->initFilm();
		m_integrator->setScene(scene);

		// Renderloop with counter
		int sampleCount = 0;
		int n = m_sampler->getNumberOfSamples();

#ifndef PARALLELIZATION
		
		Sample* sample = new Sample(0,0);
		Ray ray;

		for( int i=0; i<n;i++){
			m_sampler->getSample(i,sample);
			ray = scene->getCamera()->generateRay(*sample);
			sample->setColor(m_integrator->integrate( ray));
			scene->setSample(*sample);
		
			sampleCount++;
			if( sampleCount%((int)(n/10.)) < (sampleCount-1)%((int)(n/10.)) ){
				std::cout << "[" << 100.*static_cast<double>(sampleCount)/static_cast<double>(n) << "%] ";
				notifyObservers();
			}
		}

		delete sample;
#else

		Sample* samples[NUMBER_OF_THREADS];
		for(int i=0;i<NUMBER_OF_THREADS;i++)
			samples[i] = new Sample(0,0);

		omp_set_num_threads(NUMBER_OF_THREADS);

#pragma omp parallel for
		for( int i=0; i<n;i++){
			Sample* sample = samples[omp_get_thread_num()];
			m_sampler->getSample(i,sample);
			Ray ray = scene->getCamera()->generateRay(*sample);
			sample->setColor(m_integrator->integrate( ray));
			scene->setSample(*sample);
		
			if( omp_get_thread_num() == 0 ){
				sampleCount++;
				if(sampleCount%1000==0){
					std::cout << ".";
					notifyObservers();
				}
			}
		}

		for(int i=0;i<NUMBER_OF_THREADS;i++)
			delete samples[i];
#endif

	}
}

void Renderer::setSampler(ISampler* sampler) {
	m_sampler = sampler;
}

double Renderer::status() {
	return 0.;
 // return m_sampler->percentageOfGeneratedSamples();
}