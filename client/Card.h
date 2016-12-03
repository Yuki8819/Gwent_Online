//This is the Card class
//Implements all the card objects

#pragma once

#include "AssistantDirector.h"

#include <windows.h>

#include <osg/Shape>  
#include <osg/ShapeDrawable>  
#include <osg/Geode>  
#include <osg/TexEnv>  
#include <osg/Texture2D>  
#include <osgDB/readfile>  
#include <osg/MatrixTransform>


#include <string>
#include <sstream>
#include <iostream>

class Card
{
public:
	//card info
	int id;				//card ID (indicates which card it is in the game)
	int no;				//card No (indicates which type of card it is)
	std::string tex_path;//file path of its texture
	int genre;			//card genre
	int skill;			//card skill
	int atk;			//the dynamic attack value of it
	int atk_weathered;	//the attack value of it after being affected by weather cards, = 1 or atk_original
	int atk_original;	//the original attack value of it, as presented on the card surface, should remain unchange
	int animator;		//status of current animator:
						//0.still;1.deal;2.zoom in;3.zoom out
						//4.play card (I);5.play card (II);6.discard
	osg::ref_ptr<osg::Material> material;	//the material of it
	bool isHero;		//is it a hero card or not
	//card status
	bool isSelected;	//is it selected or not
	bool isPickerActive;//can it be selected or not
	bool isFielded;		//is it on field or not
	bool isGarbaged;	//is it discarded or not
	//things related to its model
	osg::ref_ptr<osg::Box> boxShape;
	osg::ref_ptr<osg::ShapeDrawable> boxShapeDrawable;
	osg::ref_ptr<osg::Geode> boxGeode;
	//osg::ref_ptr <osg::MatrixTransform> mt;
	osg::ref_ptr<osg::PositionAttitudeTransform> pat;
	osg::Vec3 v3Return;	//position it should return to
	osg::Vec3 direction;//direction of current animation 
	static float x, y, z;//width, length, height
	//testure
	osg::ref_ptr<osg::Texture2D> boxTexture;
	osg::ref_ptr<osg::Image> boxImage;
	osg::ref_ptr<osg::TexEnv> boxTexEnv;
	osg::ref_ptr<osg::StateSet> boxStateSet;

	//functions
	//default constructor
	Card();
	//constructor, generate a card as "num" indicates
	Card(int num);
	//Initailization
	int Init(int num);
	//logic of creating the card
	int CreateLogic(int num);
	//visualize
	osg::ref_ptr<osg::PositionAttitudeTransform> GeneratePATransform(osg::Vec3 center = osg::Vec3(0,0,0));
	//set center position
	void SetCenter(osg::Vec3 &center);
};