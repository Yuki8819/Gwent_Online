# Gwent_Online
This is a multiplayer version of the card game Gwent, from The Witcher 3.  
The client is developed with C++ and OpenScenGraph, and it is developed by me, Yiran Zheng.  
The server is developed with C++ and Socket, and it is developed by my friend, Shengjie Hu.  
  
Manual:  
About the rules of the Gwent, see:  
http://witcher.wikia.com/wiki/Gwent  
*Notice: this game only contains a card deck for Northern Realms.  
  
The game allow 2 players play this card game each other, and the server can serve up to 4 players at the same time.  
  
To run the game, first, run the server.  
Second, run the client, and input the IP address of the server and press Enter. (for local tests, use the default 127.0.0.1)  
Third, to select a game room to play, click on one of the card showing on the screen. Make sure the person you want to play with clicked on the same card.  
Then, Play the game. You can click on cards to zoom in and check their details, click again on that card you will be able to play it, just similar to the original Gwent game.  
If you want to end your turn, or give up your round, press Q.  
  
After finishing the whole game, a message box will appear to show you the result of the battle. Then you will be able to connect to the server again and have an other card battle.  
  
To build the project:  
I used Visual Studio 2012 to develop it, other versions of Visual Studio or other IDEs should also be fine to use.  
As I used OpenSceneGraph to implement the graphics, OpenSceneGraph should be installed in advance.  
To install OpenSceneGraph, see:  
http://www.openscenegraph.org/  
*The version of OpenSceneGraph I used is 3.2.0, it should be okay to use any newer versions.
