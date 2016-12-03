#include "AssistantDirector.h"

SOCKET AssistantDirector::sockets = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
int AssistantDirector::currentID = -1;

int AssistantDirector::cards_of_deck[37] = {30000,31000,32000,33000,37000,38000,39000,
											69,1079,2059,2059,2049,5049,5049,5049,
											15059,15059,15059,15059,10049,10058,10059,10069,
											22019,24059,24059,26019,26019,26019,20069,20069,25089,
											70,150,151,2000,14070};
int AssistantDirector::deck_amount = 37;

osg::Vec3 AssistantDirector::v3RowLenth = osg::Vec3(78.0,0.0,0.0);

osg::Vec3 AssistantDirector::v3Hand = osg::Vec3(-35.2,9.99,-36.5);
osg::Vec3 AssistantDirector::v3Melee = osg::Vec3(-22.0,9.99,5.0);
osg::Vec3 AssistantDirector::v3Archer = osg::Vec3(-22.0,9.99,-8.5);
osg::Vec3 AssistantDirector::v3Siege = osg::Vec3(-22.0,9.99,-22.0);
osg::Vec3 AssistantDirector::v3Score = osg::Vec3(-50.8,9.99,-19.4);

osg::Vec3 AssistantDirector::v3Melee_2 = osg::Vec3(-22.0,9.99,19.6);
osg::Vec3 AssistantDirector::v3Archer_2 = osg::Vec3(-22.0,9.99,33.1);
osg::Vec3 AssistantDirector::v3Siege_2 = osg::Vec3(-22.0,9.99,46.6);
osg::Vec3 AssistantDirector::v3Score_2 = osg::Vec3(-50.8,9.99,20.8);

osg::Vec3 AssistantDirector::v3Weather = osg::Vec3(-78.6,9.99,2.6);
osg::Vec3 AssistantDirector::v3Focused = osg::Vec3(15.5,2.2,0.5);

osg::Vec3 AssistantDirector::v3Garbaged = osg::Vec3(64.0,10.0,-36.2);
osg::Vec3 AssistantDirector::v3Garbaged_2 = osg::Vec3(64.0,10.0,39.3);

osg::Vec3 AssistantDirector::v3HandCount = osg::Vec3(-68.0,9.99,-18.0);
osg::Vec3 AssistantDirector::v3HandCount_2 = osg::Vec3(-68.0,9.99,15.5);

osg::Vec3 AssistantDirector::v3GarbageFocused = osg::Vec3(-32.5,4.4,13.0);

float AssistantDirector::hornX = -11.0;
float AssistantDirector::xmin = 378;
float AssistantDirector::xmax = 1012;
float AssistantDirector::y[4] = {172, 264, 354, 442};

float AssistantDirector::row_text_x = -22.2;
float AssistantDirector::row_text_z = -1.2;

int AssistantDirector::hornBuf = 2;

int AssistantDirector::ReceivesShadowTraversalMask = 0x1;
int AssistantDirector::CastsShadowTraversalMask = 0x2;

char* AssistantDirector::stringToChar(std::string str)
{	
	const char *buf = str.c_str();
	char *s = new char[strlen(buf) + 1];
	strcpy(s,buf);
	
	return s;
}

void AssistantDirector::setSocket(SOCKET s)
{
	sockets = s;
}

void AssistantDirector::Recv_msg(std::string &str)
{
	char buf[MAX];
	recv(sockets, buf, sizeof(buf), 0);
	str = buf;
}

void AssistantDirector::Recv_msg(char* str)
{
	recv(sockets, str, sizeof(str), 0);
}

bool AssistantDirector::Send_msg(std::string str)
{
	char buf[MAX];
	for(int i=0;i<str.size();i++){
		buf[i]=str[i];
	}
	buf[str.size()]='\0';
	send(sockets,buf,sizeof(buf),0);
	printf("send:%s\n",buf);
	return true;
}

bool AssistantDirector::Send_msg(char* str)
{
	send(sockets,str,sizeof(str),0);
	printf("send:%s\n",str);
	return true;
}

int AssistantDirector::GenerateID()
{
	return ++currentID;
}

osg::PositionAttitudeTransform* AssistantDirector::createLightSource(unsigned int num,const osg::Vec3& trans,const osg::Vec4& color)
{
	 osg::ref_ptr<osg::Light> light = new osg::Light;
	 light->setLightNum( num );  
	 light->setAmbient(osg::Vec4(0.75,0.75,0.75,1.0));
	 light->setDiffuse( color ); 
	 light->setSpecular(color);
	 light->setPosition( osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f) ); 
	 light->setDirection(osg::Vec3(0.0,10.0,0.0));

	 osg::ref_ptr<osg::LightSource> lightSource = new osg::LightSource; 
	 lightSource->setLight( light );
	 osg::ref_ptr<osg::PositionAttitudeTransform> sourceTrans = new osg::PositionAttitudeTransform;
	 sourceTrans->setPosition(trans);
	 sourceTrans->addChild( lightSource.get() );
	 return sourceTrans.release(); 
}

osg::ref_ptr<osg::Geode> AssistantDirector::createBox(osg::Vec3 center, float x, float y, float z)  
{
	
	osg::ref_ptr<osg::Box> boxShape = new osg::Box(center, x, y, z);
	boxShape->setDataVariance(osg::Object::DYNAMIC);
	osg::ref_ptr<osg::ShapeDrawable> BoxShapeDrawable = new osg::ShapeDrawable(boxShape.get());
	BoxShapeDrawable->setDataVariance(osg::Object::DYNAMIC);
	osg::ref_ptr<osg::Geode> boxGeode = new osg::Geode;
	boxGeode->addDrawable(BoxShapeDrawable.get());
	boxGeode->setDataVariance(osg::Object::DYNAMIC);
	return boxGeode;
}

osg::ref_ptr<osgText::Text> AssistantDirector::createText(const osg::Vec3& pos,const std::string& content,float size)
{
	osg::ref_ptr<osgText::Text> text = new osgText::Text;
	text->setCharacterSize( size );   
	text->setAxisAlignment( osgText::TextBase::XZ_PLANE );  
	text->setPosition( pos );  
	text->setText( content ); 
	text->setDataVariance(osg::Object::DYNAMIC);
	return text.release();
}

std::string AssistantDirector::intToString (int n)
{
	std::stringstream sstream;
    std::string str;
	sstream<<n;
	sstream>>str;
	return str;
}