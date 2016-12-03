//This class is to manage static values and functions for global uses

#pragma once

#include<winsock2.h>
#include<stdio.h>
#include <string>

#include <windows.h>
#include <string>
#include <sstream>
#include <iostream>

#include <osgAnimation/Sampler>
#include <osgUtil/SmoothingVisitor>
#include <osg/CullFace>
#include <osg/Material>
#include <osg/MatrixTransform> 
#include <osg/LightSource> 
#include <osg/Camera>
#include <osg/Shape>  
#include <osg/ShapeDrawable>  
#include <osg/Geode>  
#include <osgViewer/Viewer>
#include <osgText/Font>
#include <osgText/Text> 
#include <osg/PositionAttitudeTransform>

#include <osgFX/BumpMapping>
#include <osgFX/Registry>

#pragma comment (lib,"ws2_32.lib")

//Macro
#define		SERVPORT		5050		//port number
#define		MAX				100
#define		MAXPLAYER		4
#define		MAXROOM			MAXPLAYER/2
#define		ROOMFULL		2
#define		AVAILABLE		1
#define		OCCUPIED		0
#define		SEATONE			0
#define		SEATTWO			1
#define		SEATTHREE		2
#define		SEATFOUR		3
#define		ROOMONE			0
#define		ROOMTOW			1
#define		ROOMFULL		2
#define		NOROOM			-1
#define		CONNECTSUCCESS	0X1
#define		SERVERFULL		0X2
#define		SERVERMSG		0X3
#define		ROOMSELECT		0X4
#define		RESELECT		0X5
#define		WAITSTART		0X6
#define		WAITANOTHER		0X7
#define		READY			"ready"
#define		LATER			"later"
#define		GIVEUP			"giveup"
#define		END				0X10
#define		SERVIP      "127.0.0.1"      //Server IP: localhost	


class AssistantDirector
{
public:
	AssistantDirector();
	//static functions
	//generate card ID
	static int GenerateID();
	//create light
	static osg::PositionAttitudeTransform* createLightSource(unsigned int num,const osg::Vec3& trans,const osg::Vec4& color);
	//create box object by position vector and length, width, height
	static osg::ref_ptr<osg::Geode> createBox(osg::Vec3 center, float x, float y, float z);
	//create text
	static osg::ref_ptr<osgText::Text> createText(const osg::Vec3& pos,const std::string& content,float size);
	//convert int to string
	static std::string intToString (int n);
	//send message, by string and by char*
	static bool Send_msg(std::string str);
	static bool Send_msg(char* str);
	//receive message, by string and by char*
	static void Recv_msg(std::string &str);
	static void Recv_msg(char* str);
	//set socket
	static void setSocket(SOCKET s);
	//convert string to char*
	static char* stringToChar(std::string str);

	//static values
	//ID of current card
	static int currentID;
	//preset position vectors (world)
	static osg::Vec3 v3Hand;
	static osg::Vec3 v3Melee;
	static osg::Vec3 v3Archer;
	static osg::Vec3 v3Siege; 
	static osg::Vec3 v3Score;
	static osg::Vec3 v3Melee_2;
	static osg::Vec3 v3Archer_2;
	static osg::Vec3 v3Siege_2; 
	static osg::Vec3 v3Score_2;
	static osg::Vec3 v3Weather;
	static osg::Vec3 v3Focused;
	static osg::Vec3 v3Garbaged;
	static osg::Vec3 v3Garbaged_2;
	static osg::Vec3 v3GarbageFocused;
	static osg::Vec3 v3HandCount;
	static osg::Vec3 v3HandCount_2;
	static osg::Vec3 v3RowLenth;
	static float hornX;
	static float xmin;
	static float xmax;
	static float y[4];
	static float row_text_x;
	static float row_text_z;
	//socket 
	static SOCKET sockets;
	//array of the whole card deck
	static int cards_of_deck[37];
	//amount of cards in deck
	static int deck_amount;
	//buff value of horn card
	static int hornBuf;
	//shadow receive mask
	static int ReceivesShadowTraversalMask;
	//shadow cast mask
	static int CastsShadowTraversalMask;
};