#include<stdio.h>
#include<winsock2.h>
#pragma comment (lib,"ws2_32.lib")
/////////////宏定义//////////////////////////////
#define		SERVPORT		5050		//定义端口号
#define		MAX				100
#define		MAXPLAYER		4
#define		MAXROOM			MAXPLAYER/2
#define		ROOMFULL		2
#define		AVAILABLE		1
#define		NOROOM			-1
#define		CONNECTSUCCESS	0X1
#define		SERVERFULL		0X2
#define		SERVERMSG		0X3
#define		ROOMSELECT		0X4
#define		RESELECT		0X5
#define		WAITSTART		0X6
#define		WAITANOTHER		0X7
#define		READY			"ready"
#define		LATER			"later"
#define		GIVEUP			"giveup"
#define		END				"end"
////////////////定义全局变量////////////////////////
int available[MAXPLAYER]={1,1,1,1};
int room[2]={0,0};
int roomnum[MAXPLAYER]={NOROOM,NOROOM,NOROOM,NOROOM};
SOCKET	sClient[MAXPLAYER];		//连接端口
////////////////判断是否有可用端口////////////////////////////
bool Available(int available[MAXPLAYER]){
	for(int i=0;i<MAXPLAYER;i++){
		if(available[i])
			return true;
	}
	return false;
}
////////////////判断消息内容函数/////////////////////
int	judge(char c[]){
	if(strcmp(c,READY)==0){
		return 1;
	}else if(strcmp(c,GIVEUP)==0){
		return 2;
	}
	return 0;
}
/////////////////异常处理函数////////////////////////
void Disconnection(int playerone,int playertwo,int roomindex){
	char buf[MAX];
	////////////playerone端口连接断开////////////////
	printf("端口 %d 已断开连接\n",playerone+1);
	closesocket(sClient[playerone]);
	available[playerone]=AVAILABLE;
	roomnum[playerone]=NOROOM;
	////////////向playertwo端口发送信息，本剧结束////
	sprintf_s(buf,"%s",END);
	if(send(sClient[playertwo],buf,sizeof(buf),0)==SOCKET_ERROR){
		//////////////playertwo端口连接断开//////////
		printf("端口 %d 已断开连接\n",playertwo+1);
		closesocket(sClient[playertwo]);
		available[playertwo]=AVAILABLE;
	}
	roomnum[playertwo]=NOROOM;
	room[roomindex]=0;
}
/////////////////游戏结束设置////////////////////////
void endonegame(int playerone,int playertwo,int roomindex){
	printf("房间 %d 对局结束\n",roomindex+1);
	printf("端口 %d 已断开连接\n",playerone+1);
	printf("端口 %d 已断开连接\n",playertwo+1);	
	available[playerone]=AVAILABLE;
	available[playertwo]=AVAILABLE;
	roomnum[playerone]=NOROOM;
	roomnum[playertwo]=NOROOM;
	room[roomindex]=0;
}
////////////////接受连接线程函数/////////////////////
void	AcceptProcess(LPVOID s){
	SOCKET sListen=*(SOCKET*)s;
	SOCKADDR_IN		clientAddr;		//客户端地址信息
	int				clientAddrLen;	//地质结构的长度
	char			buf[MAX];		//通信帧
	SOCKET			Sparesocket;	//备用连接端口
	clientAddrLen=sizeof(SOCKADDR);
	while(1){
		/////////////有可用端口////////////////////////////////
		if(Available(available)){
			int i=0;
			while(!available[i])	i++;
			sClient[i]=accept(sListen,(SOCKADDR *)&clientAddr, &clientAddrLen);
			available[i]=0;
			buf[0]=CONNECTSUCCESS;
			buf[1]='\0';
			send(sClient[i],buf,sizeof(buf),0);
			printf("端口 %d 已连入.\n",i+1);
		}
		////////////服务期满，启用备用端口连接////////////////
		else{
			Sparesocket=accept(sListen,(SOCKADDR *)&clientAddr, &clientAddrLen);
			buf[0]=SERVERFULL;
			buf[1]='\0';
			send(Sparesocket,buf,sizeof(buf),0);
			////////////重置备用端口/////////////////////////
			closesocket(Sparesocket);
		}
	}
}
/////////////选择房间线程函数组/////////////////////////////////
/////////////端口////////////////
void	Seat(LPVOID	s){
	int		seatnum=*(int*)(s);
	bool	reselect=false;
	char	buf[MAX];
	while(1){
		/////////端口1尚未连接则等待//////////
		while(available[seatnum]){
			Sleep(1000);
		}
		/////////端口1连接成功后开启选择房间//////////
		if(roomnum[seatnum]==NOROOM){
			printf("等待 %d 端口选择房间\n",seatnum+1);
			////////////发送服务器信息//////////////
			buf[0]=SERVERMSG;
			for(int i=0;i<MAXROOM;i++){
				buf[i+1]=room[i];
			}
			buf[MAXROOM+1]='\0';
			////////////若发送失败//////////////////
			if(send(sClient[seatnum],buf,4,0)==SOCKET_ERROR){
				printf("端口 %d 连接断开\n",seatnum+1);
				closesocket(sClient[seatnum]);
				available[seatnum]=AVAILABLE;		
				continue;			//连接异常，中断连接，跳出本次循环
			}
			////////////接受客户端选择信息/////////
			if(recv(sClient[seatnum],buf,sizeof(buf),0)!=SOCKET_ERROR){
				///////////接收到选择信息//////////////
				if(buf[0]==ROOMSELECT){
					int n=(int)buf[1];
					////////////若所选房间已满//////
					if(room[n-1]==ROOMFULL){
						buf[0]=RESELECT;
						buf[1]='\0';
						reselect=true;
						printf("端口 %d 所选房间人数已满，断开其连接\n",seatnum+1);
					}else{
						/////////所选房间未满//////
						roomnum[seatnum]=n-1;
						room[n-1]++;
						printf("端口 %d 选择房间 %d\n房间 %d 现有 %d 名玩家\n",seatnum+1,n,n,room[n-1]);
						if(room[n-1]==ROOMFULL){
							/////////加入后房间满////////
							buf[0]=WAITSTART;
							buf[1]='\0';
						}else{
							/////////加入后房间还没满///
							buf[0]=WAITANOTHER;
							buf[1]='\0';
						}
					}
					////////////发送判断结果////////////////////////////
					if(send(sClient[seatnum],buf,sizeof(buf),0)==SOCKET_ERROR){
						///////////若发送失败//////////////
						printf("端口 %d 连接断开\n",seatnum+1);
						closesocket(sClient[seatnum]);
						available[seatnum]=AVAILABLE;		
						continue;			//连接异常，中断连接，跳出本次循环
					}
					if(reselect){
						closesocket(sClient[seatnum]);
						available[seatnum]=AVAILABLE;		
						reselect=false;
						continue;			//连接异常，中断连接，跳出本次循环
					}
				}else{	
					///////接收到的不是选择信息//////////////
					printf("端口 %d 连接断开\n",seatnum+1);
					closesocket(sClient[seatnum]);
					available[seatnum]=AVAILABLE;
					continue;		//连接异常，中断连接，跳出本次循环		
				}
			}else{
				/////////接受信息失败/////////////////////
				printf("端口 %d 连接断开\n",seatnum+1);
				closesocket(sClient[seatnum]);
				available[seatnum]=AVAILABLE;
				continue;			//连接异常，中断连接，跳出本次循环
			}
		}
		////////等待端口一断开连接//////////
		while(!available[seatnum]){
			Sleep(1000);
		}
	}
}
/////////////游戏运行线程////////////////////////////////////////
//////////房间////////////////
void	Room(LPVOID s){
	int roomindex=*(int*)(s);
	char	buf[MAX],message[2][MAX];
	int		playerone,playertwo;
	while(1){
		////////////等待房间人满///////////
		while(room[roomindex]!=ROOMFULL){
			Sleep(1000);
		}
		printf("房间 %d 对局开始\n",roomindex+1);
		Sleep(1000);		//等待清空消息队列
		////////////确定两名玩家端口///////
		int i=0;
		for(;i<MAXPLAYER;i++){
			if(roomnum[i]==roomindex){
				playerone=i;
				break;
			}
		}
		for(i=i+1;i<MAXPLAYER;i++){
			if(roomnum[i]==roomindex){
				playertwo=i;
				break;
			}
		}
		///////////////设置游戏运行控制参数//////////////
		bool	Giveupone,Giveuptwo;		//判断是否放弃本局
		int		n;							//判断消息类型
		int     Scoreone,Scoretwo;			//两人每局点数
		int     Score=0;					//比分
		bool    first=false;				//用于循环中判断先手
		bool    disconnect=false;			//断线处理
		////////////////三居游戏主循环////////////////////
		for(int i=0;i<3;i++){
			///////////////向后手方发送后手消息////////////////////////////////
			sprintf_s(buf,"%s",LATER);
			if(send(sClient[playertwo],buf,sizeof(buf),0)==SOCKET_ERROR){
				////////////异常处理////////////////
				Disconnection(playertwo,playerone,roomindex);
				break;
			}
			/////////////////向先手方发送先手消息/////////////////////////////
			sprintf_s(buf,"%s",READY);
			if(send(sClient[playerone],buf,sizeof(buf),0)==SOCKET_ERROR){
				//////////////////异常处理////////////////////
				Disconnection(playerone,playertwo,roomindex);
				break;
			}
			Giveupone=false;
			Giveuptwo=false;
			first=true;
			while(!(Giveupone&&Giveuptwo)){
				/////////当先手没放弃本局，则可以出牌////////////
				while((!Giveupone)&&first){
					if(recv(sClient[playerone],buf,sizeof(buf),0)==SOCKET_ERROR){
				////////////playerone端口连接断开////////////////
						Disconnection(playerone,playertwo,roomindex);
						disconnect=true;
						break;
					}
					if(strcmp(buf,"38000")==0||strcmp(buf,"39000")==0){
						char assbuf[MAX];
						if(recv(sClient[playerone],assbuf,sizeof(assbuf),0)==SOCKET_ERROR){
						////////////playerone端口连接断开////////////////
							Disconnection(playerone,playertwo,roomindex);
							disconnect=true;
							break;
						}
						strcat(buf,assbuf);
					}
					if(send(sClient[playertwo],buf,sizeof(buf),0)==SOCKET_ERROR){
						////////////playertwo端口连接断开////////////////
						Disconnection(playertwo,playerone,roomindex);
						disconnect=true;
						break;
					}
					printf("%s\n", buf);
					n=judge(buf);
					if(n==1){
						break;
					}else if(n==2){
						Giveupone=true;
						if(recv(sClient[playerone],buf,sizeof(buf),0)==SOCKET_ERROR){
					////////////playerone端口连接断开////////////////
						Disconnection(playerone,playertwo,roomindex);
						disconnect=true;
						break;
						}
						sscanf_s(buf,"%d:%d",&Scoreone,&Scoretwo);
						break;
					}
					Sleep(1000);
				}
				if(disconnect)	break;
				////////后手没放弃本局，后手可以出牌/////////////
				while(!Giveuptwo){
					if(!first)	first=true;
					if(recv(sClient[playertwo],buf,sizeof(buf),0)==SOCKET_ERROR){
						////////////playertwo端口连接断开////////////////
						Disconnection(playertwo,playerone,roomindex);
						disconnect=true;
						break;
					}
					if(strcmp(buf,"38000")==0||strcmp(buf,"39000")==0){
						char assbuf[MAX];
						if(recv(sClient[playertwo],assbuf,sizeof(assbuf),0)==SOCKET_ERROR){
							////////////playertwo端口连接断开////////////////
							Disconnection(playertwo,playerone,roomindex);
							disconnect=true;
							break;
						}
						strcat(buf,assbuf);
					}
					if(send(sClient[playerone],buf,sizeof(buf),0)==SOCKET_ERROR){
					////////////playerone端口连接断开////////////////
						Disconnection(playerone,playertwo,roomindex);
						disconnect=true;
						break;
					}
					printf("%s\n", buf);
					n=judge(buf);
					if(n==1){
						break;
					}else if(n==2){
						Giveuptwo=true;
						if(recv(sClient[playertwo],buf,sizeof(buf),0)==SOCKET_ERROR){
							////////////playertwo端口连接断开////////////////
							Disconnection(playertwo,playerone,roomindex);
							disconnect=true;
							break;
						}
						sscanf_s(buf,"%d:%d",&Scoretwo,&Scoreone);
						break;
					}
					Sleep(1000);
				}
				if(disconnect)	break;
			}
			///////////////判断局分///////////////
			Sleep(1000);
			if(Scoreone>Scoretwo){
				Score++;
				first=true;
				sprintf_s(message[playerone],"%s",READY);
				sprintf_s(message[playertwo],"%s",LATER);
			}else if(Scoreone<Scoretwo){
				Score--;
				first=false;
				sprintf_s(message[playerone],"%s",LATER);
				sprintf_s(message[playertwo],"%s",READY);
			}else{
				Score=(Score<0)? Score-1:Score+1;
				first=true;
				sprintf_s(message[playerone],"%s",READY);
				sprintf_s(message[playertwo],"%s",LATER);
			}
			////////////////游戏已经结束////////////////
			if(Score==2||Score==-2){
				endonegame(playerone,playertwo,roomindex);
				break;
			}
		}
		///////////一盘游戏结束/////////////////
		endonegame(playerone,playertwo,roomindex);
	}
}
//////////主函数///////////////
void main(){
	WSADATA			wsaData;
	SOCKET			sListen;		//监听socket
	SOCKADDR_IN		serverAddr;		//服务器地址信息
	int				nResult;		//返回值用于异常处理
	////////////////////初始化///////////////////////////
	WSAStartup(MAKEWORD(2,2),&wsaData);
	sListen=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=htons(SERVPORT);
	serverAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	memset(&(serverAddr.sin_zero),0,sizeof(serverAddr.sin_zero));
	///////////////////绑定监听端口/////////////////////
	nResult=bind(sListen,(SOCKADDR*)&serverAddr,sizeof(SOCKADDR));
	if(nResult==SOCKET_ERROR){
		printf("绑定监听端口失败\n");
		return;
	}
	else{
		printf("开始监听\n");
	}
	/////////////////开始监听，最大等待队列为5//////////
	listen(sListen,5);
	/////////////////向量表////////////////////////////
	int valueforthread[MAXPLAYER]={0,1,2,3};
	/////////////////创建连接线程///////////////////////
	HANDLE	hThreadAccept;
	DWORD	ThreadAccept;
	hThreadAccept=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AcceptProcess,  (LPVOID)&sListen,0, &ThreadAccept);
	printf("接受连接线程已启用\n");
	////////////////创建房间选择线程///////////////////
	HANDLE	hThreadSelect[4];
	DWORD	ThreadSelect[4];
	for(int i=0;i<MAXPLAYER;i++){
		hThreadSelect[i]=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Seat,  (LPVOID)&valueforthread[i],0, &ThreadSelect[i]);
		printf("端口 %d 选择房间线程已开启\n",i+1);
	}
	///////////////游戏房间运行线程////////////////////
	HANDLE	hThreadRoom[2];
	DWORD	ThreadRoom[2];
	for(int i=0;i<2;i++){
		hThreadRoom[0]=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Room,  (LPVOID)&valueforthread[i],0, &ThreadRoom[0]);
		printf("房间 %d 运行线程已开启\n",i+1);
	}
	while(1){
		Sleep(100000);
	}
}