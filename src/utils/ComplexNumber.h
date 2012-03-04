/****************************************************************************
|*  Raytracer Framework
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/

#ifndef _COMPLEXNUMBER_H
#define _COMPLEXNUMBER_H

class ComplexNumber {
public:
	// Vector3 Methods
	ComplexNumber(double _real=0, double _img=0)
		: real(_real), img(_img) {
	}

	ComplexNumber operator+(const ComplexNumber &v) const {
		return ComplexNumber(real + v.real, img + v.img);
	}
	
	ComplexNumber& operator+=(const ComplexNumber &v) {
		real += v.real; img += v.img;
		return *this;
	}

	ComplexNumber operator-(const ComplexNumber &v) const {
		return ComplexNumber(real - v.real, img - v.img);
	}
	
	ComplexNumber& operator-=(const ComplexNumber &v) {
		real -= v.real; img -= v.img;
		return *this;
	}

	bool operator==(const ComplexNumber &v) const {
		return real == v.real && img == v.img;
	}

	ComplexNumber operator*(double f) const {
		return ComplexNumber(f*real, f*img);
	}

	ComplexNumber operator*(ComplexNumber f) const {
		return ComplexNumber(real*f.real-img*f.img, real*f.img+img*f.real);
	}
	
	ComplexNumber &operator*=(double f) {
		real *= f; img *= f;
		return *this;
	}

	ComplexNumber &operator*=(ComplexNumber f) {
		real = real*f.real-img*f.img; img = real*f.img+img*f.real;
		return *this;
	}

	ComplexNumber operator/(double f) const {
		assert(f!=0);
		double inv = 1.f / f;
		return ComplexNumber(real * inv, img * inv);
	}
	
	ComplexNumber &operator/=(double f) {
		assert(f!=0);
		double inv = 1.f / f;
		real *= inv; img *= inv;
		return *this;
	}

	ComplexNumber operator-() const {
		return ComplexNumber(-real, -img);
	}

	double real, img;
};


#endif
