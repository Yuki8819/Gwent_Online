//This class takes charge of controlling most of the stuff in game
//There will be only one Director object in game
//It contains the whole scene and it is the "root" of the game

#pragma once

#include "Player.h"

#include <windows.h>
#include <stdlib.h> 
#include <time.h> 

#include <osgUtil/Optimizer>
#include <osg/Camera>
#include <osg/Shape>  
#include <osg/ShapeDrawable>  
#include <osg/Geode>  
#include <osgViewer/Viewer>

#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowVolume>
#include <osgShadow/ShadowTexture>
#include <osgShadow/ShadowMap>
#include <osgShadow/SoftShadowMap>
#include <osgShadow/ParallelSplitShadowMap>

class Director
{
public:
	//functions
	//constructor
	Director();
	//initailize
	void Init();
	//clean all cards in the scene
	void CleanAllCards();
	//initialize camera
	void Init_Camera();
	//generate deck
	Deck GenerateDeck(int nums[], int length);
	//deal cards, by card numbers
	Hand Deal(int nums[], int length);
	//deal one card
	Hand Deal(int num);
	//player (local) play one card
	void toField(Card card);
	//player (server, or the other player) play one card
	void toField(int num);
	//player one card (for the uses of doctor card)
	void toFieldFromCard(Card card);
	//burn function
	void Burn();
	//burn function for only one line
	void BurnLine(Player *player, int line);
	//clean all the cards on one player's field
	void CleanUp(Player *player);

	//properties

	//state machine
	//-3: IP enter interface
	//-2: room select interface
	//-1: waiting
	//0: playing cards
	//1: waiting for the other player to play cards
	int StateMachine;
	//player objects
	Player player_1;
	Player player_2;
	//node objects
	osg::ref_ptr<osg::Group> root;
	osg::ref_ptr<osg::PositionAttitudeTransform> light1;
	osg::ref_ptr<osg::Light> light1_obj;
	osg::ref_ptr<osg::PositionAttitudeTransform> light2;
	osg::ref_ptr<osg::Light> light2_obj;
	//camera node
	osg::ref_ptr<osg::Camera> camera;
	//UI nodes
	osg::ref_ptr<osg::Geode> UIGeode;
	osg::ref_ptr<osgText::Text> t_IP;
	osg::ref_ptr<osgText::Text> t_Login_Caution;
	osg::ref_ptr<osgText::Text> t_Gaming_Caution;
	osg::ref_ptr<osgText::Text> t_Room_1;
	osg::ref_ptr<osgText::Text> t_Room_2;
	//controllers
	//is the card the player playing a horn card
	bool isHorn;
	//is the card the player playing a scarecrow
	bool isScarecrow;
	//is the card the player playing a doctor
	bool isCuring;
	//the number (in hand card stack) of the card the player is playing
	int currentHandi;
	//if player_2 plays a horn, the line number it indicates
	int p2Horn;
	//if player_2 plays a scarecrow, the card number it indicates
	int p2ScarecrowNo;
	//if player_2 plays a scarecrow, the line number it indicates
	int p2ScarecrowLine;
	//server IP address
	std::string ip;
	//rooms' availability
	int	available[MAXROOM];
};