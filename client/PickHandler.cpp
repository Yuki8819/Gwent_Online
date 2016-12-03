#include "PickHandler.h"

bool PickHandler::handle( const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa ) 
{
	osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);    
	if ( viewer )
	{
		//login
		if (director->StateMachine == -3)
		{
			if(ea.getEventType()==osgGA::GUIEventAdapter::KEYDOWN)
			{
				//entering IP
				switch(ea.getKey())
				{
				case osgGA::GUIEventAdapter::KEY_0:
				case osgGA::GUIEventAdapter::KEY_KP_0:
					director->ip += "0";
					break;
				case osgGA::GUIEventAdapter::KEY_1:
				case osgGA::GUIEventAdapter::KEY_KP_1:
					director->ip += "1";
					break;
				case osgGA::GUIEventAdapter::KEY_2:
				case osgGA::GUIEventAdapter::KEY_KP_2:
					director->ip += "2";
					break;
				case osgGA::GUIEventAdapter::KEY_3:
				case osgGA::GUIEventAdapter::KEY_KP_3:
					director->ip += "3";
					break;
				case osgGA::GUIEventAdapter::KEY_4:
				case osgGA::GUIEventAdapter::KEY_KP_4:
					director->ip += "4";
					break;
				case osgGA::GUIEventAdapter::KEY_5:
				case osgGA::GUIEventAdapter::KEY_KP_5:
					director->ip += "5";
					break;
				case osgGA::GUIEventAdapter::KEY_6:
				case osgGA::GUIEventAdapter::KEY_KP_6:
					director->ip += "6";
					break;
				case osgGA::GUIEventAdapter::KEY_7:
				case osgGA::GUIEventAdapter::KEY_KP_7:
					director->ip += "7";
					break;
				case osgGA::GUIEventAdapter::KEY_8:
				case osgGA::GUIEventAdapter::KEY_KP_8:
					director->ip += "8";
					break;
				case osgGA::GUIEventAdapter::KEY_9:
				case osgGA::GUIEventAdapter::KEY_KP_9:
					director->ip += "9";
					break;
				case osgGA::GUIEventAdapter::KEY_Period:
					director->ip += ".";
					break;
				case osgGA::GUIEventAdapter::KEY_BackSpace:
					if (!director->ip.empty())
						director->ip.pop_back();
					break;
				case osgGA::GUIEventAdapter::KEY_Return:
				case osgGA::GUIEventAdapter::KEY_KP_Enter:
					
					WSADATA              wsaData;
					SOCKADDR_IN          serverAddr;	
					char				  buf[MAX];		
					SOCKET               Sconnect;
					WSAStartup(MAKEWORD(2,2), &wsaData);
					Sconnect = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
					serverAddr.sin_family = AF_INET;
					serverAddr.sin_port = htons(SERVPORT);    
					serverAddr.sin_addr.s_addr = inet_addr(AssistantDirector::stringToChar(director->ip));
					memset(&(serverAddr.sin_zero), 0, sizeof(serverAddr.sin_zero));
					if(connect(Sconnect,(SOCKADDR*)&serverAddr,sizeof(SOCKADDR))==SOCKET_ERROR){
						printf("Connect failed!\n");
						return false ;
					}
					else{
						printf("Connecting...\n");
					}
					recv(Sconnect,buf,sizeof(buf),0);
					if(buf[0]==SERVERFULL){
						closesocket(Sconnect);
						Sconnect = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
						serverAddr.sin_family = AF_INET;
						serverAddr.sin_port = htons(SERVPORT);    
						serverAddr.sin_addr.s_addr = inet_addr(AssistantDirector::stringToChar(director->ip));
						memset(&(serverAddr.sin_zero), 0, sizeof(serverAddr.sin_zero));
						Sleep(1000);
						if( connect(Sconnect,(SOCKADDR*)&serverAddr,sizeof(SOCKADDR))==SOCKET_ERROR){
							printf("Connect\n");
						}
						recv(Sconnect,buf,sizeof(buf),0);
						if(buf[0]==SERVERFULL){
							printf("Server is full!\n");
							closesocket(Sconnect);
							WSACleanup();
							//return 0;
							return false;
						}else if(buf[0]==CONNECTSUCCESS){
							printf("Connect success!\n");
						}else{
							printf("Connect failed!\n");
							closesocket(Sconnect);
							WSACleanup();
							
							return false;
						}
						
					}else if(buf[0]==CONNECTSUCCESS){
						printf("Connect success!\n");
					}else{
						printf("Connect failed!\n");
						closesocket(Sconnect);
						WSACleanup();
						return false ;
					}
					DataReceiverThread::instance()->setSocket(Sconnect);
					AssistantDirector::setSocket(Sconnect);
					//connection set, enter the game
					//move camera to the room selection scene
					director->StateMachine = -2;
					director->camera->setViewMatrixAsLookAt(osg::Vec3(0.0,0.0,-108.0), osg::Vec3(0.0,1.0,-108.0), osg::Vec3(0.0,0.0,1.0));
					//receive room info
					recv(Sconnect,buf,sizeof(buf),0);
					if(buf[0]==SERVERMSG){
						printf("Server message:\n");
						for(int i=0;i<MAXROOM;i++){
							if((int)buf[i+1]==ROOMFULL){
								director->available[i]=0;
								printf("Room %d is full\n",i+1);
								if (i+1 == 1)
								{
									director->t_Room_1->setText("Full");
								}
								else if (i+1 == 2)
								{
									director->t_Room_2->setText("Full");
								}
							}else{
								director->available[i]=1;
								printf("Room %d is available\n",i+1);
								if (i+1 == 1)
								{
									director->t_Room_1->setText("Available");
								}
								else if (i+1 == 2)
								{
									director->t_Room_2->setText("Available");
								}
							}
						}
					}else{
						printf("Error!\n");
						closesocket(Sconnect);
						WSACleanup();
					}
					return false;
					
					break;
				}
				director->t_IP->setText(director->ip);
				director->t_IP->update();
				return false;
			}
			else
			{
				return false;
			}
		}
		if (director->StateMachine == -2)
		{
			if ( ea.getEventType()!=osgGA::GUIEventAdapter::RELEASE || ea.getButton()!=osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON/* || !(ea.getModKeyMask()&osgGA::GUIEventAdapter::MODKEY_CTRL)*/)
				return false;
			float x = ea.getX();
			float y = ea.getY();
			std::cout<<x<<","<<y<<std::endl;
			int n = -1;
			//mouse clicks on room to enter
			if (x >= 273 && x <= 513 && y >= 227 && y <= 707)
			{
				n = 1;
			}
			else if (x >= 897 && x <= 1139 && y >= 227 && y <= 707)
			{
				n = 2;
			}
			else
			{
				return false;
			}
			char buf[3];
			buf[0]=ROOMSELECT;
			buf[1]=n;
			buf[2]='\0';
			AssistantDirector::Send_msg(buf);

			char result[MAX];

			AssistantDirector::Recv_msg(result);
			if(result[0]==WAITSTART)
			{
				//wait for start
				printf("Wait start!\n");
			}else if(result[0]==WAITANOTHER)
			{
				//wait for another player
				printf("Wait anotherone!\n");
			}
			else if(result[0]==RESELECT)
			{
				//room full
				printf("Room is full\n");
				//initialize
				closesocket(AssistantDirector::sockets);
				DataReceiverThread::instance()->cleanSocket();
				WSACleanup();
				DataReceiverThread::instance()->cancel();
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
				return false;
			}
			else
			{
				printf("something is wrong!\n");
				return false;
			}
			//room entered
			//start the receiving thread, set state machine, move camera
			DataReceiverThread::instance()->startThread();
			director->StateMachine = -1;
			director->camera->setViewMatrixAsLookAt(osg::Vec3(0.0,0.0,-0.05), osg::Vec3(0.0,10.0,0.0), osg::Vec3(0.0,-0.05,10.0));
			return false;
		}

		//inputs actually in game
		//give up?
		if (director->StateMachine == 0)
		{
			if (ea.getEventType()==osgGA::GUIEventAdapter::KEYDOWN && ea.getKey()==osgGA::GUIEventAdapter::KEY_Q)
			{
				Sleep(2000);
				AssistantDirector::Send_msg("giveup");
				AssistantDirector::Send_msg(AssistantDirector::intToString(director->player_1.field.score) + 
								":" + AssistantDirector::intToString(director->player_2.field.score));
				director->player_1.isGiveup = true;
				director->StateMachine = 1;
				return false;
			}
		}
		//mouse animation
		if ( ea.getEventType()==osgGA::GUIEventAdapter::MOVE)
		{
			//the light following the mouse
			float x,z;
			x = (ea.getX()*1920/1280)/10 - 96.0;
			z = (ea.getY()*1080/720)/10 - 54.0;
			director->light2->setPosition(osg::Vec3(x,0.0,z));

			//intersector, to indicate which objects mouse clicked on
			osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector = new osgUtil::LineSegmentIntersector(osgUtil::Intersector::WINDOW, ea.getX(), ea.getY());   

			osgUtil::IntersectionVisitor iv( intersector.get() );   
			iv.setTraversalMask( ~0x1 );      
			viewer->getCamera()->accept( iv );         
			if ( intersector->containsIntersections() )    
			{          
				osgUtil::LineSegmentIntersector::Intersection result = *(intersector->getIntersections().begin());
				//hand stack
				for (int i = 0; i < director->player_1.hand.stack.size(); i++)
				{
					if (director->player_1.hand.stack[i].isPickerActive 
						&& !director->player_1.hand.stack[i].isSelected
						&& director->player_1.hand.stack[i].animator == 0)
					{
						if (result.drawable == director->player_1.hand.stack[i].boxShapeDrawable)
						{
							director->player_1.hand.stack[i].pat->setPosition(
								director->player_1.hand.stack[i].v3Return
								+ osg::Vec3(0.0,-0.5,2.0));
						}
						else
						{
							director->player_1.hand.stack[i].pat->setPosition(director->player_1.hand.stack[i].v3Return);
						}
					}
				}
				//field stack
				for (int i = 0; i < director->player_1.field.melee.stack.size(); i++)
				{
					if (director->player_1.field.melee.stack[i].animator == 0)
					{
						if (result.drawable == director->player_1.field.melee.stack[i].boxShapeDrawable)
						{
							director->player_1.field.melee.stack[i].pat->setPosition(
								director->player_1.field.melee.stack[i].v3Return
								+ osg::Vec3(0.0,-0.5,0.5));
						}
						else
						{
							director->player_1.field.melee.stack[i].pat->setPosition(director->player_1.field.melee.stack[i].v3Return);
						}
					}
				}
				for (int i = 0; i < director->player_1.field.archer.stack.size(); i++)
				{
					if (director->player_1.field.archer.stack[i].animator == 0)
					{
						if (result.drawable == director->player_1.field.archer.stack[i].boxShapeDrawable)
						{
							director->player_1.field.archer.stack[i].pat->setPosition(
								director->player_1.field.archer.stack[i].v3Return
								+ osg::Vec3(0.0,-0.5,1.0));
						}
						else
						{
							director->player_1.field.archer.stack[i].pat->setPosition(director->player_1.field.archer.stack[i].v3Return);
						}
					}
				}
				for (int i = 0; i < director->player_1.field.siege.stack.size(); i++)
				{
					if (director->player_1.field.siege.stack[i].animator == 0)
					{
						if (result.drawable == director->player_1.field.siege.stack[i].boxShapeDrawable)
						{
							director->player_1.field.siege.stack[i].pat->setPosition(
								director->player_1.field.siege.stack[i].v3Return
								+ osg::Vec3(0.0,-0.5,1.5));
						}
						else
						{
							director->player_1.field.siege.stack[i].pat->setPosition(director->player_1.field.siege.stack[i].v3Return);
						}
					}
				}
				//garbage stack
				for (int i = 0; i < director->player_1.garbage.stack.size(); i++)
				{
					if (director->isCuring
						&& director->player_1.garbage.stack[i].animator == 0)
					{
						if (result.drawable == director->player_1.garbage.stack[i].boxShapeDrawable)
						{
							director->player_1.garbage.stack[i].material->setEmission(osg::Material::FRONT, osg::Vec4(1.0,0.25,0.25,1.0));
						}
						else
						{
							director->player_1.garbage.stack[i].material->setEmission(osg::Material::FRONT, osg::Vec4(0.75,0.75,0.75,1.0));
						}
					}
				}
			}
		}

		//horn card
		if ( ea.getEventType()!=osgGA::GUIEventAdapter::RELEASE || ea.getButton()!=osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
			return false; 
		//play horn card
		if (director->isHorn)
		{
			float x = ea.getX();
			float y = ea.getY();
			int i = director->currentHandi;
			if (x >= AssistantDirector::xmin && x <= AssistantDirector::xmax)
			{
				if (y >= AssistantDirector::y[0] && y < AssistantDirector::y[1])
				{
					//siege
					director->player_1.field.siegeHorn.PushCard(director->player_1.hand.PopCard(i));
					int size = director->player_1.field.siegeHorn.stack.size();

					director->player_1.field.siegeHorn.stack[size - 1].direction = osg::Vec3(director->player_1.field.v3Siege.x() + AssistantDirector::hornX,
																								  director->player_1.field.v3Siege.y(),
																								  director->player_1.field.v3Siege.z());
					director->player_1.field.siegeHorn.stack[size - 1].animator = 5;

					director->isHorn = false;
					AssistantDirector::Send_msg("horn=2");
					AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(director->player_1.hand.stack.size()));
					if (!director->player_2.isGiveup)
						AssistantDirector::Send_msg("ready");
					director->player_1.field.CalculateScore();
					director->player_2.field.CalculateScore();
					return false;

				}
				else if (y >= AssistantDirector::y[1] && y < AssistantDirector::y[2])
				{
					//archer
					director->player_1.field.archerHorn.PushCard(director->player_1.hand.PopCard(i));
					int size = director->player_1.field.archerHorn.stack.size();

					director->player_1.field.archerHorn.stack[size - 1].direction = osg::Vec3(director->player_1.field.v3Archer.x() + AssistantDirector::hornX,
																								  director->player_1.field.v3Archer.y(),
																								  director->player_1.field.v3Archer.z());
					director->player_1.field.archerHorn.stack[size - 1].animator = 5;

					director->isHorn = false;
					AssistantDirector::Send_msg("horn=1");
					AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(director->player_1.hand.stack.size()));
					if (!director->player_2.isGiveup)
						AssistantDirector::Send_msg("ready");
					director->player_1.field.CalculateScore();
					director->player_2.field.CalculateScore();
					return false;
				}
				else if (y >= AssistantDirector::y[2] && y <= AssistantDirector::y[3])
				{
					//melee
					director->player_1.field.meleeHorn.PushCard(director->player_1.hand.PopCard(i));
					int size = director->player_1.field.meleeHorn.stack.size();

					director->player_1.field.meleeHorn.stack[size - 1].direction = osg::Vec3(director->player_1.field.v3Melee.x() + AssistantDirector::hornX,
																								  director->player_1.field.v3Melee.y(),
																								  director->player_1.field.v3Melee.z());
					director->player_1.field.meleeHorn.stack[size - 1].animator = 5;

					director->isHorn = false;
					AssistantDirector::Send_msg("horn=0");
					AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(director->player_1.hand.stack.size()));
					if (!director->player_2.isGiveup)
						AssistantDirector::Send_msg("ready");
					director->player_1.field.CalculateScore();
					director->player_2.field.CalculateScore();
					return false;
				}
			}
		}
		for (int i = 0; i < director->player_1.hand.stack.size(); i++)
		{
			{
				director->player_1.hand.stack[i].v3Return = osg::Vec3(AssistantDirector::v3Hand.x() + i * Card::x,
																	  AssistantDirector::v3Hand.y(),
																	  AssistantDirector::v3Hand.z());
			}
		}
		//select a card to play
		//intersector
		osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector = new osgUtil::LineSegmentIntersector(osgUtil::Intersector::WINDOW, ea.getX(), ea.getY());   
		osgUtil::IntersectionVisitor iv( intersector.get() );   
		iv.setTraversalMask( ~0x1 );      
		viewer->getCamera()->accept( iv );         
		if ( intersector->containsIntersections() )    
		{
			osgUtil::LineSegmentIntersector::Intersection result = *(intersector->getIntersections().begin());
			//scarecrow
			if (director->isScarecrow)
			{
				int handi = director->currentHandi;
				for (int i = 0; i < director->player_1.field.melee.stack.size(); i++)
				{
					if (result.drawable == director->player_1.field.melee.stack[i].boxShapeDrawable)
					{
						int num = director->player_1.field.melee.stack[i].no;
						director->player_1.field.melee.stack[i].isSelected = false;
						director->player_1.hand.PushCard(director->player_1.field.PopMelee(i));
						director->player_1.field.PushMelee(director->player_1.hand.PopCard(handi));

						director->isScarecrow = false;
						

						
						AssistantDirector::Send_msg("scarecrow=0," + AssistantDirector::intToString(num));
						AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(director->player_1.hand.stack.size()));
						if (!director->player_2.isGiveup)
							AssistantDirector::Send_msg("ready");
						director->player_1.field.CalculateScore();
						director->player_2.field.CalculateScore();
						return false;
					}
				}
				for (int i = 0; i < director->player_1.field.archer.stack.size(); i++)
				{
					if (result.drawable == director->player_1.field.archer.stack[i].boxShapeDrawable)
					{
						int num = director->player_1.field.archer.stack[i].no;
						director->player_1.field.archer.stack[i].isSelected = false;
						director->player_1.hand.PushCard(director->player_1.field.PopArcher(i));
						director->player_1.field.PushArcher(director->player_1.hand.PopCard(handi));

						director->isScarecrow = false;

						
						AssistantDirector::Send_msg("scarecrow=1," + AssistantDirector::intToString(num));
						AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(director->player_1.hand.stack.size()));
						if (!director->player_2.isGiveup)
							AssistantDirector::Send_msg("ready");
						director->player_1.field.CalculateScore();
						director->player_2.field.CalculateScore();
						return false;
					}
				}
				for (int i = 0; i < director->player_1.field.siege.stack.size(); i++)
				{
					if (result.drawable == director->player_1.field.siege.stack[i].boxShapeDrawable)
					{
						int num = director->player_1.field.siege.stack[i].no;
						director->player_1.field.siege.stack[i].isSelected = false;
						director->player_1.hand.PushCard(director->player_1.field.PopSiege(i));
						director->player_1.field.PushSiege(director->player_1.hand.PopCard(handi));

						director->isScarecrow = false;

						
						AssistantDirector::Send_msg("scarecrow=2," + AssistantDirector::intToString(num));
						AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(director->player_1.hand.stack.size()));
						if (!director->player_2.isGiveup)
							AssistantDirector::Send_msg("ready");
						director->player_1.field.CalculateScore();
						director->player_2.field.CalculateScore();
						return false;
					}
				}
				director->player_1.field.CalculateScore();
				director->player_2.field.CalculateScore();
			}
			//doctor
			if (director->isCuring)
			{
				if (!director->player_1.garbage.stack.empty())
				{
					for (int i = 0; i < director->player_1.garbage.stack.size(); i++)
					{
						if (result.drawable == director->player_1.garbage.stack[i].boxShapeDrawable)
						{
							Card card = director->player_1.garbage.stack[i];
							if (card.genre >= 0 && card.genre <= 2 && !card.isHero)
							{
								director->isCuring = false;
								director->player_1.garbage.stack[i].material->setEmission(osg::Material::FRONT, osg::Vec4(0.75,0.75,0.75,1.0));
								director->toFieldFromCard(director->player_1.garbage.PopCard(i));
								return false;
							}
						}
					}
				}
				else
				{
					director->isCuring = false;
					AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(director->player_1.hand.stack.size()));
					if (!director->player_2.isGiveup)
						AssistantDirector::Send_msg("ready");
					return false;
				}
			}
			//hand card selecting
			for (int i = 0; i < director->player_1.hand.stack.size(); i++)
			{
				if (director->player_1.hand.stack[i].isPickerActive)
				{
					if (result.drawable == director->player_1.hand.stack[i].boxShapeDrawable)
					{
						//set animator
						if (!director->player_1.hand.stack[i].isSelected)
						{
							director->player_1.hand.stack[i].isSelected = true;
							director->player_1.hand.stack[i].animator = 2;
							director->currentHandi = i;
						}
						//play the card
						else
						{
							director->toField(director->player_1.hand.stack[i]);
							break;
						}
					}
					else
					{
						//place the card back to hand stack
						if (director->player_1.hand.stack[i].isSelected)
						{
							director->player_1.hand.stack[i].isSelected = false;
							director->player_1.hand.stack[i].animator = 3;
						}
						else
						{
							//do nothing
						}
					}
				}
			}
		}  
	}   
	return false;
} 

PickHandler::PickHandler(Director *d)
{
	director = d;
}
			
PickHandler::~PickHandler()
{

}