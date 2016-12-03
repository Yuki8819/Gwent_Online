#pragma once

#include "CardStack.h"

class Hand:public CardStack
{
public:
	Hand():CardStack(){};

	//push +1 overridden
	void PushCard(Card &c);
	//pop +1 overridden
	Card PopCard(int n);
};