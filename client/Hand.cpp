#include "Hand.h"

//+1 overridden
void Hand::PushCard(Card &c)
{
	c.v3Return = osg::Vec3(AssistantDirector::v3Hand.x() + stack.size() * Card::x,
										AssistantDirector::v3Hand.y(),
										AssistantDirector::v3Hand.z());
	c.isPickerActive = true;
	c.pat->setPosition(osg::Vec3(AssistantDirector::v3Hand.x() + stack.size() * Card::x,
										AssistantDirector::v3Hand.y(),
										AssistantDirector::v3Hand.z()));
	c.material->setEmission(osg::Material::FRONT, osg::Vec4(0.0,0.0,0.0,1.0));
	stack.push_back(c);
}

//+1 overridden
Card Hand::PopCard(int n)
{
	Card temp_card = Card();
	if (n >= 0 && n < stack.size())
	{
		temp_card = stack[n];
		stack.erase(stack.begin() + n);
	}
	for (int i = 0; i < stack.size(); i++)
	{
		{
			stack[i].v3Return = osg::Vec3(AssistantDirector::v3Hand.x() + i * Card::x,
									   	  AssistantDirector::v3Hand.y(),
										  AssistantDirector::v3Hand.z());
		}
		stack[i].pat->setPosition(stack[i].v3Return);
	}
	return temp_card;
}