/****************************************************************************
|*  Raytracer Framework
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <assert.h>
#include <utils/Vector3.h>

// ====================================================================
// ====================================================================
// Simple image class

class Image {

public:

  // ========================
  // CONSTRUCTOR & DESTRUCTOR
  Image(int w, int h) {
    width = w;
    height = h;
    data = new Vector3[width*height]; }
  ~Image() {
    delete [] data; }

  // =========
  // ACCESSORS
  int Width() const { return width; }
  int Height() const { return height; }
  const Vector3& GetPixel(int x, int y) const {
    assert(x >= 0 && x < width);
    assert(y >= 0 && y < height);
    return data[y*width + x]; }
  
  // =========
  // MODIFIERS
  void SetAllPixels(const Vector3 &color) {
    for (int i = 0; i < width*height; i++) {
      data[i] = color; } }
  void SetPixel(int x, int y, const Vector3 &color) {
    assert(x >= 0 && x < width);
    assert(y >= 0 && y < height);
    data[y*width + x] = color; }

  // ===========
  // LOAD & SAVE

  static Image* LoadTGA(const char *filename);
  void SaveTGA(const char *filename) const; 
  
  // extension for image comparison
  static Image* Compare(Image* img1, Image* img2);
  
private:

  // ==============
  // REPRESENTATION
  int width;
  int height;
  Vector3 *data;

};

// ====================================================================
// ====================================================================

#endif
