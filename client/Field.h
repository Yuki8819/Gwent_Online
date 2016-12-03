//This class is to organize the gaming field, aka the desk, for one player
//The field contains a hand card stack, three unit card stack and the horn card stacks for them (melee, archer, siege), a weather card stack and a garbage stack
//It also contains several UI objects

#pragma once

#include "CardStack.h"

class Field
{
public:
	//scores
	int score;
	int score_melee;
	int score_archer;
	int score_siege;
	//stacks
	CardStack melee;
	CardStack archer;
	CardStack siege;
	static CardStack weather;	//两个Field共用一个weather队列
	CardStack meleeHorn;
	CardStack archerHorn;
	CardStack siegeHorn;
	//weather status
	static bool isRain;
	static bool isFog;
	static bool isFrost;
	//preset positions
	osg::Vec3 v3Melee;
	osg::Vec3 v3Archer;
	osg::Vec3 v3Siege;
	static osg::Vec3 v3Weather;
	osg::Vec3 v3Score;
	//erase weather effects
	static void EraseWeather();
	//UI texts
	osg::ref_ptr<osgText::Text> t_score_melee;
	osg::ref_ptr<osgText::Text> t_score_archer;
	osg::ref_ptr<osgText::Text> t_score_siege;
	osg::ref_ptr<osgText::Text> t_score;


	Field();
	//intialize
	void Init(bool isP1);
	//push and pop functions
	void PushMelee(Card &card);
	void PushArcher(Card &card);
	void PushSiege(Card &card);
	Card PopMelee(int n);
	Card PopArcher(int n);
	Card PopSiege(int n);
	static void PushWeather(Card &card);
	//score calculator
	int CalculateScore();
};
