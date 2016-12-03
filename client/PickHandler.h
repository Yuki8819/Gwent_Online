//This class is to handle user inputs

#pragma once
#include <windows.h>

//#include "Card.h"
#include "DataReceiverThread.h"
#include "Director.h"

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgFX/Scribe>

#include <osgGA/GUIEventHandler>

#include <osgUtil/Optimizer>

#include <osg/MatrixTransform>
#include <osg/PolygonMode>
#include <osg/ShapeDrawable>
#include <osgUtil/LineSegmentIntersector>

#include <iostream>

class PickHandler : public osgGA::GUIEventHandler 
{
	
public: 
	PickHandler(Director* d);
	~PickHandler();
	//handler
	virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
protected:    
	//director node
	Director* director;
};