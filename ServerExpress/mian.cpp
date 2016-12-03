#include<stdio.h>
#include<winsock2.h>
#pragma comment (lib,"ws2_32.lib")
/////////////�궨��//////////////////////////////
#define		SERVPORT		5050		//����˿ں�
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
////////////////����ȫ�ֱ���////////////////////////
int available[MAXPLAYER]={1,1,1,1};
int room[2]={0,0};
int roomnum[MAXPLAYER]={NOROOM,NOROOM,NOROOM,NOROOM};
SOCKET	sClient[MAXPLAYER];		//���Ӷ˿�
////////////////�ж��Ƿ��п��ö˿�////////////////////////////
bool Available(int available[MAXPLAYER]){
	for(int i=0;i<MAXPLAYER;i++){
		if(available[i])
			return true;
	}
	return false;
}
////////////////�ж���Ϣ���ݺ���/////////////////////
int	judge(char c[]){
	if(strcmp(c,READY)==0){
		return 1;
	}else if(strcmp(c,GIVEUP)==0){
		return 2;
	}
	return 0;
}
/////////////////�쳣������////////////////////////
void Disconnection(int playerone,int playertwo,int roomindex){
	char buf[MAX];
	////////////playerone�˿����ӶϿ�////////////////
	printf("�˿� %d �ѶϿ�����\n",playerone+1);
	closesocket(sClient[playerone]);
	available[playerone]=AVAILABLE;
	roomnum[playerone]=NOROOM;
	////////////��playertwo�˿ڷ�����Ϣ���������////
	sprintf_s(buf,"%s",END);
	if(send(sClient[playertwo],buf,sizeof(buf),0)==SOCKET_ERROR){
		//////////////playertwo�˿����ӶϿ�//////////
		printf("�˿� %d �ѶϿ�����\n",playertwo+1);
		closesocket(sClient[playertwo]);
		available[playertwo]=AVAILABLE;
	}
	roomnum[playertwo]=NOROOM;
	room[roomindex]=0;
}
/////////////////��Ϸ��������////////////////////////
void endonegame(int playerone,int playertwo,int roomindex){
	printf("���� %d �Ծֽ���\n",roomindex+1);
	printf("�˿� %d �ѶϿ�����\n",playerone+1);
	printf("�˿� %d �ѶϿ�����\n",playertwo+1);	
	available[playerone]=AVAILABLE;
	available[playertwo]=AVAILABLE;
	roomnum[playerone]=NOROOM;
	roomnum[playertwo]=NOROOM;
	room[roomindex]=0;
}
////////////////���������̺߳���/////////////////////
void	AcceptProcess(LPVOID s){
	SOCKET sListen=*(SOCKET*)s;
	SOCKADDR_IN		clientAddr;		//�ͻ��˵�ַ��Ϣ
	int				clientAddrLen;	//���ʽṹ�ĳ���
	char			buf[MAX];		//ͨ��֡
	SOCKET			Sparesocket;	//�������Ӷ˿�
	clientAddrLen=sizeof(SOCKADDR);
	while(1){
		/////////////�п��ö˿�////////////////////////////////
		if(Available(available)){
			int i=0;
			while(!available[i])	i++;
			sClient[i]=accept(sListen,(SOCKADDR *)&clientAddr, &clientAddrLen);
			available[i]=0;
			buf[0]=CONNECTSUCCESS;
			buf[1]='\0';
			send(sClient[i],buf,sizeof(buf),0);
			printf("�˿� %d ������.\n",i+1);
		}
		////////////�������������ñ��ö˿�����////////////////
		else{
			Sparesocket=accept(sListen,(SOCKADDR *)&clientAddr, &clientAddrLen);
			buf[0]=SERVERFULL;
			buf[1]='\0';
			send(Sparesocket,buf,sizeof(buf),0);
			////////////���ñ��ö˿�/////////////////////////
			closesocket(Sparesocket);
		}
	}
}
/////////////ѡ�񷿼��̺߳�����/////////////////////////////////
/////////////�˿�////////////////
void	Seat(LPVOID	s){
	int		seatnum=*(int*)(s);
	bool	reselect=false;
	char	buf[MAX];
	while(1){
		/////////�˿�1��δ������ȴ�//////////
		while(available[seatnum]){
			Sleep(1000);
		}
		/////////�˿�1���ӳɹ�����ѡ�񷿼�//////////
		if(roomnum[seatnum]==NOROOM){
			printf("�ȴ� %d �˿�ѡ�񷿼�\n",seatnum+1);
			////////////���ͷ�������Ϣ//////////////
			buf[0]=SERVERMSG;
			for(int i=0;i<MAXROOM;i++){
				buf[i+1]=room[i];
			}
			buf[MAXROOM+1]='\0';
			////////////������ʧ��//////////////////
			if(send(sClient[seatnum],buf,4,0)==SOCKET_ERROR){
				printf("�˿� %d ���ӶϿ�\n",seatnum+1);
				closesocket(sClient[seatnum]);
				available[seatnum]=AVAILABLE;		
				continue;			//�����쳣���ж����ӣ���������ѭ��
			}
			////////////���ܿͻ���ѡ����Ϣ/////////
			if(recv(sClient[seatnum],buf,sizeof(buf),0)!=SOCKET_ERROR){
				///////////���յ�ѡ����Ϣ//////////////
				if(buf[0]==ROOMSELECT){
					int n=(int)buf[1];
					////////////����ѡ��������//////
					if(room[n-1]==ROOMFULL){
						buf[0]=RESELECT;
						buf[1]='\0';
						reselect=true;
						printf("�˿� %d ��ѡ���������������Ͽ�������\n",seatnum+1);
					}else{
						/////////��ѡ����δ��//////
						roomnum[seatnum]=n-1;
						room[n-1]++;
						printf("�˿� %d ѡ�񷿼� %d\n���� %d ���� %d �����\n",seatnum+1,n,n,room[n-1]);
						if(room[n-1]==ROOMFULL){
							/////////����󷿼���////////
							buf[0]=WAITSTART;
							buf[1]='\0';
						}else{
							/////////����󷿼仹û��///
							buf[0]=WAITANOTHER;
							buf[1]='\0';
						}
					}
					////////////�����жϽ��////////////////////////////
					if(send(sClient[seatnum],buf,sizeof(buf),0)==SOCKET_ERROR){
						///////////������ʧ��//////////////
						printf("�˿� %d ���ӶϿ�\n",seatnum+1);
						closesocket(sClient[seatnum]);
						available[seatnum]=AVAILABLE;		
						continue;			//�����쳣���ж����ӣ���������ѭ��
					}
					if(reselect){
						closesocket(sClient[seatnum]);
						available[seatnum]=AVAILABLE;		
						reselect=false;
						continue;			//�����쳣���ж����ӣ���������ѭ��
					}
				}else{	
					///////���յ��Ĳ���ѡ����Ϣ//////////////
					printf("�˿� %d ���ӶϿ�\n",seatnum+1);
					closesocket(sClient[seatnum]);
					available[seatnum]=AVAILABLE;
					continue;		//�����쳣���ж����ӣ���������ѭ��		
				}
			}else{
				/////////������Ϣʧ��/////////////////////
				printf("�˿� %d ���ӶϿ�\n",seatnum+1);
				closesocket(sClient[seatnum]);
				available[seatnum]=AVAILABLE;
				continue;			//�����쳣���ж����ӣ���������ѭ��
			}
		}
		////////�ȴ��˿�һ�Ͽ�����//////////
		while(!available[seatnum]){
			Sleep(1000);
		}
	}
}
/////////////��Ϸ�����߳�////////////////////////////////////////
//////////����////////////////
void	Room(LPVOID s){
	int roomindex=*(int*)(s);
	char	buf[MAX],message[2][MAX];
	int		playerone,playertwo;
	while(1){
		////////////�ȴ���������///////////
		while(room[roomindex]!=ROOMFULL){
			Sleep(1000);
		}
		printf("���� %d �Ծֿ�ʼ\n",roomindex+1);
		Sleep(1000);		//�ȴ������Ϣ����
		////////////ȷ��������Ҷ˿�///////
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
		///////////////������Ϸ���п��Ʋ���//////////////
		bool	Giveupone,Giveuptwo;		//�ж��Ƿ��������
		int		n;							//�ж���Ϣ����
		int     Scoreone,Scoretwo;			//����ÿ�ֵ���
		int     Score=0;					//�ȷ�
		bool    first=false;				//����ѭ�����ж�����
		bool    disconnect=false;			//���ߴ���
		////////////////������Ϸ��ѭ��////////////////////
		for(int i=0;i<3;i++){
			///////////////����ַ����ͺ�����Ϣ////////////////////////////////
			sprintf_s(buf,"%s",LATER);
			if(send(sClient[playertwo],buf,sizeof(buf),0)==SOCKET_ERROR){
				////////////�쳣����////////////////
				Disconnection(playertwo,playerone,roomindex);
				break;
			}
			/////////////////�����ַ�����������Ϣ/////////////////////////////
			sprintf_s(buf,"%s",READY);
			if(send(sClient[playerone],buf,sizeof(buf),0)==SOCKET_ERROR){
				//////////////////�쳣����////////////////////
				Disconnection(playerone,playertwo,roomindex);
				break;
			}
			Giveupone=false;
			Giveuptwo=false;
			first=true;
			while(!(Giveupone&&Giveuptwo)){
				/////////������û�������֣�����Գ���////////////
				while((!Giveupone)&&first){
					if(recv(sClient[playerone],buf,sizeof(buf),0)==SOCKET_ERROR){
				////////////playerone�˿����ӶϿ�////////////////
						Disconnection(playerone,playertwo,roomindex);
						disconnect=true;
						break;
					}
					if(strcmp(buf,"38000")==0||strcmp(buf,"39000")==0){
						char assbuf[MAX];
						if(recv(sClient[playerone],assbuf,sizeof(assbuf),0)==SOCKET_ERROR){
						////////////playerone�˿����ӶϿ�////////////////
							Disconnection(playerone,playertwo,roomindex);
							disconnect=true;
							break;
						}
						strcat(buf,assbuf);
					}
					if(send(sClient[playertwo],buf,sizeof(buf),0)==SOCKET_ERROR){
						////////////playertwo�˿����ӶϿ�////////////////
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
					////////////playerone�˿����ӶϿ�////////////////
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
				////////����û�������֣����ֿ��Գ���/////////////
				while(!Giveuptwo){
					if(!first)	first=true;
					if(recv(sClient[playertwo],buf,sizeof(buf),0)==SOCKET_ERROR){
						////////////playertwo�˿����ӶϿ�////////////////
						Disconnection(playertwo,playerone,roomindex);
						disconnect=true;
						break;
					}
					if(strcmp(buf,"38000")==0||strcmp(buf,"39000")==0){
						char assbuf[MAX];
						if(recv(sClient[playertwo],assbuf,sizeof(assbuf),0)==SOCKET_ERROR){
							////////////playertwo�˿����ӶϿ�////////////////
							Disconnection(playertwo,playerone,roomindex);
							disconnect=true;
							break;
						}
						strcat(buf,assbuf);
					}
					if(send(sClient[playerone],buf,sizeof(buf),0)==SOCKET_ERROR){
					////////////playerone�˿����ӶϿ�////////////////
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
							////////////playertwo�˿����ӶϿ�////////////////
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
			///////////////�жϾַ�///////////////
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
			////////////////��Ϸ�Ѿ�����////////////////
			if(Score==2||Score==-2){
				endonegame(playerone,playertwo,roomindex);
				break;
			}
		}
		///////////һ����Ϸ����/////////////////
		endonegame(playerone,playertwo,roomindex);
	}
}
//////////������///////////////
void main(){
	WSADATA			wsaData;
	SOCKET			sListen;		//����socket
	SOCKADDR_IN		serverAddr;		//��������ַ��Ϣ
	int				nResult;		//����ֵ�����쳣����
	////////////////////��ʼ��///////////////////////////
	WSAStartup(MAKEWORD(2,2),&wsaData);
	sListen=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=htons(SERVPORT);
	serverAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	memset(&(serverAddr.sin_zero),0,sizeof(serverAddr.sin_zero));
	///////////////////�󶨼����˿�/////////////////////
	nResult=bind(sListen,(SOCKADDR*)&serverAddr,sizeof(SOCKADDR));
	if(nResult==SOCKET_ERROR){
		printf("�󶨼����˿�ʧ��\n");
		return;
	}
	else{
		printf("��ʼ����\n");
	}
	/////////////////��ʼ���������ȴ�����Ϊ5//////////
	listen(sListen,5);
	/////////////////������////////////////////////////
	int valueforthread[MAXPLAYER]={0,1,2,3};
	/////////////////���������߳�///////////////////////
	HANDLE	hThreadAccept;
	DWORD	ThreadAccept;
	hThreadAccept=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AcceptProcess,  (LPVOID)&sListen,0, &ThreadAccept);
	printf("���������߳�������\n");
	////////////////��������ѡ���߳�///////////////////
	HANDLE	hThreadSelect[4];
	DWORD	ThreadSelect[4];
	for(int i=0;i<MAXPLAYER;i++){
		hThreadSelect[i]=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Seat,  (LPVOID)&valueforthread[i],0, &ThreadSelect[i]);
		printf("�˿� %d ѡ�񷿼��߳��ѿ���\n",i+1);
	}
	///////////////��Ϸ���������߳�////////////////////
	HANDLE	hThreadRoom[2];
	DWORD	ThreadRoom[2];
	for(int i=0;i<2;i++){
		hThreadRoom[0]=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Room,  (LPVOID)&valueforthread[i],0, &ThreadRoom[0]);
		printf("���� %d �����߳��ѿ���\n",i+1);
	}
	while(1){
		Sleep(100000);
	}
}