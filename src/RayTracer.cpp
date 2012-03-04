/****************************************************************************
|*  RayTracer.cpp
|*
|*  Main file of the raytracer.  
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/

#include <iostream>

#ifdef _WIN32
	#include <GL/glut.h>
	#include "Windows.h"
#endif
#ifdef __APPLE__
	#include <GLUT/glut.h>
	#include <sys/time.h>
#elif __linux__
	#include <GL/glut.h>
#endif

#include <Renderer.h>
#include <Scene.h>
#include <parser/SceneParser.h>
#include <parser/ConfigParser.h>
#include <exporter/Exporter.h>
#include <utils/Point.h>
#include <utils/Matrix4.h>


// Globals
Scene* scene;
SceneParser sceneParser;
ConfigParser configParser;
Renderer* renderer;
Exporter* exporter;
int lastStatusPercentage;
int minStatusStepSize;
char * sceneDescription;

enum Menu {
	MENU_RELOAD_SCENE,
	MENU_RELOAD_RENDERER,
	MENU_START_RENDERER,
	MENU_EXPORT_IMAGE,
	MENU_EXIT,
	MENU_EMPTY
};

// Functions
// glut display callback function
void displayFunction(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	if (scene) {
		Point p = scene->getCamera()->getResolution();
		glViewport(0,0,p.x, p.y);

		// Write camera film into frame buffer
		glRasterPos2i(-1,-1);
		glDrawPixels(
			p.x,
			p.y,
			GL_RGB,
			GL_FLOAT,
			scene->getCamera()->getFilm());
	}
	glutSwapBuffers();
}

unsigned long getTime(){
	unsigned long ret = 0;
	#ifdef _WIN32
	LARGE_INTEGER liTimerFrequency;
	QueryPerformanceFrequency(&liTimerFrequency);
	LARGE_INTEGER liLastTime;
	QueryPerformanceCounter(&liLastTime);
	ret = (INT)( ((double)liLastTime.QuadPart /
	liTimerFrequency.QuadPart)*1000 ); 
	#else
	struct timeval tv;
	struct timezone tz;
	tz.tz_minuteswest = 0;
	tz.tz_dsttime = 0;
	gettimeofday(&tv,&tz);
	ret = (tv.tv_sec*1000)+(tv.tv_usec/1000);
	#endif
	return (unsigned long)ret;
}

// update function that gets registered for renderer updates
void update(void){
	int status = (int) (renderer->status()*100);
	if (status - lastStatusPercentage >= minStatusStepSize) {
		lastStatusPercentage = status;
		//std::cout << "[" << status << "%" << "]";
	}
  displayFunction();
}

// glut idle callback function
void idleFunction(void) {
}

// glut key callback function
void keyFunction(unsigned char key, int x, int y) {
	switch(key) {
		case 27: // esc
			exit(0);
			break;
	}	
}

// init glut window
void initWindow(void) {

	if (scene) {
		Point p = scene->getCamera()->getResolution();
		glutInitWindowSize(p.x, p.y);
	}
	else {
		glutInitWindowSize(200, 200);
	}

#ifdef _WIN32
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glutCreateWindow("Raytracer");
#else
	glutCreateWindow("Raytracer");
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glClearColor(0.0, 0.0, 0.0, 1.0);
#endif

}

// glut menu callback function
void menuFunction(int value) {
	if (value == MENU_START_RENDERER) {
		//start renderer
		if (scene && renderer) {
			// Render
			minStatusStepSize = 10;
			lastStatusPercentage = -minStatusStepSize;

			std::cout << "Renderer: rendering.... ";
			unsigned long startTime = getTime();
			renderer->render(scene);
			unsigned long stopTime = getTime();
			std::cout << "[done]\n\n";
			std::cout << "Total rendering time: " << (stopTime - startTime)/1000.0 << " sec \n";
			if(scene->useKDTree())
			{
				std::cout << "Total number of intersection tests: " << scene->numOfIntersectionTests() << "\n\n";
			}
			displayFunction();
		}
		else {
			std::cerr << "Rendering failed!\n\n";
		}
	}
	else if (value == MENU_EXPORT_IMAGE) {
		//export buffer to image
		Point p = scene->getCamera()->getResolution();
		if (!exporter->exportImage(scene->getCamera()->getFilm(), p.x, p.y)) {
			std::cerr << "Exporting to image failed!\n\n";
		}
		else {
			std::cout << "Image file exported.\n\n";
		}
	}
	else if (value == MENU_EXIT) {
		//exit program
		exit(0);
	}
	else if (value == MENU_RELOAD_SCENE) {
		// reparse scnene description
		delete scene;
		scene = sceneParser.parse(sceneDescription);	
		if (!scene) {
			std::cerr << "Reload scene failed!\n\n";
		}
		else {

			if (scene->useKDTree()){
				scene->buildKDTree();
			}

			// resize window
			Point p = scene->getCamera()->getResolution();
			glutReshapeWindow(p.x, p.y);
			std::cout << "\nRayTracer ready! \n-> Right click to enter menu.\n\n";
		}
	}
	else if (value == MENU_RELOAD_RENDERER) {
		// reload Renderer
		delete renderer;
		std::cout << "Reload Renderer... ";
		renderer = configParser.parse("data/Config.xml");
		if (!renderer) {
			std::cerr << "Reload renderer failed!\n\n";
		}
		else {
			renderer->addObserver(&update);
			std::cout << "\nRayTracer ready! \n-> Right click to enter menu.\n\n";
		}
	}
}

// RayTracer main function
int main(int argc, char *argv[]) {
	
	// Init Renderer
	renderer = configParser.parse("data/Config.xml");
	if (renderer) {
		// Add observer to renderer
		renderer->addObserver(&update);
	}

	// Init Scene
	if (argc <= 1) {
		sceneDescription = "data/scenes/Default/scene.xml";
	}
	else {
		sceneDescription = argv[1];
	}
	scene = sceneParser.parse(sceneDescription);

	// build kd Tree	
	if (scene != NULL && scene->useKDTree()){
		scene->buildKDTree();
	}

	// Init Exporter
	exporter = new Exporter();

	// Initialize glut window
	glutInit(&argc, argv);
	initWindow();

	// add glut callback functions
	glutDisplayFunc(displayFunction);
	glutKeyboardFunc(keyFunction);
//	glutIdleFunc(idleFunction);

	// add glut menu
	glutCreateMenu(menuFunction);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutAddMenuEntry("Render Image", MENU_START_RENDERER);
	glutAddMenuEntry("----------------------", MENU_EMPTY);
	glutAddMenuEntry("Reload Renderer Config", MENU_RELOAD_RENDERER);
	glutAddMenuEntry("Reload Scene", MENU_RELOAD_SCENE);
	glutAddMenuEntry("----------------------", MENU_EMPTY);
	glutAddMenuEntry("Export image...", MENU_EXPORT_IMAGE);
	glutAddMenuEntry("Exit", MENU_EXIT);

	// display status on console
	if (scene && renderer) {
		std::cout << "\nRayTracer ready! \n-> Right click to enter menu.\n\n";
	}
	else {
		std::cout << "\nRayTracer not ready!\n";
		if (!renderer) {
			std::cout << "-> Reading renderer config file failed!\n";
		}
		if (!scene) {
			std::cout << "-> Reading scene file failed!\n";
		}
		std::cout << "-> Right click to enter menu.\n\n";
	}
	
	// enter glut event loop
	glutMainLoop();

	// clean up
	delete(scene);
	delete(renderer);
	delete(exporter);
	
	// quit
	return 0;
}
