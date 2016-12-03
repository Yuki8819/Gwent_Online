//This class is to handle the messages received from DataReceiverThread

#pragma once

#include "Director.h"
#include "DataReceiverThread.h"

#include <windows.h>

#include <osg/Geode>

class P2Callback : public osg::NodeCallback
{
public:
	//constructor
	P2Callback(Director *d);
	//process the messages received
	virtual void operator()(osg::Node* node,osg::NodeVisitor* nv);
	//director node
	Director* director;
	//animation step, aka speed
	float _step;
};
