/****************************************************************************
|*  SimpleCamera.h
|*
|*  Declaration of the class SimpleCamera.
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/


#ifndef _SIMPLECAMERA_H
#define _SIMPLECAMERA_H


#include <sceneelements/ICamera.h>
#include <rendererelements/Sampler/Sample.h>
#include <utils/Matrix4.h>

#define PI 3.1415926

class SimpleCamera : public ICamera {

public:
	SimpleCamera(int res_x, int res_y);


	SimpleCamera(Vector3 pos, Vector3 dir, Vector3 up,
	       float openingAngle, int res_x, int res_y);


	~SimpleCamera(void);


	virtual Point getResolution(void);

	//generate ray out of a image sample
	virtual Ray generateRay(const Sample& sample);

	//write sample to image buffer
	virtual void setSample(const Sample& s);

	//get color on image pixel
	virtual Vector3 getPixelColor(int pos_x, int pos_y);

	//get image buffer
	virtual float * getFilm(void);

	// clear image buffer
	virtual void initFilm(void);


	// setter & getter
	void setPosition(Vector3 position);
	Vector3 getPosition(void);
	void setOrientation(Vector3 direction, Vector3 up);
	void setOpeningAngle(float a);

private:
	Vector3 m_pos;
	Vector3 m_dir;
	Vector3 m_up;
	Vector3 m_originalUp;
	Vector3 m_right;

	float m_openingAngle;//half angle of y axis aperture

	int m_resolutionX;
	int m_resolutionY;

	double* m_hdriFilm;
	float* m_film;

	Matrix4 m_camToWorld;
};


#endif //_SIMPLECAMERA_H
