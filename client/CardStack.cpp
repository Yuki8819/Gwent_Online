#include "CardStack.h"

CardStack::CardStack()
{
	stack = std::deque<Card>();
}

void CardStack::PushCard(Card c)
{
	stack.push_back(c);
}

Card CardStack::PopCard(int n)
{
	Card temp_card = Card();
	if (n >= 0 && n < stack.size())
	{
		temp_card = stack[n];
		stack.erase(stack.begin() + n);
	}
	return temp_card;
}