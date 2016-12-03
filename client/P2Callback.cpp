#include "P2Callback.h"

P2Callback::P2Callback(Director *d)
{
	director = d;
	_step = 3.0;
}

void P2Callback::operator()(osg::Node* node,osg::NodeVisitor* nv)
{
	//set UI texts
	if (director->StateMachine == 1)
	{
		director->t_Gaming_Caution->setText("Waiting for Player2 to act");
	}
	else if (director->StateMachine == -1)
	{
		director->t_Gaming_Caution->setText("Waiting for Player2 to enter");
	}
	else
	{
		director->t_Gaming_Caution->setText("");
	}
	//decide the game is finished or not
	if (director->StateMachine >= 0)
	{
		if (director->player_1.isGiveup && director->player_2.isGiveup)
		{
			if (director->player_1.field.score < director->player_2.field.score)
			{
				//P1 loses
				director->player_1.loss++;
			}
			else if (director->player_1.field.score > director->player_2.field.score)
			{
				//P1 wins
				director->player_2.loss++;
				//P1 +1 Deal
				srand((unsigned)time(NULL)); 
				int n = director->player_1.deck.stack.size();
				int k = (rand() % (n-0))+ 0;
				director->Deal(k);
			}
			else
			{
				director->player_1.loss++;
				director->player_2.loss++;
			}
			director->player_1.t_gamePoints->setText(AssistantDirector::intToString(2 - director->player_1.loss));
			director->player_2.t_gamePoints->setText(AssistantDirector::intToString(2 - director->player_2.loss));
			if (director->player_1.loss >=  2 || director->player_2.loss >= 2)
			{
				//the whole match is set
				closesocket(AssistantDirector::sockets);
				DataReceiverThread::instance()->cleanSocket();
				WSACleanup();
				DataReceiverThread::instance()->cancel();
				if (director->player_1.loss < 2 && director->player_2.loss >= 2)
				{
					MessageBox(0,"You Won!","Game Set",MB_OK);
				}
				else if (director->player_1.loss >=  2 && director->player_2.loss < 2)
				{
					MessageBox(0,"You lost!","Game Set",MB_OK);
				}
				else if (director->player_1.loss >=  2 && director->player_2.loss >= 2)
				{
					MessageBox(0,"Draw...","Game Set",MB_OK);
				}
				//initialize the whole game again
				director->StateMachine = -3;
				director->camera->setViewMatrixAsLookAt(osg::Vec3(0.0,0.0,-216.0), osg::Vec3(0.0,1.0,-216.0), osg::Vec3(0.0,0.0,1.0));
				director->CleanAllCards();
				director->Init();
				director->GenerateDeck(AssistantDirector::cards_of_deck, AssistantDirector::deck_amount);
				//deal
				for (int i = 0; i < 10; i++)
				{
					srand((unsigned)time(NULL)); 
					int n = director->player_1.deck.stack.size();
					int k = (rand() % (n-0))+ 0;
					director->Deal(k);
				}
				return;
			}
			//initialize the game for another round
			director->player_1.isGiveup = false;
			director->player_2.isGiveup = false;
			director->StateMachine = -1;
			director->CleanUp(&director->player_1);
			director->CleanUp(&director->player_2);
		}
	}
	//network communication
	std::string receive = "";
	if (DataReceiverThread::instance()->getContent(receive))
	{
		if (receive == "end")
		{
			//the game is ended by the other player (he is offline)
			MessageBox(0,"Your rival quited.","Retired",MB_OK);
			//initialize the whole game
			director->StateMachine = -3;
			director->camera->setViewMatrixAsLookAt(osg::Vec3(0.0,0.0,-216.0), osg::Vec3(0.0,1.0,-216.0), osg::Vec3(0.0,0.0,1.0));
			director->CleanAllCards();
			director->Init();
			director->GenerateDeck(AssistantDirector::cards_of_deck, AssistantDirector::deck_amount);
			//deal
			for (int i = 0; i < 10; i++)
			{
				srand((unsigned)time(NULL)); 
				int n = director->player_1.deck.stack.size();
				int k = (rand() % (n-0))+ 0;
				director->Deal(k);
			}
		}
		if (receive == "ready")
		{
			//the server is ready to receive message
			director->StateMachine = 0;
		}
		else if (receive == "later")
		{
			//the server is not ready and waiting for the other player to act
			director->StateMachine = 1;
		}
		else if (receive.size() > 5 && receive[0] == 'h' && receive[1] == 'a')
		{
			//the info of the card that the other player plays
			std::string temp;
			for (int i = receive.size() - 1; i >= 5; i--)
			{
				temp.push_back(receive[i]);
			}
			std::reverse(temp.begin(), temp.end());
			std::stringstream sstream;
			int num;
			sstream<<temp;
			sstream>>num;
			director->player_2.handCount = num;
			director->player_2.t_handCount->setText(AssistantDirector::intToString(director->player_2.handCount));
		}
		else if (director->StateMachine == 1)
		{
			//messages that could be received when waiting
			if (receive == "giveup")
			{
				//P2 gives up
				director->player_2.isGiveup = true;
				if (!director->player_1.isGiveup)
				{
					director->StateMachine = 0;
				}
			}
			else if (receive.size() > 5 && receive[0] == '3' && receive[1] == '8')
			{
				//horn card
				director->p2Horn = receive[10] - 48;
				director->toField(38000);
			}
			else if (receive.size() > 5 && receive[0] == '3' && receive[1] == '9')
			{
				//scarecrow
				std::string temp;
				director->p2ScarecrowLine = receive[15] - 48;
				for (int i = receive.size() - 1; i >= 17; i--)
				{
					temp.push_back(receive[i]);
				}
				std::reverse(temp.begin(), temp.end());
				std::stringstream sstream;
				int num;
				sstream<<temp;
				sstream>>num;
				director->p2ScarecrowNo = num;
				director->toField(39000);
			}
			else
			{
				//unit cards
				std::stringstream sstream;
				int num;
				sstream<<receive;
				sstream>>num;
				//director->P2 play one card
				director->toField(num);
			}
		}
		
	}

	//********************************animation for hand stack********************************
	for (int i = 0; i < director->player_1.hand.stack.size(); i++)
	{
		switch (director->player_1.hand.stack[i].animator)
		{
		case 0://still
			break;
		case 1://deal
			break;
		case 2://zoom in
			{
				director->player_1.hand.stack[i].material->setEmission(osg::Material::FRONT, osg::Vec4(0.75,0.75,0.75,1.0));
				osg::Vec3 distance = AssistantDirector::v3Focused - director->player_1.hand.stack[i].pat->getPosition();
				if (distance.length() <= _step)
				{
					director->player_1.hand.stack[i].pat->setPosition(AssistantDirector::v3Focused);
					director->player_1.hand.stack[i].animator = 0;
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_1.hand.stack[i].pat->setPosition(director->player_1.hand.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		case 3://zoom out
			{
				director->player_1.hand.stack[i].material->setEmission(osg::Material::FRONT, osg::Vec4(0.0,0.0,0.0,1.0));
				osg::Vec3 distance = director->player_1.hand.stack[i].v3Return - director->player_1.hand.stack[i].pat->getPosition();

				if (distance.length() <= _step)
				{
					director->player_1.hand.stack[i].pat->setPosition(director->player_1.hand.stack[i].v3Return);
					director->player_1.hand.stack[i].animator = 0;
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_1.hand.stack[i].pat->setPosition(director->player_1.hand.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		default:
			break;
		}
	}
	//********************************Fieldanimation for field stacks********************************
	for (int i = 0; i < director->player_1.field.melee.stack.size(); i++)
	{
		switch (director->player_1.field.melee.stack[i].animator)
		{
		case 0:
			break;
		case 4:
			{
				osg::Vec3 distance = osg::Vec3(-22.0 + 78.0,director->player_1.field.melee.stack[i].direction.y(),director->player_1.field.melee.stack[i].direction.z())
					- director->player_1.field.melee.stack[i].pat->getPosition();
				if (distance.length() <= _step)
				{
					director->player_1.field.melee.stack[i].pat->
						setPosition(osg::Vec3(-22.0 + 78.0
						,director->player_1.field.melee.stack[i].direction.y(),
						director->player_1.field.melee.stack[i].direction.z()));
					director->player_1.field.melee.stack[i].animator = 5;
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_1.field.melee.stack[i].pat->setPosition(director->player_1.field.melee.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		case 5:
			{
				osg::Vec3 distance = director->player_1.field.melee.stack[i].direction	- director->player_1.field.melee.stack[i].pat->getPosition();
				if (distance.length() <= _step)
				{
					director->player_1.field.melee.stack[i].pat->setPosition(director->player_1.field.melee.stack[i].direction);
					director->player_1.field.melee.stack[i].animator = 0;
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_1.field.melee.stack[i].pat->setPosition(director->player_1.field.melee.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		default:
			break;
		}
	}
	for (int i = 0; i < director->player_1.field.archer.stack.size(); i++)
	{
		switch (director->player_1.field.archer.stack[i].animator)
		{
		case 0:
			break;
		case 4:
			{
				osg::Vec3 distance = osg::Vec3(-22.0 + 78.0,director->player_1.field.archer.stack[i].direction.y(),director->player_1.field.archer.stack[i].direction.z())
					- director->player_1.field.archer.stack[i].pat->getPosition();
				if (distance.length() <= _step)
				{
					director->player_1.field.archer.stack[i].pat->
						setPosition(osg::Vec3(-22.0 + 78.0
						,director->player_1.field.archer.stack[i].direction.y(),
						director->player_1.field.archer.stack[i].direction.z()));
					director->player_1.field.archer.stack[i].animator = 5;
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_1.field.archer.stack[i].pat->setPosition(director->player_1.field.archer.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		case 5:
			{
				osg::Vec3 distance = director->player_1.field.archer.stack[i].direction	- director->player_1.field.archer.stack[i].pat->getPosition();
				if (distance.length() <= _step)
				{
					director->player_1.field.archer.stack[i].pat->setPosition(director->player_1.field.archer.stack[i].direction);
					director->player_1.field.archer.stack[i].animator = 0;
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_1.field.archer.stack[i].pat->setPosition(director->player_1.field.archer.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		default:
			break;
		}
	}
	for (int i = 0; i < director->player_1.field.siege.stack.size(); i++)
	{
		switch (director->player_1.field.siege.stack[i].animator)
		{
		case 0:
			break;
		case 4:
			{
				osg::Vec3 distance = osg::Vec3(-22.0 + 78.0,director->player_1.field.siege.stack[i].direction.y(),director->player_1.field.siege.stack[i].direction.z())
					- director->player_1.field.siege.stack[i].pat->getPosition();
				if (distance.length() <= _step)
				{
					director->player_1.field.siege.stack[i].pat->
						setPosition(osg::Vec3(-22.0 + 78.0
						,director->player_1.field.siege.stack[i].direction.y(),
						director->player_1.field.siege.stack[i].direction.z()));
					director->player_1.field.siege.stack[i].animator = 5;
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_1.field.siege.stack[i].pat->setPosition(director->player_1.field.siege.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		case 5:
			{
				osg::Vec3 distance = director->player_1.field.siege.stack[i].direction	- director->player_1.field.siege.stack[i].pat->getPosition();
				if (distance.length() <= _step)
				{
					director->player_1.field.siege.stack[i].pat->setPosition(director->player_1.field.siege.stack[i].direction);
					director->player_1.field.siege.stack[i].animator = 0;
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_1.field.siege.stack[i].pat->setPosition(director->player_1.field.siege.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		default:
			break;
		}
	}
	for (int i = 0; i < director->player_2.field.melee.stack.size(); i++)
	{
		switch (director->player_2.field.melee.stack[i].animator)
		{
		case 0:
			break;
		case 4:
			{
				osg::Vec3 distance = osg::Vec3(-22.0 + 78.0,director->player_2.field.melee.stack[i].direction.y(),director->player_2.field.melee.stack[i].direction.z())
					- director->player_2.field.melee.stack[i].pat->getPosition();
				if (distance.length() <= _step)
				{
					director->player_2.field.melee.stack[i].pat->
						setPosition(osg::Vec3(-22.0 + 78.0
						,director->player_2.field.melee.stack[i].direction.y(),
						director->player_2.field.melee.stack[i].direction.z()));
					director->player_2.field.melee.stack[i].animator = 5;
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_2.field.melee.stack[i].pat->setPosition(director->player_2.field.melee.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		case 5:
			{
				osg::Vec3 distance = director->player_2.field.melee.stack[i].direction	- director->player_2.field.melee.stack[i].pat->getPosition();
				if (distance.length() <= _step)
				{
					director->player_2.field.melee.stack[i].pat->setPosition(director->player_2.field.melee.stack[i].direction);
					director->player_2.field.melee.stack[i].animator = 0;
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_2.field.melee.stack[i].pat->setPosition(director->player_2.field.melee.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		default:
			break;
		}
	}
	for (int i = 0; i < director->player_2.field.archer.stack.size(); i++)
	{
		switch (director->player_2.field.archer.stack[i].animator)
		{
		case 0:
			break;
		case 4:
			{
				osg::Vec3 distance = osg::Vec3(-22.0 + 78.0,director->player_2.field.archer.stack[i].direction.y(),director->player_2.field.archer.stack[i].direction.z())
					- director->player_2.field.archer.stack[i].pat->getPosition();
				if (distance.length() <= _step)
				{
					director->player_2.field.archer.stack[i].pat->
						setPosition(osg::Vec3(-22.0 + 78.0
						,director->player_2.field.archer.stack[i].direction.y(),
						director->player_2.field.archer.stack[i].direction.z()));
					director->player_2.field.archer.stack[i].animator = 5;
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_2.field.archer.stack[i].pat->setPosition(director->player_2.field.archer.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		case 5:
			{
				osg::Vec3 distance = director->player_2.field.archer.stack[i].direction	- director->player_2.field.archer.stack[i].pat->getPosition();
				if (distance.length() <= _step)
				{
					director->player_2.field.archer.stack[i].pat->setPosition(director->player_2.field.archer.stack[i].direction);
					director->player_2.field.archer.stack[i].animator = 0;
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_2.field.archer.stack[i].pat->setPosition(director->player_2.field.archer.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		default:
			break;
		}
	}
	for (int i = 0; i < director->player_2.field.siege.stack.size(); i++)
	{
		switch (director->player_2.field.siege.stack[i].animator)
		{
		case 0:
			break;
		case 4:
			{
				osg::Vec3 distance = osg::Vec3(-22.0 + 78.0,director->player_2.field.siege.stack[i].direction.y(),director->player_2.field.siege.stack[i].direction.z())
					- director->player_2.field.siege.stack[i].pat->getPosition();
				if (distance.length() <= _step)
				{
					director->player_2.field.siege.stack[i].pat->
						setPosition(osg::Vec3(-22.0 + 78.0
						,director->player_2.field.siege.stack[i].direction.y(),
						director->player_2.field.siege.stack[i].direction.z()));
					director->player_2.field.siege.stack[i].animator = 5;
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_2.field.siege.stack[i].pat->setPosition(director->player_2.field.siege.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		case 5:
			{
				osg::Vec3 distance = director->player_2.field.siege.stack[i].direction	- director->player_2.field.siege.stack[i].pat->getPosition();
				if (distance.length() <= _step)
				{
					director->player_2.field.siege.stack[i].pat->setPosition(director->player_2.field.siege.stack[i].direction);
					director->player_2.field.siege.stack[i].animator = 0;
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_2.field.siege.stack[i].pat->setPosition(director->player_2.field.siege.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		default:
			break;
		}
	}
	for (int i = 0; i < director->player_1.field.weather.stack.size(); i++)
	{
		switch (director->player_1.field.weather.stack[i].animator)
		{
		case 0:
			break;
		case 4:
			{
				osg::Vec3 distance = osg::Vec3(-22.0 + 78.0,director->player_1.field.weather.stack[i].direction.y(),director->player_1.field.weather.stack[i].direction.z())
					- director->player_1.field.weather.stack[i].pat->getPosition();
				if (distance.length() <= _step)
				{
					director->player_1.field.weather.stack[i].pat->
						setPosition(osg::Vec3(-22.0 + 78.0
						,director->player_1.field.weather.stack[i].direction.y(),
						director->player_1.field.weather.stack[i].direction.z()));
					director->player_1.field.weather.stack[i].animator = 5;
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_1.field.weather.stack[i].pat->setPosition(director->player_1.field.weather.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		case 5:
			{
				osg::Vec3 distance = director->player_1.field.weather.stack[i].direction	- director->player_1.field.weather.stack[i].pat->getPosition();
				if (distance.length() <= _step)
				{
					director->player_1.field.weather.stack[i].pat->setPosition(director->player_1.field.weather.stack[i].direction);
					director->player_1.field.weather.stack[i].animator = 0;
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_1.field.weather.stack[i].pat->setPosition(director->player_1.field.weather.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		default:
			break;
		}
	}
	for (int i = 0; i < director->player_1.field.meleeHorn.stack.size(); i++)
	{
		switch (director->player_1.field.meleeHorn.stack[i].animator)
		{
		case 0:
			break;
		case 4:
			{
				osg::Vec3 distance = osg::Vec3(-22.0 + 78.0,director->player_1.field.meleeHorn.stack[i].direction.y(),director->player_1.field.meleeHorn.stack[i].direction.z())
					- director->player_1.field.meleeHorn.stack[i].pat->getPosition();
				if (distance.length() <= _step)
				{
					director->player_1.field.meleeHorn.stack[i].pat->
						setPosition(osg::Vec3(-22.0 + 78.0
						,director->player_1.field.meleeHorn.stack[i].direction.y(),
						director->player_1.field.meleeHorn.stack[i].direction.z()));
					director->player_1.field.meleeHorn.stack[i].animator = 5;
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_1.field.meleeHorn.stack[i].pat->setPosition(director->player_1.field.meleeHorn.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		case 5:
			{
				osg::Vec3 distance = director->player_1.field.meleeHorn.stack[i].direction	- director->player_1.field.meleeHorn.stack[i].pat->getPosition();
				if (distance.length() <= _step)
				{
					director->player_1.field.meleeHorn.stack[i].pat->setPosition(director->player_1.field.meleeHorn.stack[i].direction);
					director->player_1.field.meleeHorn.stack[i].animator = 0;
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_1.field.meleeHorn.stack[i].pat->setPosition(director->player_1.field.meleeHorn.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		default:
			break;
		}
	}
	for (int i = 0; i < director->player_1.field.archerHorn.stack.size(); i++)
	{
		switch (director->player_1.field.archerHorn.stack[i].animator)
		{
		case 0:
			break;
		case 4:
			{
				osg::Vec3 distance = osg::Vec3(-22.0 + 78.0,director->player_1.field.archerHorn.stack[i].direction.y(),director->player_1.field.archerHorn.stack[i].direction.z())
					- director->player_1.field.archerHorn.stack[i].pat->getPosition();
				if (distance.length() <= _step)
				{
					director->player_1.field.archerHorn.stack[i].pat->
						setPosition(osg::Vec3(-22.0 + 78.0
						,director->player_1.field.archerHorn.stack[i].direction.y(),
						director->player_1.field.archerHorn.stack[i].direction.z()));
					director->player_1.field.archerHorn.stack[i].animator = 5;
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_1.field.archerHorn.stack[i].pat->setPosition(director->player_1.field.archerHorn.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		case 5:
			{
				osg::Vec3 distance = director->player_1.field.archerHorn.stack[i].direction	- director->player_1.field.archerHorn.stack[i].pat->getPosition();
				if (distance.length() <= _step)
				{
					director->player_1.field.archerHorn.stack[i].pat->setPosition(director->player_1.field.archerHorn.stack[i].direction);
					director->player_1.field.archerHorn.stack[i].animator = 0;
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_1.field.archerHorn.stack[i].pat->setPosition(director->player_1.field.archerHorn.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		default:
			break;
		}
	}
	for (int i = 0; i < director->player_1.field.siegeHorn.stack.size(); i++)
	{
		switch (director->player_1.field.siegeHorn.stack[i].animator)
		{
		case 0:
			break;
		case 4:
			{
				osg::Vec3 distance = osg::Vec3(-22.0 + 78.0,director->player_1.field.siegeHorn.stack[i].direction.y(),director->player_1.field.siegeHorn.stack[i].direction.z())
					- director->player_1.field.siegeHorn.stack[i].pat->getPosition();
				if (distance.length() <= _step)
				{
					director->player_1.field.siegeHorn.stack[i].pat->
						setPosition(osg::Vec3(-22.0 + 78.0
						,director->player_1.field.siegeHorn.stack[i].direction.y(),
						director->player_1.field.siegeHorn.stack[i].direction.z()));
					director->player_1.field.siegeHorn.stack[i].animator = 5;
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_1.field.siegeHorn.stack[i].pat->setPosition(director->player_1.field.siegeHorn.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		case 5:
			{
				osg::Vec3 distance = director->player_1.field.siegeHorn.stack[i].direction	- director->player_1.field.siegeHorn.stack[i].pat->getPosition();
				if (distance.length() <= _step)
				{
					director->player_1.field.siegeHorn.stack[i].pat->setPosition(director->player_1.field.siegeHorn.stack[i].direction);
					director->player_1.field.siegeHorn.stack[i].animator = 0;
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_1.field.siegeHorn.stack[i].pat->setPosition(director->player_1.field.siegeHorn.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		default:
			break;
		}
	}
	for (int i = 0; i < director->player_1.garbage.stack.size(); i++)
	{
		switch (director->player_1.garbage.stack[i].animator)
		{
		case 0:
			break;
		case 4:
			{
				osg::Vec3 distance = osg::Vec3(-22.0 + 78.0,director->player_1.garbage.stack[i].direction.y(),director->player_1.garbage.stack[i].direction.z())
					- director->player_1.garbage.stack[i].pat->getPosition();
				if (distance.length() <= _step)
				{
					director->player_1.garbage.stack[i].pat->
						setPosition(osg::Vec3(-22.0 + 78.0
						,director->player_1.garbage.stack[i].direction.y(),
						director->player_1.garbage.stack[i].direction.z()));
					director->player_1.garbage.stack[i].animator = 5;
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_1.garbage.stack[i].pat->setPosition(director->player_1.garbage.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		case 5:
			{
				osg::Vec3 distance = director->player_1.garbage.stack[i].direction	- director->player_1.garbage.stack[i].pat->getPosition();
				if (distance.length() <= _step)
				{
					director->player_1.garbage.stack[i].pat->setPosition(director->player_1.garbage.stack[i].direction);
					director->player_1.garbage.stack[i].animator = 0;
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_1.garbage.stack[i].pat->setPosition(director->player_1.garbage.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		case 6:
			{
				osg::Vec3 distance = director->player_1.garbage.stack[i].direction	- director->player_1.garbage.stack[i].pat->getPosition();
				if (distance.length() <= _step)
				{
					director->player_1.garbage.stack[i].pat->setPosition(director->player_1.garbage.stack[i].direction);
					director->player_1.garbage.stack[i].animator = 0;
					director->camera->removeChild(director->player_1.garbage.PopCard(i).pat.get());
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_1.garbage.stack[i].pat->setPosition(director->player_1.garbage.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		default:
			break;
		}
	}
	for (int i = 0; i < director->player_2.garbage.stack.size(); i++)
	{
		switch (director->player_2.garbage.stack[i].animator)
		{
		case 0:
			break;
		case 4:
			{
				osg::Vec3 distance = osg::Vec3(-22.0 + 78.0,director->player_2.garbage.stack[i].direction.y(),director->player_2.garbage.stack[i].direction.z())
					- director->player_2.garbage.stack[i].pat->getPosition();
				if (distance.length() <= _step)
				{
					director->player_2.garbage.stack[i].pat->
						setPosition(osg::Vec3(-22.0 + 78.0
						,director->player_2.garbage.stack[i].direction.y(),
						director->player_2.garbage.stack[i].direction.z()));
					director->player_2.garbage.stack[i].animator = 5;
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_2.garbage.stack[i].pat->setPosition(director->player_2.garbage.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		case 5:
			{
				osg::Vec3 distance = director->player_2.garbage.stack[i].direction	- director->player_2.garbage.stack[i].pat->getPosition();
				if (distance.length() <= _step)
				{
					director->player_2.garbage.stack[i].pat->setPosition(director->player_2.garbage.stack[i].direction);
					director->player_2.garbage.stack[i].animator = 0;
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_2.garbage.stack[i].pat->setPosition(director->player_2.garbage.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		case 6:
			{
				osg::Vec3 distance = director->player_2.garbage.stack[i].direction	- director->player_2.garbage.stack[i].pat->getPosition();
				if (distance.length() <= _step)
				{
					director->player_2.garbage.stack[i].pat->setPosition(director->player_2.garbage.stack[i].direction);
					director->player_2.garbage.stack[i].animator = 0;
					director->camera->removeChild(director->player_2.garbage.PopCard(i).pat.get());
				}
				else
				{
					distance.normalize();
					distance *= _step;
					director->player_2.garbage.stack[i].pat->setPosition(director->player_2.garbage.stack[i].pat->getPosition() + distance);
				}
			}
			break;
		default:
			break;
		}
	}

}
