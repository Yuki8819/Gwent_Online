#include "Card.h"

float Card::x = 6.5;
float Card::y = 0.001;
float Card::z = 13.0;


Card::Card()		//This is useless
{
	id = -1;
	atk = atk_original = atk_weathered = 0;
	animator = 0;
	direction = osg::Vec3(0.0,0.0,0.0);
}

Card::Card(int num)
{
	Init(num);
	animator = 0;
	direction = osg::Vec3(0.0,0.0,0.0);
}

int Card::Init(int num)
{
	//Generate the texture file path
    std::string str = AssistantDirector::intToString(num);
	
	while (str.length() < 5)
	{
		str = "0" + str;
	}
	tex_path = "Gwent\\2.0\\" + str + ".jpg";
	//Bind Texture
	boxTexture = new osg::Texture2D;  
    boxTexture->setDataVariance(osg::Object::DYNAMIC);  
	boxTexture->setFilter(osg::Texture::FilterParameter::MIN_FILTER,osg::Texture::FilterMode::LINEAR_MIPMAP_LINEAR);
	boxTexture->setFilter(osg::Texture::FilterParameter::MAG_FILTER,osg::Texture::FilterMode::LINEAR_MIPMAP_LINEAR);
    boxImage = osgDB::readImageFile(tex_path);  
	if (!boxImage.get())  
    {
		std::cout<<"Failed to open the file! path:"<<tex_path<<std::endl;  
		system("PAUSE");
        //return -1;  
    }
	else
	{
		std::cout<<"Loaded path:"<<tex_path<<std::endl;
		boxTexture->setImage(boxImage.get()); 
	}
	//Create function
	return CreateLogic(num);
}

int Card::CreateLogic(int num)
{
	/*
	using a 5-digit code to define the card
	1st digit (genere): 0 - melee,1 - archer,2 - siege,3 - function

	(if 1st digit is 0~2)
	2nd digit (skill):  0 - null,1 - burn,2 - spy,3 - special horn,4 - doctor,
						5 - brothers,6 - horn,7 - summon
	3rd & 4th digit: the atk value 
	5th digit: characteristic number,hero from 0,others from 9 

	(if 1st digit is 3)
	2nd: 0 - sun,1 - rain,2 - fog,3 - frost,9 - scarecrow,8 - horn,7 - burn
	others:0 - null
	*/
	isHero = false;

	isPickerActive = false;

	isSelected = false;

	isFielded = false;

	isGarbaged = false;

	int hero = 0;

	no = num;

	hero = num % 10;
	if(hero < 5)
	{
		isHero = true;
	}

	num = num / 10;
	atk = num % 100;
	
	atk_original = atk;
	atk_weathered = atk;

	num = num / 100;
	skill = num % 10;
		
	num = num / 10;
	genre = num;

	return id = AssistantDirector::GenerateID();
}

osg::ref_ptr<osg::PositionAttitudeTransform> Card::GeneratePATransform(osg::Vec3 center)
{
	v3Return = center;
	boxShape = new osg::Box(osg::Vec3(0.0,0.0,0.0), x, y, z);
	boxShape->setDataVariance(osg::Object::DYNAMIC);
	boxShapeDrawable = new osg::ShapeDrawable(boxShape.get());
	boxShapeDrawable->setDataVariance(osg::Object::DYNAMIC);
	//boxShapeDrawable->setUpdateCallback(new DynamicCardCallback);
	boxGeode = new osg::Geode;
	boxGeode->addDrawable(boxShapeDrawable.get());
	//Create a TexEnv instance for binding the StateSet  
    
	/*
	boxTexEnv = new osg::TexEnv;  
    boxTexEnv->setMode(osg::TexEnv::DECAL);
    boxStateSet = new osg::StateSet;  
    boxStateSet->setAttribute(boxTexEnv.get());  
    boxStateSet->setAttributeAndModes(boxTexture.get());  
    boxGeode->setStateSet(boxStateSet.get());
	*/


	material = new osg::Material();

	material->setDiffuse(osg::Material::FRONT,  osg::Vec4(1.0, 1.0, 1.0, 1.0));

	material->setSpecular(osg::Material::FRONT, osg::Vec4(0.0, 0.0, 0.0, 1.0));

	material->setAmbient(osg::Material::FRONT,  osg::Vec4(0.3, 0.3, 0.3, 1.0));

	material->setEmission(osg::Material::FRONT, osg::Vec4(0.0, 0.0, 0.0, 1.0));

	material->setShininess(osg::Material::FRONT, 25.0);


	boxGeode->getOrCreateStateSet()->setTextureAttributeAndModes(0,boxTexture.get());
	boxGeode->getOrCreateStateSet()->setAttribute(material);

	//boxGeode->setNodeMask(AssistantDirector::CastsShadowTraversalMask);

	//mt = new osg::MatrixTransform();
	//osg::Matrix mx;
	//mx = osg ::Matrix::translate(osg::Vec3(0.0,0.0,0.0));
	//mt->addChild(boxGeode.get());

	pat = new osg::PositionAttitudeTransform;
	pat->setPosition(center);
	pat->addChild(boxGeode.get());
	//pat->addChild(mt.get());
	//mt_r = *mt;
	//return boxGeode;
	return pat;
}

void Card::SetCenter(osg::Vec3 &center)
{
	boxShape->setCenter(center);
}