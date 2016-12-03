#include <windows.h>

#include "PickHandler.h"
#include "Director.h"
#include "P2Callback.h"

#include "Card.h"

#include <osg/Camera>
#include <osg/Group>  
#include <osgDB/readfile>  
#include <osgViewer/Viewer>  
#include <osg/TexEnv>  
#include <osg/Texture2D>  
#include <osg/PositionAttitudeTransform>  
#include <iostream>  


int main(int argc, char** argv)
{
	//display the studio logo (LKAA)
    osg::ref_ptr<osg::Node> node = new osg::Node();
    node = osgDB::readNodeFile("Gwent\\LKAA_Mark.osgb");
	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
	osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform;
	node->setDataVariance(osg::Object::DYNAMIC);
	pat->addChild(node.get());
	pat->setPosition(osg::Vec3(73.0,10.0,-48.5));
	pat->setDataVariance(osg::Object::DYNAMIC);



	//create director
	Director director = Director();
	//generate deck
	director.GenerateDeck(AssistantDirector::cards_of_deck, AssistantDirector::deck_amount);
	//deal
	for (int i = 0; i < 10; i++)
	{
		srand((unsigned)time(NULL)); 
		int n = director.player_1.deck.stack.size();
		int k = (rand() % (n-0))+ 0;
		director.Deal(k);
	}
	//callback functions
	osg::ref_ptr<PickHandler> picker = new PickHandler(&director);
	osg::ref_ptr<P2Callback> p2callback = new P2Callback(&director);

	//add camera, callback functions and other stuff
	director.root->addChild(director.camera.get());
	director.root->addUpdateCallback(p2callback);
	director.camera->addChild(pat.get());
	director.root->getOrCreateStateSet()->setMode( GL_LIGHT0,  osg::StateAttribute::ON );
	director.root->addChild( director.light1 );
	director.root->getOrCreateStateSet()->setMode( GL_LIGHT1,  osg::StateAttribute::ON );
	director.root->addChild( director.light2 );

    osgUtil::Optimizer optimizer ;
    optimizer.optimize(director.root.get()) ;

	osgViewer::Viewer viewer;
	viewer.setSceneData(director.root.get());
	viewer.addEventHandler(picker.get());
	viewer.setUpViewInWindow(10,10,1280,720);

	viewer.realize();
	viewer.run();
	return 0;
}