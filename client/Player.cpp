#include "Player.h"

Player::Player()
{
	deck = Deck();
	hand = Hand();
	garbage = Garbage();
	field = Field();

	isGiveup = false;
	loss = 0;
	handCount = 0;
}

void Player::Init(bool isP1)
{
	/*
	if (isP1)
	{
		v3t_handCount = AssistantDirector::v3HandCount;
	}
	t_handCount = AssistantDirector::createText(v3t_handCount,AssistantDirector::intToString(handCount), 3.0);
	*/
	
}

void Player::SetDeck(Deck &d)
{
	deck = d;
}

Card Player::PopDeck(int n)
{
	if (n < deck.stack.size())
	{
		return deck.PopCard(n);
	}
	else
	{
		std::cout<<"Player.PopDeck Error!"<<std::endl;
		return Card();
	}
}

void Player::SetHand(Hand &h)
{
	hand = h;
	/*
	handCount = hand.stack.size();
	t_handCount->setText(AssistantDirector::intToString(handCount));
	*/
}

void Player::PushHand(Card &c)
{
	c.v3Return = osg::Vec3(AssistantDirector::v3Hand.x() + hand.stack.size() * Card::x,
										AssistantDirector::v3Hand.y(),
										AssistantDirector::v3Hand.z());
	c.GeneratePATransform(c.v3Return);
	
	hand.PushCard(c);
	/*
	handCount = hand.stack.size();
	t_handCount->setText(AssistantDirector::intToString(handCount));
	*/
}

void Player::PushGarbage(Card &c)
{
	garbage.PushCard(c);
}
