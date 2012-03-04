/****************************************************************************
|*  Sample.h
|*
|*  Declaration of the class Sample.
|*  A Sample represents a pixel of the cameras film. It contains information about
|*  the position and color of this pixel.
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/


#ifndef _SAMPLE_H
#define _SAMPLE_H


#include <utils/Vector2.h>
#include <utils/Vector4.h>


class Sample  {

public:
	Sample(int x, int y);

	Sample(int x, int y, Vector4 color);

	~Sample(void);

	
	void setPosX(int x);
	int getPosX(void) const ;

	
	void setPosY(int y);
	int getPosY(void) const;


	void setSize(unsigned long size);
	unsigned long getSize(void) const;


	void setColor(Vector4 color);
	void setColor(double r, double g, double b, double a);
	Vector4 getColor(void) const;


	void setColR(double r);
	double getColR(void) const;
	
	void setColG(double g);
	double getColG(void) const ;
	
	void setColB(double b);
	double getColB(void) const ;

	void setAlpha(double a);
	double getAlpha(void) const ;


	// Weight of the sample color when written into buffer
  // default: 1
	void setRenderWeight(double w);
	double getRenderWeight() const;

  // Weight of the old color in the buffer, when this sample is added to it (buffercolor = oldColorRenderWeight*buffercolor + renderWeight*thiscolor)
  // default: 0
	void setOldColorRenderWeight(double w);
	double getOldColorRenderWeight() const;

	Vector2 getOffset() const;
	void setOffset(Vector2 _offset);

	void set(Vector2 _offset, Vector4 _color);


private:

	int m_x;
	int m_y;

	unsigned long m_size;

	Vector4 m_color;

	Vector2 offset;
	double m_renderWeight;
	double m_oldColorRenderWeight;

};


#endif //_SAMPLE_H
