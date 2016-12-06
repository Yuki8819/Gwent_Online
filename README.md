# Gwent_Online
This is a multiplayer version of the card game Gwent, from <b>The Witcher 3</b>.  
Now CD Projekt has their offical Gwent game, see:  
https://www.playgwent.com/  
We developed this game in the early 2016, at that time CD Projekt didn't announced their Gwent game.  

The client is developed with C++ and <b>OpenScenGraph</b>, by me, <i>Yiran Zheng</i>.  
The server is developed with C++ and Socket, by my friend, <i>Shengjie Hu</i>.  
We are both fans of the Witcher series, we just want to build a game that we both love and could play together with.  
  
<b>To build the project:</b>  
I used Visual Studio 2012 developing it, other versions of Visual Studio or other IDEs should also be fine to use.  
As I used <b>OpenSceneGraph</b> to implement the graphics, <b>OpenSceneGraph</b> should be installed in advance.  
To install <b>OpenSceneGraph</b>, see:  
http://www.openscenegraph.org/index.php/documentation/getting-started  
More information about this OpenGL-based graphics toolkit:  
http://www.openscenegraph.org/  
*The version of OpenSceneGraph I used is 3.2.0, it may be okay to use any newer versions.
  
<b>Manual:</b>  
About the rules of the Gwent, see:  
http://witcher.wikia.com/wiki/Gwent  
*Notice: this game only contains a card deck for Northern Realms.  
  
The game allow 2 players play this card game each other, and the server can serve up to 4 players at the same time.  
  
To run the game, first, run the server.  
Second, run the client, and input the IP address of the server and press Enter. (for local tests, use the default 127.0.0.1)  
Third, to select a game room to play, click on one of the card showing on the screen. Make sure the person you want to play with clicked on the same card.  
Then, Play the game. You can click on cards to zoom in and check their details, click again on that card you will be able to play it, just similar to the original Gwent game.  
If you want to end your turn, or give up your round, press Q.  
  
After finishing the whole game, a message box will appear to show you the result of the battle. Then you will be able to connect to the server again and have another card battle.  
