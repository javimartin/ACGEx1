/****************************************************************************
|*  IFunctionObservable.h
|*
|*  Declaration of the class IFunctionObservable to implement an observer pattern through callback functions.
|*
|*
|*  Thomas Oskam, Michael Eigensatz, Hao Li, Bálint Miklós, Raphael Hoever - Applied Geometry Group ETH Zurich, Computer Vision Laboratory
|*  oskamt@student.ethz.ch, eigensatz@inf.ethz.ch, hli@inf.ethz.ch, balint@inf.ethz.ch, hoever@vision.ee.ethz.ch
\***********************************************************/


#ifndef _IFUNCTIONOBSERVABLE_H
#define _IFUNCTIONOBSERVABLE_H

#include <list>


class IFunctionObservable {

protected:

	//function type of update-functions that can be added to the observable
	typedef void (*UpdateFunction)(void);

	std::list<UpdateFunction> observers; //list of update function pointers


public:
	IFunctionObservable(){}
	virtual ~IFunctionObservable(){
		observers.clear();
	}

	void addObserver(UpdateFunction update) {
		observers.push_back(update);
	}

	void removeObserver(UpdateFunction update) {
		observers.remove(update);
	}

	//calls all the registered functions
	void notifyObservers(void) {
		std::list<UpdateFunction>::iterator it;
		for( it = observers.begin(); it != observers.end(); it++ ) {
			(*it)();
		}   
	}
	
};


#endif //_IFUNCTIONOBSERVABLE_H