#include "Garbage.h"

//+1 overridden
void Garbage::PushCard(Card &c)
{
	
	c.isPickerActive = false;
	if (stack.empty())
	{
		c.v3Return = osg::Vec3(v3Garbage.x(),v3Garbage.y() - 10 * Card::y,v3Garbage.z());		
	}
	else
	{
		c.v3Return = v3Garbage + osg::Vec3(0.0, -9 * Card::y,0.0);
	}

	c.direction = c.v3Return;
	if (c.genre != 3)
	{
		c.animator = 5;
	}
	else
	{
		c.animator = 6;
	}
	
	
	c.material->setEmission(osg::Material::FRONT, osg::Vec4(0.0,0.0,0.0,1.0));
	stack.push_back(c);
	
}

//+1 overridden
Card Garbage::PopCard(int n)
{
	
	Card temp_card = Card();
	if (n >= 0 && n < stack.size())
	{
		temp_card = stack[n];
		stack.erase(stack.begin() + n);
	}

	//recalculate garbage cards position values
	if (!stack.empty())
	{
		stack[0].material->setEmission(osg::Material::FRONT, osg::Vec4(0.0,0.0,0.0,1.0));
		stack[0].v3Return = osg::Vec3(v3Garbage.x(),
									  v3Garbage.y() - 10 * Card::y,
									  v3Garbage.z());
		stack[0].pat->setPosition(stack[0].v3Return);
		for (int i = 1; i < stack.size(); i++)
		{
			stack[i].material->setEmission(osg::Material::FRONT, osg::Vec4(0.0,0.0,0.0,1.0));
			
			stack[i].v3Return = v3Garbage + osg::Vec3(0.0, -9 * Card::y,0.0);

			stack[i].pat->setPosition(stack[i].v3Return);
		}
	}
	return temp_card;
}

void Garbage::ShowAll()
{
	for (int i = 0; i < stack.size(); i++)
	{
		stack[i].material->setEmission(osg::Material::FRONT, osg::Vec4(0.75,0.75,0.75,1.0));
		stack[i].direction = (osg::Vec3(AssistantDirector::v3GarbageFocused.x() + (Card::x + 0.1) * (i % 10),
											AssistantDirector::v3GarbageFocused.y(),
											AssistantDirector::v3GarbageFocused.z() - (Card::z + 0.1) * (i / 10)));
		stack[i].animator = 5;
	}
}