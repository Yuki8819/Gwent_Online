//Deck class, organize all the cards in player's deck, basically as same as the CardStack class

#pragma once

#include "CardStack.h"

class Deck:public CardStack
{
public:
	Deck():CardStack(){};
};