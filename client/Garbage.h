#pragma once

#include "CardStack.h"

class Garbage:public CardStack
{
public:
	Garbage():CardStack(){};

	//push +1 overridden
	void PushCard(Card &c);
	//pop +1 overridden
	Card PopCard(int n);
	//display all the cards in garbage stack on screen
	void ShowAll();
	//preset position vector
	osg::Vec3 v3Garbage;
};