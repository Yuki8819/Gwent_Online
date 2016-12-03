//Class for the Receiving Thread, this thread takes charge of receiving messages from the server

#pragma once
#include "AssistantDirector.h"
#include <windows.h>

#include <osg/Geode>
#include <osgDB/ReadFile> 
#include <osgText/Text>
#include <osgViewer/Viewer> 
#include <iostream>
class DataReceiverThread : public OpenThreads::Thread 
{
public:   
	//get the thread instance
	static DataReceiverThread* instance() 
	{      
		static DataReceiverThread s_thread;  
		return &s_thread;  
	}
	virtual int cancel(); 
	virtual void run();    
	//set socket for receiving thread
	void setSocket(SOCKET s);
	//clean socket
	void cleanSocket();
	//get content string
	bool getContent( std::string& str );  
	
protected:  
	//mutex lock
	OpenThreads::Mutex _mutex; 
	//the content
	std::string _content;
	//varities for mutex controlling
	bool _done;  
	bool _dirty; 
	SOCKET sockets;
};