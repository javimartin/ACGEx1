/****************************************************************************
|*  Raytracer Framework
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/

#include "OBJFileReader.h"
#include "../sceneelements/geometry/Mesh.h"
#include "../sceneelements/geometry/MeshVertex.h"
#include "../sceneelements/geometry/MeshTriangle.h"
#include "../Scene.h"

#include <../utils/Vector2.h>
#include <../utils/Vector3.h>
#include <../utils/Vector4.h>
#include <../utils/Material.h>

#define _USE_MATH_DEFINES
#include <cmath> 
#include <limits>

#ifndef M_PI
#define M_PI 3.141592653589793238462
#endif

bool readOBJFile(const std::string &filename, Mesh * _mesh, Scene * _scene, bool & _readVertexNormals,	std::vector<Vector3 * >& vertexNormalList ,bool & _readTexture) {

	std::vector<Vector2 * > vertexTextureList;
	vertexNormalList.clear();
	vertexTextureList.clear();
  // Load the mesh.  This routine supports a simplified version of the
  // .obj file format.  Lines begin with either '#' to indicate a comment,
  // 'v' to indicate a vertex, or 'f' to indicat a face.  Comments are
  // ignored.  ** Vertex indices (in 'f' lines) are one based, so they 
  // start with one and not zero. **  Faces can only have three indices --
  // so they must be triangles.  Any other tag in the file is ignored.

  printf("Loading mesh \"%s\".\n", filename.c_str());

  // Load a simplified version of the .obj file format.

  // Open the file.
  FILE *stream = fopen(filename.c_str(), "r");
  
  if (stream == NULL) {
    printf("Error opening file \"%s\" for reading.\n", filename.c_str());
    return false;
  }

  // Read the contents.

  bool unknownWarning = false;
  char str[256];
  double x, y, z;
	double tx,ty;
  int i0, i1, i2;
	int it0, it1, it2;
	int in0, in1, in2;
  int v;
	printf("starting\n");
  while (fscanf(stream, "%s", str) == 1) {

    if (str[0] == '#') {

      // Comment
      // Read until the next newline.
      
      while (1) {
        char c;
        v = fscanf(stream, "%c", &c);
        if ((c == '\n') || (v != 1))
          break;
      }
      v=1;

    } else if (strcmp(str, "vn") == 0) {
 /*     while (1) {
        char c;
        v = fscanf(stream, "%c", &c);
        if ((c == '\n') || (v != 1))
          break;
      }*/
	
	
	  double nx, ny, nz;
      v = fscanf(stream, "%lf", &nx);
      v = fscanf(stream, "%lf", &ny);
      v = fscanf(stream, "%lf", &nz);
			Vector3 * newNormal = new Vector3(nx, ny, nz);
			newNormal->normalize();
			vertexNormalList.push_back(newNormal);

    } else if (strcmp(str, "vt") == 0) {
      
      v = fscanf(stream, "%lf", &tx);
      v = fscanf(stream, "%lf", &ty);
			Vector2 * newTexture = new Vector2(tx, ty);
			vertexTextureList.push_back(newTexture);

    }else if (strcmp(str, "v") == 0) {
      
      // Vertex
      // Read three doubles and add the vertex.

      v = fscanf(stream, "%lf", &x);
      v = fscanf(stream, "%lf", &y);
      v = fscanf(stream, "%lf", &z);
			MeshVertex *mv = new MeshVertex(Vector3(x, y, z));
      _mesh->addVertex(mv);
			//printf("x:%f,y:%f,z:%f\n",x,y,z);
    } else if (strcmp(str, "f") == 0) {
      
      // Face 
      // Read three vertex indices and add the triangle.

			char c;
      bool vts=false; 
      bool vns=false;
      i0=i1=i2=-1;
      it0=it1=it2=-1;
      in0=in1=in2=-1;

			v = fscanf(stream, "%d", &i0);
      v = fscanf(stream, "%c", &c);
      if( c=='/')
      {
        vts=true;
			  v = fscanf(stream, "%d", &it0); 
			  v = fscanf(stream, "%c", &c);
        if( c=='/' ){
          vns=true;
			    v = fscanf(stream, "%d", &in0);
        }
      }

			v = fscanf(stream, "%d", &i1);
			v = fscanf(stream, "%c", &c);
      if( c=='/')
      {
        vts=true;
			  v = fscanf(stream, "%d", &it1); 
			  v = fscanf(stream, "%c", &c);
        if( c=='/' ){
          vns=true;
			    v = fscanf(stream, "%d", &in1);
        }
      }

			v = fscanf(stream, "%d", &i2);
			v = fscanf(stream, "%c", &c);
      if( c=='/')
      {
        vts=true;
			  v = fscanf(stream, "%d", &it2); 
			  v = fscanf(stream, "%c", &c);
        if( c=='/' ){
          vns=true;
			    v = fscanf(stream, "%d", &in2);
        }
      }

      // Since the .obj file format is one-based, the indices start
      // at one rather than at zero.  So, we must subtract one from
      // each index.
      i0--; i1--; i2--;
	  it0--;it1--;it2--;
      in0--;in1--;in2--;
      
      //_triangles.push_back(Tri(i0, i1, i2));
			
    assert(i0>=0 && i1 >=0 && i2 >= 0); //checks if we have valid vertex indices
	  MeshTriangle *mt = new MeshTriangle(_mesh, _mesh->getVertex(i0),
												 _mesh->getVertex(i1),
												 _mesh->getVertex(i2));
	  if (vertexTextureList.size() > 0) {
      assert(it0>=0 && it1>=0 &&it2>=0); //checks if we have valid texture vertices
		  _mesh->getVertex(i0)->setTexture(vertexTextureList[it0]);
		  _mesh->getVertex(i1)->setTexture(vertexTextureList[it1]);
		  _mesh->getVertex(i2)->setTexture(vertexTextureList[it2]);
      _readTexture=true;
	  }
    if(vertexNormalList.size() > 0){
      assert(in0 >=0 && in1>=0 &&in2>=0);
		  _mesh->getVertex(i0)->setNormal(vertexNormalList[in0]);
		  _mesh->getVertex(i1)->setNormal(vertexNormalList[in1]);
		  _mesh->getVertex(i2)->setNormal(vertexNormalList[in2]);
      _readVertexNormals=true;
    }

	  _mesh->addTriangle(mt);
	  _scene->addElement(mt);
    v=1;


	  //printf("i0:%d,i1:%d,i2:%d\n",i0,i1,i2);

    } else {
      
      if (!unknownWarning) {
        printf("Unknown token \"%s\" encountered.  (Additional warnings surpressed.)\n", str);
        unknownWarning = true;
      }

      // Read until the next newline.
      while (1) {
        char c;
        v = fscanf(stream, "%c", &c);
        if ((c == '\n') || (v != 1))
          break;
      }
    }
    
    if (v != 1) {
      printf("Error: Premature end of file while reading \"%s\".", filename.c_str());
      return false;
    }
  }
  
  fclose(stream);
  return true;
	
}

void preprocessing(Mesh * _mesh, Vector3 rotate, Vector3 scale, Vector3 translate, 	std::vector<Vector3 * >& normals )
{
	// compute bounding box
		Vector3 minPosition;
		Vector3 maxPosition;
		Vector3 centerPosition;
		double radius=0.0;
		maxPosition[0]=std::numeric_limits<double>::min();
		maxPosition[1]=std::numeric_limits<double>::min();
		maxPosition[2]=std::numeric_limits<double>::min();

		minPosition[0]=std::numeric_limits<double>::max();
		minPosition[1]=std::numeric_limits<double>::max();
		minPosition[2]=std::numeric_limits<double>::max();

		for(unsigned int i=0;i<_mesh->numberOfVertices();i++)
		{
			Vector3 point = *(_mesh->getVertex(i)->getPosition());
			if(point[0] < minPosition[0])
			{
				minPosition[0] = point[0];
			}
			if(point[1] <minPosition[1])
			{
				minPosition[1] = point[1];
			}
			if(point[2] <minPosition[2])
			{
				minPosition[2] = point[2];
			}

			if(point[0] >maxPosition[0])
			{
				maxPosition[0] = point[0];
			}
			if(point[1] >maxPosition[1])
			{
				maxPosition[1] = point[1];
			}
			if(point[2] >maxPosition[2])
			{
				maxPosition[2] = point[2];
			}

		}
	// compute center position
	centerPosition[0] = (maxPosition[0] + minPosition[0])/2.0;
	centerPosition[1] = (maxPosition[1] + minPosition[1])/2.0;
	centerPosition[2] = (maxPosition[2] + minPosition[2])/2.0;

	// compute radius
	//radius =	(maxPosition[0]-centerPosition[0])*(maxPosition[0]-centerPosition[0]) +
	//			(maxPosition[1]-centerPosition[1])*(maxPosition[1]-centerPosition[1])	+
	//			(maxPosition[2]-centerPosition[2])*(maxPosition[2]-centerPosition[2]);

	//bounding box diagonal length:
	double bbdl = (maxPosition-minPosition).length();

	// normalize size and translate to origin
	for (unsigned int i=0; i < _mesh->numberOfVertices(); i++) {
		 Vector3 p = *(_mesh->getVertex(i)->getPosition());
			p = (p-centerPosition)/bbdl;
			
		 _mesh->getVertex(i)->setPosition(p);
	}

	// transform mesh corresponding to rotate, translate and scale
	Matrix4 s;
	s.loadScaling(scale);
	Matrix4 rx;
	rx.loadRotation(Vector3(1.,0.,0.),(M_PI/180.)*rotate.x);
	Matrix4 ry;
	ry.loadRotation(Vector3(0.,1.,0.),(M_PI/180.)*rotate.y);
	Matrix4 rz;
	rz.loadRotation(Vector3(0.,0.,1.),(M_PI/180.)*rotate.z);
	Matrix4 Tf;
	Tf.loadIdentity();
	//Tf *= s*rx*ry*rz*t;

	Tf = s*rx*ry*rz;
	Matrix4 invTft = Tf.Inverse().Transpose();

	//Transform points and normals
	for (unsigned int i=0; i < _mesh->numberOfVertices(); i++) {
		Vector3 p = *(_mesh->getVertex(i)->getPosition());
		p = Tf*p + translate;
		_mesh->getVertex(i)->setPosition(p);
	}

	for( std::vector<Vector3*>::iterator it=normals.begin(); it!=normals.end(); ++it)
	{
		Vector3* n = (*it);
		Vector4 n4 = invTft*Vector4(*n,0.);
		*n = Vector3(n4.x,n4.y,n4.z);
		n->normalize();
		//_mesh->getVertex(i)->setNormal(n);
	}
		
}

void calcVertexNormals(Mesh * _mesh, std::vector<Vector3*>& vertexNormalList )
{
	// Calculate per-vertex normals.
  
	//std::vector<Vector3> vertexNormalList;
	vertexNormalList.clear();
	vertexNormalList.resize(_mesh->numberOfVertices());

	//Allocate memory for normals
	for (unsigned int i = 0; i < _mesh->numberOfVertices(); i++) {
		Vector3* ni = new Vector3(0.,0.,0.);
		vertexNormalList[i] = ni; 
	}

	// Iterate over all triangles.
	for (unsigned int i = 0; i < _mesh->numberOfFaces(); i++) {

    MeshTriangle * triangle = _mesh->getFace(i);

		// For each triangle, calculate it's face normal.
		Vector3 point1 = *(triangle->getVertex(0)->getPosition());
		Vector3 point2 = *(triangle->getVertex(1)->getPosition());
		Vector3 point3 = *(triangle->getVertex(2)->getPosition());
		Vector3 vec1 = point2 - point1;
		Vector3 vec2 = point3 - point1;
		Vector3 faceNormal = vec1.cross(vec2);
		//printf("%f, %f, %f\n",vec1[0],vec1[1],vec1[2]);
	    
		// Add this face normal to the normal vector for each of the
		// triangle's three indices.	
		(*vertexNormalList[triangle->getVertex(0)->getIndex()]) += faceNormal;
		(*vertexNormalList[triangle->getVertex(1)->getIndex()]) += faceNormal;
		(*vertexNormalList[triangle->getVertex(2)->getIndex()]) += faceNormal;	
	}

	// Finally, normalize all of the normals.
	for (unsigned int i = 0; i < _mesh->numberOfVertices(); i++)
	{
		vertexNormalList[i]->normalize();
		_mesh->getVertex(i)->setNormal(vertexNormalList[i]);
	}	

}