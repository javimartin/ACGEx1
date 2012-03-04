/****************************************************************************
|*  SimpleCamera.cpp
|*
|*  Definition of a SimpleCamera object. It provides the buffer to store an
|*	image. It also generates a ray for a given sample (see Sample.h).
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/


#include "SimpleCamera.h"


SimpleCamera::SimpleCamera(const int res_x, const int res_y) {
	m_pos = Vector3(0.0, 0.0, 0.0);
	setOrientation(Vector3(0.0, 0.0, 1.0), Vector3(0.0, 1.0, 0.0));

	m_openingAngle = 30;
	m_resolutionX = res_x;
	m_resolutionY = res_y;
	
	m_film = new float[3 * res_x * res_y];
	m_hdriFilm = new double[4 * res_x * res_y];
	initFilm();
}


SimpleCamera::SimpleCamera(Vector3 pos, Vector3 dir, Vector3 up,
	       float openingAngle, int res_x, int res_y) 
: 	m_pos(pos),
	m_openingAngle(openingAngle),
	m_resolutionX(res_x), 
	m_resolutionY(res_y)
{
	setOrientation(dir, up);
	m_film = new float[3 * res_x * res_y];
	m_hdriFilm = new double[4 * res_x * res_y];
}


SimpleCamera::~SimpleCamera(void) {
	delete[] m_film;
	delete[] m_hdriFilm;
}


Point SimpleCamera::getResolution(void) {
	return Point(m_resolutionX, m_resolutionY);
}


//generate ray out of a image sample
Ray SimpleCamera::generateRay( const Sample& sample) {
  //calculate pixel size to transform between pixel indices and camera coordinates
	double pixelsize = 2*tan(m_openingAngle/180.0 * PI) / m_resolutionY;

	//C: camera coordinates
	double xC = ((double)sample.getPosX()+sample.getOffset().x - (double)(m_resolutionX-1)/2.0) * pixelsize;
	double yC = ((double)sample.getPosY()+sample.getOffset().y - (double)(m_resolutionY-1)/2.0) * pixelsize;
	Vector3 rayDirC(xC, yC, 1.0);

	//W: world coordinates
	Vector3 rayDirW = m_camToWorld * rayDirC;
	rayDirW.normalize();

	return Ray(m_pos, rayDirW);
}


//write sample to image buffer
void SimpleCamera::setSample( const Sample& s) {

  for (unsigned long j = 0; j < s.getSize(); j++) {
		for (unsigned long i = 0; i < s.getSize(); i++) {
			if ( (s.getPosX()+i) < (unsigned long) m_resolutionX && (s.getPosY()+j) < (unsigned long) m_resolutionY) {

				int pos4 = m_resolutionX*4 * (s.getPosY()+j) + (s.getPosX()+i) *4;
				double alpha = m_hdriFilm[pos4+3];
				for(int k=0;k<3;k++)
					m_hdriFilm[pos4+k] = m_hdriFilm[pos4+k] +  s.getColor()[k] * s.getRenderWeight();
				m_hdriFilm[pos4+3] = alpha + s.getRenderWeight();

				int pos3 = m_resolutionX*3 * (s.getPosY()+j) + (s.getPosX()+i) *3;
				alpha = m_hdriFilm[pos4+3];
				if(alpha > 0.){
					for(int k=0;k<3;k++)
						m_film[pos3+k]	  = (float) std::min( std::max(m_hdriFilm[pos4+k]/alpha,0.), 1.);
				}
			}
		}
	}
}

//get image buffer
float * SimpleCamera::getFilm(void) {
	return m_film;
}

//get color on image pixel
Vector3 SimpleCamera::getPixelColor(int pos_x, int pos_y) {
	Vector3 color;
	color.x = m_film[m_resolutionX*3*pos_y + pos_x*3];
	color.y = m_film[m_resolutionX*3*pos_y + pos_x*3+1];
	color.z = m_film[m_resolutionX*3*pos_y + pos_x*3+2];

	return color;
}



// setter & getter
void SimpleCamera::setPosition(Vector3 position) {
	m_pos = position;
}

Vector3 SimpleCamera::getPosition(void){
	return m_pos;
}

void SimpleCamera::setOrientation(Vector3 direction, Vector3 up) {
	m_originalUp = up.normalize();
	m_dir = direction.normalize();
	m_right = (m_dir.cross(m_originalUp)).normalize();
	// compute up as the second base vector of the camera system
	m_up = (m_right.cross(m_dir)).normalize();

	m_camToWorld = Matrix4(m_right, m_up, m_dir);


}



void SimpleCamera::setOpeningAngle(float a) {
	m_openingAngle = a;
}


// clear image buffer
void SimpleCamera::initFilm(void) {
	for (int i=0; i<3*m_resolutionX * m_resolutionY; i++) m_film[i] = 0;
	for (int i=0; i<4*m_resolutionX * m_resolutionY; i++) m_hdriFilm[i] = 0.;
}

