#include "DataReceiverThread.h"

void DataReceiverThread::setSocket(SOCKET s)
{
	sockets = s;
}

void DataReceiverThread::cleanSocket()
{
	closesocket(sockets);
}

int DataReceiverThread::cancel() 
{   
	_done = true; 
	while( isRunning() )
		YieldCurrentThread();  
	return 0; 
} 

void DataReceiverThread::run() 
{
	_done = false;  
	_dirty = true;  
	
	do   
	{
		YieldCurrentThread();    
		char buf[MAX];
		recv(sockets,buf,sizeof(buf),0);
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex); 
		_content=buf;
		_dirty=true;	
		printf("%s\n",buf);
	} while( !_done );
} 

bool DataReceiverThread::getContent( std::string& str ) 
{   
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex); 
	if ( _dirty )  
	{
		 str = _content;   		
		 std::cout<<_content<<"dirty:\n";
		 _content = "";
		 _dirty = false;  

		 return true;  
	} 
	return false;
} 