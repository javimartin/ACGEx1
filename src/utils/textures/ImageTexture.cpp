/****************************************************************************
|*  ImageTexture.cpp
|*
|*  ImageTexture implements a texture which uses a .tga file
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/



#include "ImageTexture.h"


ImageTexture::ImageTexture(Image* image) {
	m_image = image;
}


ImageTexture::~ImageTexture(void) {
	delete(m_image);
}
	
Vector4 ImageTexture::EvaluateTexture(Vector2 uv) {
		//std::cout << " " << uv.x << " " << uv.y << "              ";
		//std::cout << " " << (int)(uv.x*100)%2 << " " << (int)(uv.y*100)%2 << std::endl;
		uv.x = uv.x < 0 ? -1 * uv.x : uv.x;
		uv.y = uv.y < 0 ? -1 * uv.y : uv.y;
		uv.x = uv.x - (int)uv.x;
		uv.y = uv.y - (int)uv.y;


		assert (uv.x <= 1);
		assert (uv.y <= 1);
		assert (uv.x >= 0);
		assert (uv.y >= 0);

		int x = (int)(uv.x * m_image->Width()); if (x>=m_image->Width()) x--;
		int y = (int)(uv.y * m_image->Height()); if (y>=m_image->Height()) y--;
		Vector3 v = m_image->GetPixel(x,y);
		return Vector4(v.x, v.y, v.z, 1);
}