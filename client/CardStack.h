//This class is to organize the cards in the form of queue
#pragma once

#include "Card.h"

#include <vector>
#include <deque>



class CardStack
{
public:
	//constructor
	CardStack();
	//push one card object into the stack
	void PushCard(Card c);
	//pop one card object out of the stack, by number in the queue
	Card PopCard(int n);

	//the stack
	std::deque<Card> stack;
};