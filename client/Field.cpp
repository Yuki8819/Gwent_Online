#include "Field.h"

osg::Vec3 Field::v3Weather = AssistantDirector::v3Weather;
CardStack Field::weather = CardStack();

bool Field::isRain = false;
bool Field::isFog = false;
bool Field::isFrost = false;

Field::Field()
{
	//Field::Init();
}
void Field::Init(bool isP1)
{
	if (isP1)
	{
		v3Melee = AssistantDirector::v3Melee;
		v3Archer = AssistantDirector::v3Archer;
		v3Siege = AssistantDirector::v3Siege;
		v3Score = AssistantDirector::v3Score;
	}
	else
	{
		v3Melee = AssistantDirector::v3Melee_2;
		v3Archer = AssistantDirector::v3Archer_2;
		v3Siege = AssistantDirector::v3Siege_2;
		v3Score = AssistantDirector::v3Score_2;
	}

	std::stringstream sstream;
    std::string str;

	score = score_melee = score_archer = score_siege = 0;
	
    sstream<<score;
    sstream>>str;

	t_score_melee = AssistantDirector::createText(osg::Vec3(v3Melee.x() + AssistantDirector::row_text_x,
															v3Melee.y(),v3Melee.z() + AssistantDirector::row_text_z),str, 3.0);
	t_score_melee->setBackdropType(osgText::Text::BackdropType::OUTLINE);
	t_score_melee->setBackdropColor(osg::Vec4(0.0,0.0,0.0,1.0));

	t_score_archer = AssistantDirector::createText(osg::Vec3(v3Archer.x() + AssistantDirector::row_text_x,
															v3Archer.y(),v3Archer.z() + AssistantDirector::row_text_z),str, 3.0);
	t_score_archer->setBackdropType(osgText::Text::BackdropType::OUTLINE);
	t_score_archer->setBackdropColor(osg::Vec4(0.0,0.0,0.0,1.0));

	t_score_siege = AssistantDirector::createText(osg::Vec3(v3Siege.x() + AssistantDirector::row_text_x,
															v3Siege.y(),v3Siege.z() + AssistantDirector::row_text_z),str, 3.0);
	t_score_siege->setBackdropType(osgText::Text::BackdropType::OUTLINE);
	t_score_siege->setBackdropColor(osg::Vec4(0.0,0.0,0.0,1.0));

	t_score = AssistantDirector::createText(osg::Vec3(v3Score.x() - 2.0,v3Score.y(),v3Score.z() - 1.2),str,3.0);
	t_score->setBackdropType(osgText::Text::BackdropType::OUTLINE);
	t_score->setBackdropColor(osg::Vec4(0.0,0.0,0.0,1.0));

	melee = CardStack();
	archer = CardStack();
	siege = CardStack();
}
void Field::PushMelee(Card &card)
{
	card.material->setEmission(osg::Material::FRONT, osg::Vec4(0.75,0.75,0.75,1.0));
	melee.PushCard(card);
	//score += card.atk;
	int last = melee.stack.size() - 1;
	melee.stack[last].v3Return
		= osg::Vec3(v3Melee.x() + (last) * Card::x,v3Melee.y(),v3Melee.z());
	//melee.stack[last].pat->setPosition(melee.stack[last].v3Return);
	melee.stack[last].direction = melee.stack[last].v3Return;
	melee.stack[last].animator = 4;
}
void Field::PushArcher(Card &card)
{
	card.material->setEmission(osg::Material::FRONT, osg::Vec4(0.75,0.75,0.75,1.0));
	archer.PushCard(card);
	//score += card.atk;
	int last = archer.stack.size() - 1;
	archer.stack[last].v3Return 
		= osg::Vec3(v3Archer.x() + (last) * Card::x,v3Archer.y(),v3Archer.z());
	//archer.stack[last].pat->setPosition(archer.stack[last].v3Return);
	archer.stack[last].direction = archer.stack[last].v3Return;
	archer.stack[last].animator = 4;
}
void Field::PushSiege(Card &card)
{
	card.material->setEmission(osg::Material::FRONT, osg::Vec4(0.75,0.75,0.75,1.0));
	siege.PushCard(card);
	//score += card.atk;
	int last = siege.stack.size() - 1;
	siege.stack[last].v3Return 
		= osg::Vec3(v3Siege.x() + (last) * Card::x,v3Siege.y(),v3Siege.z());
	//siege.stack[last].pat->setPosition(siege.stack[last].v3Return);
	siege.stack[last].direction = siege.stack[last].v3Return;
	siege.stack[last].animator = 4;
}
Card Field::PopMelee(int n)
{
	Card card = melee.PopCard(n);
	for (int i = 0; i < melee.stack.size(); i++)
	{
		melee.stack[i].v3Return = osg::Vec3(v3Melee.x() + i * Card::x,v3Melee.y(),v3Melee.z());
		melee.stack[i].pat->setPosition(melee.stack[i].v3Return);
	}
	return card;
}
Card Field::PopArcher(int n)
{
	Card card = archer.PopCard(n);
	for (int i = 0; i < archer.stack.size(); i++)
	{
		archer.stack[i].v3Return = osg::Vec3(v3Archer.x() + i * Card::x,v3Archer.y(),v3Archer.z());
		archer.stack[i].pat->setPosition(archer.stack[i].v3Return);
	}
	return card;
}
Card Field::PopSiege(int n)
{
	Card card = siege.PopCard(n);
	for (int i = 0; i < siege.stack.size(); i++)
	{
		siege.stack[i].v3Return = osg::Vec3(v3Siege.x() + i * Card::x,v3Siege.y(),v3Siege.z());
		siege.stack[i].pat->setPosition(siege.stack[i].v3Return);
	}
	return card;
}
void Field::PushWeather(Card &card)
{
	//into the weather queue
	card.material->setEmission(osg::Material::FRONT, osg::Vec4(0.75,0.75,0.75,1.0));
	switch (card.skill)
	{
		//don't need to care about sun card here
		/*
	case 0:
		break;
		*/
	case 1:
		if (isRain)
		{
			;
		}
		else
		{
			weather.PushCard(card);
			int last = weather.stack.size() - 1;
			weather.stack[last].v3Return 
				= osg::Vec3(v3Weather.x() + (last) * Card::x,v3Weather.y(),v3Weather.z());
			//weather.stack[last].pat->setPosition(weather.stack[last].v3Return);
			weather.stack[last].direction = weather.stack[last].v3Return;
			weather.stack[last].animator = 5;
			isRain = true;
		}
		break;
	case 2:
		if (isFog)
		{
			;
		}
		else
		{
			weather.PushCard(card);
			int last = weather.stack.size() - 1;
			weather.stack[last].v3Return 
				= osg::Vec3(v3Weather.x() + (last) * Card::x,v3Weather.y(),v3Weather.z());
			//weather.stack[last].pat->setPosition(weather.stack[last].v3Return);
			weather.stack[last].direction = weather.stack[last].v3Return;
			weather.stack[last].animator = 5;
			isFog = true;
		}
		break;
	case 3:
		if (isFrost)
		{
			;
		}
		else
		{
			weather.PushCard(card);
			int last = weather.stack.size() - 1;
			weather.stack[last].v3Return 
				= osg::Vec3(v3Weather.x() + (last) * Card::x,v3Weather.y(),v3Weather.z());
			//weather.stack[last].pat->setPosition(weather.stack[last].v3Return);
			weather.stack[last].direction = weather.stack[last].v3Return;
			weather.stack[last].animator = 5;
			isFrost = true;
		}
		break;
	default:
		break;
	}
	/*
	if(card.skill == 0)
	{
		EraseWeather();
	}
	else
	{
		int last = weather.stack.size() - 1;
		weather.stack[last].v3Return 
			= osg::Vec3(v3Weather.x() + (last) * Card::x,v3Weather.y(),v3Weather.z());
		weather.stack[last].pat->setPosition(weather.stack[last].v3Return);
	}
	*/
}
void Field::EraseWeather()
{
	weather.stack.clear();
}
int Field::CalculateScore()
{
	score = score_melee = score_archer = score_siege = 0;
	if (weather.stack.size() > 0)
	{
		for (int i = 0; i < weather.stack.size(); i++)
		{
			//weather effects
			//first, calculate the effect of weather cards
			//bad weathers: the indicated line -> atk_weathered = 1
			//sun: atk_weathered = atk_original
			//also calculates other buff or debuff effects
			if (isRain)
			{
				for (int j = 0; j < siege.stack.size(); j++)
				{
					siege.stack[j].atk_weathered = 1;
				}
			}
			else
			{
				for (int j = 0; j < siege.stack.size(); j++)
				{
					siege.stack[j].atk_weathered = siege.stack[j].atk_original;
				}
			}
			if (isFog)
			{
				for (int j = 0; j < archer.stack.size(); j++)
				{
					archer.stack[j].atk_weathered = 1;
				}
			}
			else
			{
				for (int j = 0; j < archer.stack.size(); j++)
				{
					archer.stack[j].atk_weathered = archer.stack[j].atk_original;
				}
			}
			if (isFrost)
			{
				for (int j = 0; j < melee.stack.size(); j++)
				{
					melee.stack[j].atk_weathered = 1;
				}
			}
			else
			{
				for (int j = 0; j < melee.stack.size(); j++)
				{
					melee.stack[j].atk_weathered = melee.stack[j].atk_original;
				}
			}
		}
	}
	else
	{
		for (int j = 0; j < siege.stack.size(); j++)
		{
			siege.stack[j].atk_weathered = siege.stack[j].atk_original;
		}
		for (int j = 0; j < archer.stack.size(); j++)
		{
			archer.stack[j].atk_weathered = archer.stack[j].atk_original;
		}
		for (int j = 0; j < melee.stack.size(); j++)
		{
			melee.stack[j].atk_weathered = melee.stack[j].atk_original;
		}
	}
	//calculation based on atk_weathered
	//Row of melee
	bool horn = false;
	int row_plus = 0;
	std::deque<Card*> bros;
	for (int i = 0; i < melee.stack.size(); i++)
	{
		melee.stack[i].atk = melee.stack[i].atk_weathered;
		Card card = melee.stack[i];
		if (card.skill == 3)
		{
			//unit horn card
			horn = true;
		}
		if (card.skill == 5)
		{
			//brotherhood
			bros.push_back(&melee.stack[i]);
		}
		if (card.skill == 6)
		{
			row_plus++;
		}
	}
	while (!bros.empty())
	{		
		int count = 1;
		int no = bros[0]->no;
		std::deque<int> position;
		position.push_back(0);
		for (int i = 1; i < bros.size(); i++)
		{
			if (bros[i]->no == no)
			{
				count++;
				position.push_back(i);
			}
		}
		//calculating actual atk points
		int atk_weathered = bros[position[count - 1]]->atk_weathered;
		for (int i = count - 1; i >= 0; i--)
		{
			bros[position[i]]->atk = count * atk_weathered;
			bros.erase(bros.begin() + position[i]);
		}
	}
	//horn card
	if (!meleeHorn.stack.empty())
	{
		horn = true;
	}
	//**************************************************sum**************************************************
	for (int i = 0; i < melee.stack.size(); i++)
	{
		if (melee.stack[i].skill == 6)
		{
			melee.stack[i].atk += (row_plus - 1);
		}
		else
		{
			melee.stack[i].atk += row_plus;
		}
		if (horn)
		{
			melee.stack[i].atk *= AssistantDirector::hornBuf;
		}
		if (melee.stack[i].isHero)
		{
			melee.stack[i].atk = melee.stack[i].atk_original;
		}
		score_melee += melee.stack[i].atk;
	}
	t_score_melee->setText(AssistantDirector::intToString(score_melee));
	//Row of archer
	horn = false;
	row_plus = 0;
	bros.clear();
	for (int i = 0; i < archer.stack.size(); i++)
	{
		archer.stack[i].atk = archer.stack[i].atk_weathered;
		Card card = archer.stack[i];
		if (card.skill == 3)
		{
			horn = true;
		}
		if (card.skill == 5)
		{
			bros.push_back(&archer.stack[i]);
		}
		if (card.skill == 6)
		{
			row_plus++;
		}
	}
	while (!bros.empty())
	{		
		int count = 1;
		int no = bros[0]->no;
		std::deque<int> position;
		position.push_back(0);
		for (int i = 1; i < bros.size(); i++)
		{
			if (bros[i]->no == no)
			{
				count++;
				position.push_back(i);
			}
		}
		int atk_weathered = bros[position[count - 1]]->atk_weathered;
		for (int i = count - 1; i >= 0; i--)
		{
			bros[position[i]]->atk = count * atk_weathered;
			bros.erase(bros.begin() + position[i]);
		}
	}
	if (!archerHorn.stack.empty())
	{
		horn = true;
	}
	//**************************************************sum**************************************************
	for (int i = 0; i < archer.stack.size(); i++)
	{
		if (archer.stack[i].skill == 6)
		{
			archer.stack[i].atk += (row_plus - 1);
		}
		else
		{
			archer.stack[i].atk += row_plus;
		}
		if (horn)
		{
			archer.stack[i].atk *= AssistantDirector::hornBuf;
		}
		if (archer.stack[i].isHero)
		{
			archer.stack[i].atk = archer.stack[i].atk_original;
		}
		score_archer += archer.stack[i].atk;
	}
	t_score_archer->setText(AssistantDirector::intToString(score_archer));
	//Row of siege
	horn = false;
	row_plus = 0;
	bros.clear();
	for (int i = 0; i < siege.stack.size(); i++)
	{
		siege.stack[i].atk = siege.stack[i].atk_weathered;
		Card card = siege.stack[i];
		if (card.skill == 3)
		{
			horn = true;
		}
		if (card.skill == 5)
		{
			bros.push_back(&siege.stack[i]);
		}
		if (card.skill == 6)
		{
			row_plus++;
		}
	}
	while (!bros.empty())
	{
		int count = 1;
		int no = bros[0]->no;
		std::deque<int> position;
		position.push_back(0);
		for (int i = 1; i < bros.size(); i++)
		{
			if (bros[i]->no == no)
			{
				count++;
				position.push_back(i);
			}
		}
		int atk_weathered = bros[position[count - 1]]->atk_weathered;
		for (int i = count - 1; i >= 0; i--)
		{
			bros[position[i]]->atk = count * atk_weathered;
			bros.erase(bros.begin() + position[i]);
		}
	}
	if (!siegeHorn.stack.empty())
	{
		horn = true;
	}
	//**************************************************sum**************************************************
	for (int i = 0; i < siege.stack.size(); i++)
	{
		if (siege.stack[i].skill == 6)
		{
			siege.stack[i].atk += (row_plus - 1);
		}
		else
		{
			siege.stack[i].atk += row_plus;
		}
		if (horn)
		{
			siege.stack[i].atk *= AssistantDirector::hornBuf;
		}
		if (siege.stack[i].isHero)
		{
			siege.stack[i].atk = siege.stack[i].atk_original;
		}
		score_siege += siege.stack[i].atk;
	}
	t_score_siege->setText(AssistantDirector::intToString(score_siege));

	score = score_melee + score_archer + score_siege;
	t_score->setText(AssistantDirector::intToString(score));
	return score;
}