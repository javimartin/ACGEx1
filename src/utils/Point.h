/****************************************************************************
|*  Point.h
|*
|*  Definition of a 2d integer vector and its basic functionality.
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/



#ifndef _POINT_H
#define _POINT_H


#include <math.h>
#include <assert.h>


class Point {
public:
	// Point Methods
	Point(int _x=0, int _y=0)
		: x(_x), y(_y) {
	}

	Point operator+(const Point &v) const {
		return Point(x + v.x, y + v.y);
	}
	
	Point& operator+=(const Point &v) {
		x += v.x; y += v.y;
		return *this;
	}

	Point operator-(const Point &v) const {
		return Point(x - v.x, y - v.y);
	}
	
	Point& operator-=(const Point &v) {
		x -= v.x; y -= v.y;
		return *this;
	}

	bool operator==(const Point &v) const {
		return x == v.x && y == v.y;
	}

	Point operator*(int f) const {
		return Point(f*x, f*y);
	}
	
	Point &operator*=(int f) {
		x *= f; y *= f;
		return *this;
	}

	Point operator-() const {
		return Point(-x, -y);
	}

	int &operator[](int i) {
		assert(i >= 0 && i <= 1);
		switch (i) {
			case 0: return x;
			case 1: return y;
		}
	} 


	// Point Data
	int x, y;
};


#endif //_POINT_H
