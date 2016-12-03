//This class is to organize the stuff of player
//Contains Deck, Hand Stack, Garbage and Field

#pragma once

#include <windows.h>

#include "Deck.h"
#include "Hand.h"
#include "Garbage.h"
#include "Field.h"

class Player
{
public:
	Player();
	//initialize
	void Init(bool isP1);
	//set deck
	void SetDeck(Deck &d);
	//pop from deck
	Card PopDeck(int n);
	//set hand
	void SetHand(Hand &h);
	void PushHand(Card &c);
	void PushGarbage(Card &c);
	//preset position vector
	osg::Vec3 v3t_handCount;
	//UI texts
	osg::ref_ptr<osgText::Text> t_handCount;
	osg::ref_ptr<osgText::Text> t_gamePoints;

	//card stack objects
	Deck deck;
	Hand hand;
	Garbage garbage;
	Field field;
	//is the player gave up or not
	bool isGiveup;
	//number of rounds that are lost
	int loss;
	//amount of cards in hand
	int handCount;
};