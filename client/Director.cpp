#include "Director.h"

Director::Director()
{
	//initialize the game, including login interface, game scenes, UI nodes and card decks
	Init();
 
	light1_obj = new osg::Light;
	light1_obj->setLightNum( 0 );  
	light1_obj->setAmbient(osg::Vec4(0.25,0.25,0.25,1.0));
	light1_obj->setDiffuse( osg::Vec4(1.0f,1.0f,1.0f,1.0f) ); 
	light1_obj->setSpecular(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
	light1_obj->setPosition( osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f) ); 
	light1_obj->setDirection(osg::Vec3(0.0,10.0,0.0));

	osg::ref_ptr<osg::LightSource> lightSource1 = new osg::LightSource; 
	lightSource1->setLight( light1_obj ); 
	light1 = new osg::PositionAttitudeTransform;
	light1->setPosition(osg::Vec3(0.0,0.0,0.0));
	light1->addChild( lightSource1.get() );

	light2_obj = new osg::Light;
	light2_obj->setLightNum( 1 );  
	light2_obj->setAmbient(osg::Vec4(0.5,0.5,0.5,1.0));
	light2_obj->setDiffuse( osg::Vec4(1.0,1.0,1.0,1.0) ); 
	light2_obj->setSpecular( osg::Vec4(1.0,1.0,1.0,1.0) ); 
	light2_obj->setPosition( osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f) ); 
	light2_obj->setSpotExponent(1.0);
	light2_obj->setSpotCutoff(45.0);
	light2_obj->setDirection(osg::Vec3(0.0,10.0,0.0));

	osg::ref_ptr<osg::LightSource> lightSource2 = new osg::LightSource; 
	lightSource2->setLight( light2_obj );
	light2 = new osg::PositionAttitudeTransform;
	light2->setPosition(osg::Vec3(0.0,0.0,0.0));
	light2->addChild( lightSource2.get() );

	//initialize camera
	Init_Camera();
	ip = "127.0.0.1";
	//create UI texts
	t_IP = AssistantDirector::createText(osg::Vec3(-6.6,9.99,-217.8),"127.0.0.1", 3.0);
	t_IP->setColor(osg::Vec4(0.0,0.0,0.0,1.0));

	t_Room_1 = AssistantDirector::createText(osg::Vec3(-45,9.90,-132),"Available", 3.0);
	t_Room_1->setColor(osg::Vec4(0.0,0.0,0.0,1.0));
	t_Room_1->setBackdropType(osgText::Text::BackdropType::DROP_SHADOW_BOTTOM_RIGHT);
	t_Room_1->setBackdropColor(osg::Vec4(1.0,1.0,1.0,1.0));

	t_Room_2 = AssistantDirector::createText(osg::Vec3(48,9.90,-132),"Available", 3.0);
	t_Room_2->setColor(osg::Vec4(0.0,0.0,0.0,1.0));
	t_Room_2->setBackdropType(osgText::Text::BackdropType::DROP_SHADOW_BOTTOM_RIGHT);
	t_Room_2->setBackdropColor(osg::Vec4(1.0,1.0,1.0,1.0));

	t_Gaming_Caution = AssistantDirector::createText(osg::Vec3(-40.0,9.50,0.0),"", 5.0);
	t_Gaming_Caution->setColor(osg::Vec4(0.2,0.2,0.2,1.0));
	t_Gaming_Caution->setBackdropType(osgText::Text::BackdropType::DROP_SHADOW_BOTTOM_RIGHT);
	t_Gaming_Caution->setBackdropColor(osg::Vec4(1.0,1.0,1.0,1.0));
	//add UI to the scene
	UIGeode->addDrawable(t_IP.get());
	UIGeode->addDrawable(t_Room_1.get());
	UIGeode->addDrawable(t_Room_2.get());
	UIGeode->addDrawable(t_Gaming_Caution.get());
	camera -> addChild(UIGeode.get());
	
	//create background for game, including binding textures, normal mapping and create nodes

	osg::ref_ptr<osg::Node> backgroundnode = new osg::Node;

	backgroundnode = osgDB::readNodeFile("Gwent\\Desk_with_Bump_Mapping.osgb");

	osg::ref_ptr<osgFX::BumpMapping> backgroundBumpMapping = new osgFX::BumpMapping;

	backgroundBumpMapping->addChild(backgroundnode.get());

	backgroundBumpMapping->setLightNumber(0);

	backgroundBumpMapping->setDiffuseTextureUnit(0);

	backgroundBumpMapping->setNormalMapTextureUnit(1);

	backgroundBumpMapping->prepareChildren();


	osg::ref_ptr<osg::PositionAttitudeTransform> backpat = new osg::PositionAttitudeTransform;

	backpat->addChild(backgroundBumpMapping.get());

	backpat->setPosition(osg::Vec3(0.0,10.0,0.0));

	camera->addChild(backpat.get());

	//login interface
	osg::ref_ptr<osg::Texture2D> loginTexture = new osg::Texture2D;  
    loginTexture->setDataVariance(osg::Object::DYNAMIC);  
    osg::ref_ptr<osg::Image> ImageLogin = osgDB::readImageFile("Gwent\\login.jpg");  
    if (!ImageLogin.get())  
    {  
        std::cout<<"Failed to open the file!"<<std::endl;  
		system("PAUSE");
        //return -1;  
    }
    loginTexture->setImage(ImageLogin.get());  
	osg::ref_ptr<osg::Geode> login = AssistantDirector::createBox(osg::Vec3(0, 10, -216.0), 192.0, 0.01, 108.0);  
    //Create a TexEnv instance for binding the StateSet  
    osg::ref_ptr<osg::TexEnv> loginTexEnv = new osg::TexEnv;  
    loginTexEnv->setMode(osg::TexEnv::DECAL);  
    osg::ref_ptr<osg::StateSet> loginStateSet = new osg::StateSet;  
    loginStateSet->setAttribute(loginTexEnv.get());  
    loginStateSet->setAttributeAndModes(loginTexture.get());  
    login->setStateSet(loginStateSet.get());
	camera -> addChild(login.get());


	//room selection interface
	osg::ref_ptr<osg::Texture2D> roomTexture = new osg::Texture2D;  
    roomTexture->setDataVariance(osg::Object::DYNAMIC);  
    osg::ref_ptr<osg::Image> ImageRoom = osgDB::readImageFile("Gwent\\room.jpg");  
    if (!ImageRoom.get())  
    {
        std::cout<<"Failed to open the file!"<<std::endl;  
		system("PAUSE");
        //return -1;  
    }
    roomTexture->setImage(ImageRoom.get());  
	osg::ref_ptr<osg::Geode> room = AssistantDirector::createBox(osg::Vec3(0, 10, -108.0), 192.0, 0.01, 108.0);  
    //Create a TexEnv instance for binding the StateSet  
    osg::ref_ptr<osg::TexEnv> roomTexEnv = new osg::TexEnv;  
    roomTexEnv->setMode(osg::TexEnv::DECAL);  
    osg::ref_ptr<osg::StateSet> roomStateSet = new osg::StateSet;  
    roomStateSet->setAttribute(roomTexEnv.get());  
    roomStateSet->setAttributeAndModes(roomTexture.get());  
    room->setStateSet(roomStateSet.get());
	camera -> addChild(room.get());

	//set camera to the login interface
	float camera_z;
	switch (StateMachine)
	{
	case -3:
		camera_z = -216.0;
		break;
	case -2:
		camera_z = -108.0;
		break;
	default:
		camera_z = 0.0;
		break;
	}
	camera -> setViewMatrixAsLookAt(osg::Vec3(0.0,0.0,camera_z), osg::Vec3(0.0,10.0,camera_z), osg::Vec3(0.0,0.0,1.0));

}

void Director::Init()
{
	root = new osg::Group;
	//set the state machine as "entering IP"
	StateMachine = -3;
	isHorn = false;
	isScarecrow = false;
	isCuring = false;
	p2Horn = -1;
	p2ScarecrowNo = -1;
	p2ScarecrowLine = -1;
	player_1 = Player();
	player_1.field.Init(true);
	player_1.garbage.v3Garbage = AssistantDirector::v3Garbaged;
	player_2 = Player();
	player_2.field.Init(false);
	player_2.handCount = 10;
	player_2.garbage.v3Garbage = AssistantDirector::v3Garbaged_2;
	//UI texts
	player_2.t_handCount = AssistantDirector::createText(AssistantDirector::v3HandCount_2,AssistantDirector::intToString(player_2.handCount), 3.0);
	player_2.t_handCount->setColor(osg::Vec4(1.0,1.0,1.0,1.0));
	player_2.t_handCount->setBackdropType(osgText::Text::BackdropType::OUTLINE);
	player_2.t_handCount->setBackdropColor(osg::Vec4(0.0,0.0,0.0,1.0));
	player_1.t_gamePoints = AssistantDirector::createText(AssistantDirector::v3HandCount + osg::Vec3(10.0,0.0,0.0),AssistantDirector::intToString(2 - player_1.loss), 3.0);
	player_1.t_gamePoints->setColor(osg::Vec4(1.0,1.0,1.0,1.0));
	player_1.t_gamePoints->setBackdropType(osgText::Text::BackdropType::OUTLINE);
	player_1.t_gamePoints->setBackdropColor(osg::Vec4(0.0,0.0,0.0,1.0));
	player_2.t_gamePoints = AssistantDirector::createText(AssistantDirector::v3HandCount_2 + osg::Vec3(10.0,0.0,0.0),AssistantDirector::intToString(2 - player_2.loss), 3.0);
	player_2.t_gamePoints->setColor(osg::Vec4(1.0,1.0,1.0,1.0));
	player_2.t_gamePoints->setBackdropType(osgText::Text::BackdropType::OUTLINE);
	player_2.t_gamePoints->setBackdropColor(osg::Vec4(0.0,0.0,0.0,1.0));
	
	UIGeode = new osg::Geode;
	UIGeode->addDrawable(player_1.field.t_score.get());
	UIGeode->addDrawable(player_1.field.t_score_melee.get());
	UIGeode->addDrawable(player_1.field.t_score_archer.get());
	UIGeode->addDrawable(player_1.field.t_score_siege.get());
	UIGeode->addDrawable(player_2.field.t_score.get());
	UIGeode->addDrawable(player_2.field.t_score_melee.get());
	UIGeode->addDrawable(player_2.field.t_score_archer.get());
	UIGeode->addDrawable(player_2.field.t_score_siege.get());
	UIGeode->addDrawable(player_2.t_handCount.get());
	UIGeode->addDrawable(player_1.t_gamePoints.get());
	UIGeode->addDrawable(player_2.t_gamePoints.get());
}

void Director::CleanAllCards()
{
	//P1
	while(!player_1.hand.stack.empty())
	{
		camera->removeChild(player_1.hand.stack[0].pat.get());
		player_1.hand.stack.pop_front();
	}
	while(!player_1.field.melee.stack.empty())
	{
		camera->removeChild(player_1.field.melee.stack[0].pat.get());
		player_1.field.melee.stack.pop_front();
	}
	while(!player_1.field.archer.stack.empty())
	{
		camera->removeChild(player_1.field.archer.stack[0].pat.get());
		player_1.field.archer.stack.pop_front();
	}
	while(!player_1.field.siege.stack.empty())
	{
		camera->removeChild(player_1.field.siege.stack[0].pat.get());
		player_1.field.siege.stack.pop_front();
	}
	while(!player_1.field.weather.stack.empty())
	{
		camera->removeChild(player_1.field.weather.stack[0].pat.get());
		player_1.field.weather.stack.pop_front();
	}
	while(!player_1.garbage.stack.empty())
	{
		camera->removeChild(player_1.garbage.stack[0].pat.get());
		player_1.garbage.stack.pop_front();
	}
	while(!player_1.field.meleeHorn.stack.empty())
	{
		camera->removeChild(player_1.field.meleeHorn.stack[0].pat.get());
		player_1.field.meleeHorn.stack.pop_front();
	}
	while(!player_1.field.archerHorn.stack.empty())
	{
		camera->removeChild(player_1.field.archerHorn.stack[0].pat.get());
		player_1.field.archerHorn.stack.pop_front();
	}
	while(!player_1.field.siegeHorn.stack.empty())
	{
		camera->removeChild(player_1.field.siegeHorn.stack[0].pat.get());
		player_1.field.siegeHorn.stack.pop_front();
	}
	//P2
	while(!player_2.field.melee.stack.empty())
	{
		camera->removeChild(player_2.field.melee.stack[0].pat.get());
		player_2.field.melee.stack.pop_front();
	}
	while(!player_2.field.archer.stack.empty())
	{
		camera->removeChild(player_2.field.archer.stack[0].pat.get());
		player_2.field.archer.stack.pop_front();
	}
	while(!player_2.field.siege.stack.empty())
	{
		camera->removeChild(player_2.field.siege.stack[0].pat.get());
		player_2.field.siege.stack.pop_front();
	}
	while(!player_2.field.weather.stack.empty())
	{
		camera->removeChild(player_2.field.weather.stack[0].pat.get());
		player_2.field.weather.stack.pop_front();
	}
	while(!player_2.garbage.stack.empty())
	{
		camera->removeChild(player_2.garbage.stack[0].pat.get());
		player_2.garbage.stack.pop_front();
	}
	while(!player_2.field.meleeHorn.stack.empty())
	{
		camera->removeChild(player_2.field.meleeHorn.stack[0].pat.get());
		player_2.field.meleeHorn.stack.pop_front();
	}
	while(!player_2.field.archerHorn.stack.empty())
	{
		camera->removeChild(player_2.field.archerHorn.stack[0].pat.get());
		player_2.field.archerHorn.stack.pop_front();
	}
	while(!player_2.field.siegeHorn.stack.empty())
	{
		camera->removeChild(player_2.field.siegeHorn.stack[0].pat.get());
		player_2.field.siegeHorn.stack.pop_front();
	}
	
	UIGeode->removeDrawable(player_1.field.t_score.get());
	UIGeode->removeDrawable(player_1.field.t_score_melee.get());
	UIGeode->removeDrawable(player_1.field.t_score_archer.get());
	UIGeode->removeDrawable(player_1.field.t_score_siege.get());
	UIGeode->removeDrawable(player_2.field.t_score.get());
	UIGeode->removeDrawable(player_2.field.t_score_melee.get());
	UIGeode->removeDrawable(player_2.field.t_score_archer.get());
	UIGeode->removeDrawable(player_2.field.t_score_siege.get());
	UIGeode->removeDrawable(player_2.t_handCount.get());
	UIGeode->removeDrawable(player_1.t_gamePoints.get());
	UIGeode->removeDrawable(player_2.t_gamePoints.get());
}

void Director::Init_Camera()
{
	//create a camera
	camera = new osg::Camera;
	//initialize
	camera -> setClearMask(GL_DEPTH_BUFFER_BIT);
	camera -> setRenderOrder(osg::Camera::POST_RENDER);
	camera -> setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	camera -> setProjectionMatrixAsFrustum(-9.6,9.6,-5.4,5.4,1,10.1);
	camera -> setViewMatrixAsLookAt(osg::Vec3(0.0,0.0,0.0), osg::Vec3(0.0,1.0,0.0), osg::Vec3(0.0,0.0,1.0));
}

Deck Director::GenerateDeck(int nums[], int length)
{
	//create a deck
	Deck temp_deck = Deck();
	//push cards into deck
	for (int i = 0; i < length; i++)
	{
		temp_deck.PushCard(Card(nums[i]));
	}
	//set the deck as p1's deck
	player_1.SetDeck(temp_deck);
	return temp_deck;
}
//deal cards, by card numbers
//didn't actually used this function in game
Hand Director::Deal(int nums[], int length)
{
	//sort
	for (int i = 0; i < length; i++)
	{
		for (int j = i + 1; j < length; j++)
		{
			if (nums[j] > nums[i])
			{
				int temp = nums[i];
				nums[i] = nums[j];
				nums[j] = temp;
			}
		}
	}
	for (int i = 0; i < length; i++)
	{
		player_1.PushHand(player_1.PopDeck(nums[i]));

		camera->addChild(player_1.hand.stack[i].pat.get());
	}
	return player_1.hand;
}
//deal one card, +1 overridden
//this function is used in the game
Hand Director::Deal(int num)
{
	player_1.PushHand(player_1.PopDeck(num));
	int last = player_1.hand.stack.size() - 1;
	camera->addChild(player_1.hand.stack[last].pat.get());
	return player_1.hand;
}
//indicate a card object and play it
//uses for doctor cards
void Director::toFieldFromCard(Card card)
{
	AssistantDirector::Send_msg(AssistantDirector::intToString(card.no));
	switch (card.genre)
	{
	case 0:
		if (card.skill != 2)
		{
			player_1.field.PushMelee(card);
			if (card.skill == 1)
			{
				//burn card
				BurnLine(&player_2, 0);
				AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(player_1.hand.stack.size()));
				if (!player_2.isGiveup)
					AssistantDirector::Send_msg("ready");
			}
			else if(card.skill == 4)
			{
				//doctor card
				player_1.garbage.ShowAll();
				//set controller
				isCuring = true;
			}
			else
			{
				AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(player_1.hand.stack.size()));
				if (!player_2.isGiveup)
					AssistantDirector::Send_msg("ready");
			}
		}
		else
		{
			player_2.field.PushMelee(card);
			for (int s = 0; s < 2 && !player_1.deck.stack.empty(); s++)
			{
				srand((unsigned)time(NULL)); 
				int n = player_1.deck.stack.size();
				int k = (rand() % (n-0))+ 0;
				Deal(k);
			}
			AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(player_1.hand.stack.size()));
			if (!player_2.isGiveup)
				AssistantDirector::Send_msg("ready");
		}
		break;
	case 1:
		if (card.skill != 2)
		{
			player_1.field.PushArcher(card);
			if (card.skill == 1)
			{
				BurnLine(&player_2, 1);
				AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(player_1.hand.stack.size()));
				if (!player_2.isGiveup)
					AssistantDirector::Send_msg("ready");
			}
			else if(card.skill == 4)
			{
				//doctor card
				player_1.garbage.ShowAll();
				//controller
				isCuring = true;
			}
			else
			{
				AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(player_1.hand.stack.size()));
				if (!player_2.isGiveup)
					AssistantDirector::Send_msg("ready");
			}
		}
		else
		{
			player_2.field.PushArcher(card);
			for (int s = 0; s < 2 && !player_1.deck.stack.empty(); s++)
			{
				srand((unsigned)time(NULL)); 
				int n = player_1.deck.stack.size();
				int k = (rand() % (n-0))+ 0;
				Deal(k);
			}
			AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(player_1.hand.stack.size()));
			if (!player_2.isGiveup)
				AssistantDirector::Send_msg("ready");
		}
		break;
	case 2:
		if (card.skill != 2)
		{
			player_1.field.PushSiege(card);
			if (card.skill == 1)
			{
				BurnLine(&player_2, 2);
				AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(player_1.hand.stack.size()));
				if (!player_2.isGiveup)
					AssistantDirector::Send_msg("ready");
			}
			else if(card.skill == 4)
			{
				//doctor
				player_1.garbage.ShowAll();
				//controller
				isCuring = true;
			}
			else
			{
				AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(player_1.hand.stack.size()));
				if (!player_2.isGiveup)
					AssistantDirector::Send_msg("ready");
			}
		}
		else
		{
			player_2.field.PushSiege(card);
			for (int s = 0; s < 2 && !player_1.deck.stack.empty(); s++)
			{
				srand((unsigned)time(NULL)); 
				int n = player_1.deck.stack.size();
				int k = (rand() % (n-0))+ 0;
				Deal(k);
			}
			AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(player_1.hand.stack.size()));
			if (!player_2.isGiveup)
				AssistantDirector::Send_msg("ready");
		}
		break;
	default:
		break;
	}
	player_1.field.CalculateScore();
	player_2.field.CalculateScore();
}
//P1 plays card
void Director::toField(Card card)
{
	if (StateMachine == 0)
	{
		if (!player_2.isGiveup)
		{
			StateMachine = 1;
		}
		
		//send (card.no) to the server
		AssistantDirector::Send_msg(AssistantDirector::intToString(card.no));
		for (int i = 0; i < player_1.hand.stack.size(); i++)
		{
			if (card.id == player_1.hand.stack[i].id)
			{
				player_1.hand.stack[i].isSelected = false;
				player_1.hand.stack[i].isPickerActive = false;

				switch (card.genre)
				{
				//0~2
				case 0:
					if (card.skill != 2)
					{
						player_1.field.PushMelee(player_1.hand.PopCard(i));
						if (card.skill == 1)
						{
							//burn
							BurnLine(&player_2, 0);
							AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(player_1.hand.stack.size()));
							if (!player_2.isGiveup)
								AssistantDirector::Send_msg("ready");
						}
						else if(card.skill == 4)
						{
							//doctor
							if (!player_1.garbage.stack.empty())
							{
								player_1.garbage.ShowAll();
								//controller
								isCuring = true;
							}
							else
							{
								AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(player_1.hand.stack.size()));
								if (!player_2.isGiveup)
									AssistantDirector::Send_msg("ready");
							}
						}
						else
						{
							AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(player_1.hand.stack.size()));
							if (!player_2.isGiveup)
								AssistantDirector::Send_msg("ready");
						}
					}
					else
					{
						player_2.field.PushMelee(player_1.hand.PopCard(i));
						for (int s = 0; s < 2 && !player_1.deck.stack.empty(); s++)
						{
							srand((unsigned)time(NULL)); 
							int n = player_1.deck.stack.size();
							int k = (rand() % (n-0))+ 0;
							Deal(k);
						}
						AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(player_1.hand.stack.size()));
						if (!player_2.isGiveup)
							AssistantDirector::Send_msg("ready");
					}
					break;
				case 1:
					if (card.skill != 2)
					{
						player_1.field.PushArcher(player_1.hand.PopCard(i));
						if (card.skill == 1)
						{
							BurnLine(&player_2, 1);
							AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(player_1.hand.stack.size()));
							if (!player_2.isGiveup)
								AssistantDirector::Send_msg("ready");
						}
						else if(card.skill == 4)
						{
							//doctor
							if (!player_1.garbage.stack.empty())
							{
								player_1.garbage.ShowAll();
								//controller
								isCuring = true;
							}
							else
							{
								AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(player_1.hand.stack.size()));
								if (!player_2.isGiveup)
									AssistantDirector::Send_msg("ready");
							}
						}
						else
						{
							AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(player_1.hand.stack.size()));
							if (!player_2.isGiveup)
								AssistantDirector::Send_msg("ready");
						}
					}
					else
					{
						player_2.field.PushArcher(player_1.hand.PopCard(i));
						for (int s = 0; s < 2 && !player_1.deck.stack.empty(); s++)
						{
							srand((unsigned)time(NULL)); 
							int n = player_1.deck.stack.size();
							int k = (rand() % (n-0))+ 0;
							Deal(k);
						}
						AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(player_1.hand.stack.size()));
						if (!player_2.isGiveup)
							AssistantDirector::Send_msg("ready");
					}
					break;
				case 2:
					if (card.skill != 2)
					{
						player_1.field.PushSiege(player_1.hand.PopCard(i));
						if (card.skill == 1)
						{
							BurnLine(&player_2, 2);
							AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(player_1.hand.stack.size()));
							if (!player_2.isGiveup)
								AssistantDirector::Send_msg("ready");
						}
						else if(card.skill == 4)
						{
							//doctor
							if (!player_1.garbage.stack.empty())
							{
								player_1.garbage.ShowAll();
								//controller
								isCuring = true;
							}
							else
							{
								AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(player_1.hand.stack.size()));
								if (!player_2.isGiveup)
									AssistantDirector::Send_msg("ready");
							}
						}
						else
						{
							AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(player_1.hand.stack.size()));
							if (!player_2.isGiveup)
								AssistantDirector::Send_msg("ready");
						}
					}
					else
					{
						player_2.field.PushSiege(player_1.hand.PopCard(i));
						for (int s = 0; s < 2 && !player_1.deck.stack.empty(); s++)
						{
							srand((unsigned)time(NULL)); 
							int n = player_1.deck.stack.size();
							int k = (rand() % (n-0))+ 0;
							Deal(k);
						}
						AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(player_1.hand.stack.size()));
						if (!player_2.isGiveup)
							AssistantDirector::Send_msg("ready");
					}
					break;
/******************************************************************************************************************************************************************************/
				//special cards 3
				case 3:
					//weather cards
					if (card.skill == 0)//sun
					{
						card.animator = 6;
						player_1.garbage.PushCard(player_1.hand.PopCard(i));
						
						while (!player_1.field.weather.stack.empty())
						{
							card.animator = 6;
							player_1.garbage.PushCard(player_1.field.weather.PopCard(0));
						}
						player_1.field.EraseWeather();
						player_1.field.isRain = false;
						player_1.field.isFog = false;
						player_1.field.isFrost = false;
						AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(player_1.hand.stack.size()));
						if (!player_2.isGiveup)
							AssistantDirector::Send_msg("ready");
						break;
					}
					if (card.skill <=3 && card.skill >= 1)//other weathers
					{
						//PushWeather, push the card to weather stack
						switch (card.skill)
						{
						case 1:
							if (player_2.field.isRain)
							{
								camera->removeChild(player_1.hand.PopCard(i).pat.get());
							}
							else
							{
								player_2.field.PushWeather(player_1.hand.PopCard(i));
							}
							break;
						case 2:
							if (player_2.field.isFog)
							{
								camera->removeChild(player_1.hand.PopCard(i).pat.get());
							}
							else
							{
								player_2.field.PushWeather(player_1.hand.PopCard(i));
							}
							break;
						case 3:
							if (player_2.field.isFrost)
							{
								camera->removeChild(player_1.hand.PopCard(i).pat.get());
							}
							else
							{
								player_2.field.PushWeather(player_1.hand.PopCard(i));
							}
							break;
						default:
							break;
						}
						AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(player_1.hand.stack.size()));
						if (!player_2.isGiveup)
							AssistantDirector::Send_msg("ready");
						break;
					}
					//burn
					if (card.skill == 7)
					{
						card.animator = 6;
						player_1.garbage.PushCard(player_1.hand.PopCard(i));
						Burn();
						AssistantDirector::Send_msg("hand=" + AssistantDirector::intToString(player_1.hand.stack.size()));
						if (!player_2.isGiveup)
							AssistantDirector::Send_msg("ready");
					}
					//horn
					if (card.skill == 8)
					{
						isHorn = true;

					}
					//scarecrow
					if (card.skill == 9)
					{
						isScarecrow = true;
					}
					break;
				default:
					break;
				}
			}
		}
		player_1.field.CalculateScore();
		player_2.field.CalculateScore();
	}
}
//+1 overridden P2 plays a card
void Director::toField(int num)
{
	Card card = Card(num);
	card.GeneratePATransform(osg::Vec3(0.0,11,0.0));
	camera->addChild(card.pat.get());

	card.isSelected = false;
	card.isPickerActive = false;
	switch (card.genre)
	{
	case 0:
		if (card.skill != 2)
		{
			player_2.field.PushMelee(card);
			if (card.skill == 1)
			{
				BurnLine(&player_1, 0);
			}
		}
		else
		{
			player_1.field.PushMelee(card);
		}
		break;
	case 1:
		if (card.skill != 2)
		{
			player_2.field.PushArcher(card);
			if (card.skill == 1)
			{
				BurnLine(&player_1, 1);
			}
		}
		else
		{
			player_1.field.PushArcher(card);
		}
		break;
	case 2:
		if (card.skill != 2)
		{
			player_2.field.PushSiege(card);
			if (card.skill == 1)
			{
				BurnLine(&player_1, 2);
			}
		}
		else
		{
			player_1.field.PushSiege(card);
		}
		break;
	case 3:
		//special cards
		//weather cards
		if (card.skill == 0)
		{
			//sun
			camera->removeChild(card.pat.get());
			for (int j = 0; j < player_2.field.weather.stack.size(); j++)
			{
				camera->removeChild(player_2.field.weather.stack[j].pat.get());
			}
			player_2.field.EraseWeather();
			player_2.field.isRain = false;
			player_2.field.isFog = false;
			player_2.field.isFrost = false;
			break;
		}
		if (card.skill <=3 && card.skill >= 1)
		{
			//push weather
			switch (card.skill)
			{
			case 1:
				if (player_2.field.isRain)
				{
					camera->removeChild(card.pat.get());
				}
				else
				{
					player_2.field.PushWeather(card);
				}
				break;
			case 2:
				if (player_2.field.isFog)
				{
					camera->removeChild(card.pat.get());
				}
				else
				{
					player_2.field.PushWeather(card);
				}
				break;
			case 3:
				if (player_2.field.isFrost)
				{
					camera->removeChild(card.pat.get());
				}
				else
				{
					player_2.field.PushWeather(card);
				}
				break;
			default:
				break;
			}
			break;
		}
		//burn
		if (card.skill == 7)
		{
			card.animator = 6;
			player_2.garbage.PushCard(card);
			Burn();
		}
		//horn
		if (card.skill == 8)
		{
			switch (p2Horn)
			{
			case 0:
				card.direction = osg::Vec3(player_2.field.v3Melee.x() + AssistantDirector::hornX,
											player_2.field.v3Melee.y(),
											player_2.field.v3Melee.z());
				card.animator = 5;
				player_2.field.meleeHorn.PushCard(card);
				break;
			case 1:
				card.direction = osg::Vec3(player_2.field.v3Archer.x() + AssistantDirector::hornX,
											player_2.field.v3Archer.y(),
											player_2.field.v3Archer.z());
				card.animator = 5;
				player_2.field.archerHorn.PushCard(card);
				break;
			case 2:
				card.direction = osg::Vec3(player_2.field.v3Siege.x() + AssistantDirector::hornX,
									player_2.field.v3Siege.y(),
									player_2.field.v3Siege.z());
				card.animator = 5;
				player_2.field.siegeHorn.PushCard(card);
				break;
			default:
				break;
			}
			p2Horn = -1;
			break;
		}
		//sacrecrow
		if (card.skill == 9)
		{
			switch (p2ScarecrowLine)
			{
			case 0:
				for (int si = 0; si < player_2.field.melee.stack.size(); si++)
				{
					if (player_2.field.melee.stack[si].no == p2ScarecrowNo)
					{
						player_2.PushGarbage(player_2.field.PopMelee(si));
						player_2.field.PushMelee(card);
						break;
					}
				}
				break;
			case 1:
				for (int si = 0; si < player_2.field.archer.stack.size(); si++)
				{
					if (player_2.field.archer.stack[si].no == p2ScarecrowNo)
					{
						player_2.PushGarbage(player_2.field.PopArcher(si));
						player_2.field.PushArcher(card);
						break;
					}
				}
				break;
			case 2:
				for (int si = 0; si < player_2.field.siege.stack.size(); si++)
				{
					if (player_2.field.siege.stack[si].no == p2ScarecrowNo)
					{
						player_2.PushGarbage(player_2.field.PopSiege(si));
						player_2.field.PushSiege(card);
						break;
					}
				}
				break;
			default:
				break;
			}
			break;
		}
		break;
	default:
		break;
	}


	player_1.field.CalculateScore();
	player_2.field.CalculateScore();
}

void Director::CleanUp(Player *player)
{
	while (!player->field.melee.stack.empty())
	{
		if (!player->field.melee.stack[0].isHero && player->field.melee.stack[0].genre != 3)
		{
			player->garbage.PushCard(player->field.melee.PopCard(0));
		}
		else
		{
			camera->removeChild(player->field.melee.PopCard(0).pat.get());
		}
	}
	while (!player->field.archer.stack.empty())
	{
		if (!player->field.archer.stack[0].isHero && player->field.archer.stack[0].genre != 3)
		{
			player->garbage.PushCard(player->field.archer.PopCard(0));
		}
		else
		{
			camera->removeChild(player->field.archer.PopCard(0).pat.get());
		}
	}
	while (!player->field.siege.stack.empty())
	{
		if (!player->field.siege.stack[0].isHero && player->field.siege.stack[0].genre != 3)
		{
			player->garbage.PushCard(player->field.siege.PopCard(0));
		}
		else
		{
			camera->removeChild(player->field.siege.PopCard(0).pat.get());
		}
	}
	//clean weather stack
	if (!player->field.weather.stack.empty())
	{
		//same as play the sun card
		//rule: weather cards don't need to place in garbage stack
		for (int j = 0; j < player->field.weather.stack.size(); j++)
		{
			camera->removeChild(player->field.weather.stack[j].pat.get());
		}
		player->field.EraseWeather();
		player->field.isRain = false;
		player->field.isFog = false;
		player->field.isFrost = false;
	}
	if (!player->field.meleeHorn.stack.empty())
	{
		for (int j = 0; j < player->field.meleeHorn.stack.size(); j++)
		{
			camera->removeChild(player->field.meleeHorn.stack[j].pat.get());
		}
		player->field.meleeHorn.stack.clear();
	}
	if (!player->field.archerHorn.stack.empty())
	{
		for (int j = 0; j < player->field.archerHorn.stack.size(); j++)
		{
			camera->removeChild(player->field.archerHorn.stack[j].pat.get());
		}
		player->field.archerHorn.stack.clear();
	}
	if (!player->field.siegeHorn.stack.empty())
	{
		for (int j = 0; j < player->field.siegeHorn.stack.size(); j++)
		{
			camera->removeChild(player->field.siegeHorn.stack[j].pat.get());
		}
		player->field.siegeHorn.stack.clear();
	}
	//recalculate the score
	player->field.CalculateScore();
}

void Director::BurnLine(Player *player, int line)
{
	//search for the card acquires the highest atk point in the line
	if (player->field.score_melee >= 10 && line == 0)
	{
		int atk = -1;
		for (int i = 0; i < player->field.melee.stack.size(); i++)
		{
			if (atk < player->field.melee.stack[i].atk && !player->field.melee.stack[i].isHero)
			{
				atk = player->field.melee.stack[i].atk;
			}
		}
		for (int i = player->field.melee.stack.size() - 1; i >= 0; i--)
		{
			if (player->field.melee.stack[i].atk == atk && !player->field.melee.stack[i].isHero)
			{
				player->garbage.PushCard(player->field.PopMelee(i));
			}
		}
	}
	else if (player->field.score_archer >= 10 && line == 1)
	{
		int atk = -1;
		for (int i = 0; i < player->field.archer.stack.size(); i++)
		{
			if (atk < player->field.archer.stack[i].atk && !player->field.archer.stack[i].isHero)
			{
				atk = player->field.archer.stack[i].atk;
			}
		}
		for (int i = player->field.archer.stack.size() - 1; i >= 0; i--)
		{
			if (player->field.archer.stack[i].atk == atk && !player->field.archer.stack[i].isHero)
			{
				player->garbage.PushCard(player->field.PopArcher(i));
			}
		}
	}
	else if (player->field.score_siege >= 10 && line == 2)
	{
		int atk = -1;
		for (int i = 0; i < player->field.siege.stack.size(); i++)
		{
			if (atk < player->field.siege.stack[i].atk && !player->field.siege.stack[i].isHero)
			{
				atk = player->field.siege.stack[i].atk;
			}
		}
		for (int i = player->field.siege.stack.size() - 1; i >= 0; i--)
		{
			if (player->field.siege.stack[i].atk == atk && !player->field.siege.stack[i].isHero)
			{
				player->garbage.PushCard(player->field.PopSiege(i));
			}
		}
	}
}

void Director::Burn()
{
	//the highest atk point
	int atk = -1;
	//search for the card acquires the highest atk point on the field
	for (int i = 0; i < player_1.field.melee.stack.size(); i++)
	{
		if (atk < player_1.field.melee.stack[i].atk && !player_1.field.melee.stack[i].isHero)
		{
			atk = player_1.field.melee.stack[i].atk;
		}
	}
	for (int i = 0; i < player_1.field.archer.stack.size(); i++)
	{
		if (atk < player_1.field.archer.stack[i].atk && !player_1.field.archer.stack[i].isHero)
		{
			atk = player_1.field.archer.stack[i].atk;
		}
	}
	for (int i = 0; i < player_1.field.siege.stack.size(); i++)
	{
		if (atk < player_1.field.siege.stack[i].atk && !player_1.field.siege.stack[i].isHero)
		{
			atk = player_1.field.siege.stack[i].atk;
		}
	}
	
	for (int i = 0; i < player_2.field.melee.stack.size(); i++)
	{
		if (atk < player_2.field.melee.stack[i].atk && !player_2.field.melee.stack[i].isHero)
		{
			atk = player_2.field.melee.stack[i].atk;
		}
	}
	for (int i = 0; i < player_2.field.archer.stack.size(); i++)
	{
		if (atk < player_2.field.archer.stack[i].atk && !player_2.field.archer.stack[i].isHero)
		{
			atk = player_2.field.archer.stack[i].atk;
		}
	}
	for (int i = 0; i < player_2.field.siege.stack.size(); i++)
	{
		if (atk < player_2.field.siege.stack[i].atk && !player_2.field.siege.stack[i].isHero)
		{
			atk = player_2.field.siege.stack[i].atk;
		}
	}
	//*********************************************************************************************
	//discard all the cards on the field that acquire the atk point 
	for (int i = player_1.field.melee.stack.size() - 1; i >= 0; i--)
	{
		if (atk == player_1.field.melee.stack[i].atk && !player_1.field.melee.stack[i].isHero)
		{
			player_1.garbage.PushCard(player_1.field.PopMelee(i));
		}
	}
	for (int i = player_1.field.archer.stack.size() - 1; i >= 0; i--)
	{
		if (atk == player_1.field.archer.stack[i].atk && !player_1.field.archer.stack[i].isHero)
		{
			player_1.garbage.PushCard(player_1.field.PopArcher(i));
		}
	}
	for (int i = player_1.field.siege.stack.size() - 1; i >= 0; i--)
	{
		if (atk == player_1.field.siege.stack[i].atk && !player_1.field.siege.stack[i].isHero)
		{
			player_1.garbage.PushCard(player_1.field.PopSiege(i));
		}
	}
	
	for (int i = player_2.field.melee.stack.size() - 1; i >= 0; i--)
	{
		if (atk == player_2.field.melee.stack[i].atk && !player_2.field.melee.stack[i].isHero)
		{
			player_2.garbage.PushCard(player_2.field.PopMelee(i));
		}
	}
	for (int i = player_2.field.archer.stack.size() - 1; i >= 0; i--)
	{
		if (atk == player_2.field.archer.stack[i].atk && !player_2.field.archer.stack[i].isHero)
		{
			player_2.garbage.PushCard(player_2.field.PopArcher(i));
		}
	}
	for (int i = player_2.field.siege.stack.size() - 1; i >= 0; i--)
	{
		if (atk == player_2.field.siege.stack[i].atk && !player_2.field.siege.stack[i].isHero)
		{
			player_2.garbage.PushCard(player_2.field.PopSiege(i));
		}
	}
}
