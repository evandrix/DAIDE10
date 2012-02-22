                                                                                                                                                       
Albert runs on the DAIDE server and Mapper available from the main site.
http://www.daide.org.uk/index.xml

***************************************
ALBERT SPECIFIC COMMANDLINE PARAMETERS
***************************************
-t Tournament Mode
This parameter disables any delay timers in the code.  These timers have been included to give Albert the appearance
of thinking like a human about retreat/adjustment phases.  For example without these delay timers Albert would submit retreat
orders in 1/10th a second which is easily identifiable as a bot in an RT game.  

-n Never Ally Mode
This parameter makes Albert treat all powers as an enemy at all times.  Currently this produces superior results in bot-bot tournaments.

-d[0-100] Depth_of_Thought
This parameter sets how deeply Albert will think.  It changes both the number of iterations per turn
as well as the number of ordersets generated for each power.  Using -d0 results in a bot that is only
slightly better than KMB.  The default Albert runs with a -d50.  Using a -d100 will cause Albert to
iterate almost twice as much and generate approximately 50% more order sets per turn.  I believe there
is a point of diminishing returns.  

Due to the length of time it takes to run tournaments I have not tested Albert at each level.  
I have mainly worked at level -d50 which I feel is decent comprimise.  

	-d20 or -d30 should already begin to yield better results, and still be relatively quick.

*****************************************************
STANDARD COMMANDLINE PARAMETERS FOR THE AI FRAMEWORK
*****************************************************

-s[Server Name] Optionally sets the connection to a server name: such as with a URL address.
-i[IP Address] Optionally sets the connection to a server's IP address.
-p[Port Number] Optionally sets the port number that the game is played on.
-r[POW:passcode] Optionally sets the reconnection attribute ex:// -rGER:4218



********************
ALBERT RELEASE NOTES
********************
Albert v3.0
	  - Added Alliances for level 0 games
	    (Albert will try to form alliances and build trust between powers. This is a major change over v2.1beta which essential
	     is at war with everyone.  Anyone who feels the alliance algorithms make Albert too easy to manipulate can use
	     the [-n] command line parameter to force Albert not to form any alliances.)
	  - Many improvements to the alliance algoritims.
	  - Includes timer-delays for Adjustment/Retreat phases in an attempt to disguise itself as a human in RT games.
	  - Added a commandline parameter '-t' to put Albert in tournament mode to eliminate timer-delays.
	  - Added auxilliary provinces into the strategic evaluation.
	  - Added penalties for moves A-B&B-C when A could move to C.
	  - Revised build Algorithms to properly handle multiple builds.
        - Albert will now offer draws, only when it is significantly trailing.  If Albert is leading it will not offer a draw, even if stalemated.
	  - It is now possible to force Albert to offer a draw, by using the checkbox in the dialog window.


Albert v2.1beta
	  - Added foreign supports, including for self bounces

Albert v2.0
        - will only play a level 0 game.
        - Newly added -d[] command line parameter for 'depth of thought' 0-100 default runs at 50
            ( at -d0 Albert will still still win over KMB by 10-15% )
        - Runs twice as fast while analyzing much more than version 1.0.
        - Revamped iteration control function
        - Iterates through each power 15 times trying to optimize its value.

Albert v1.0
        - will only play a level 0 game.
        - Based partially on KMB version 6.0 code
        - Analyzes Position for every Power on the board.
        - Iterates through each power 10 times trying to optimize its value.
        - Calculates the probabilities of provinces being occupied for each iteration
        - Completely new orderset evaluation system based on probabilities of success.



*************
FINAL COMMENT
*************

I hope you enjoy playing against Albert.  I have worked hard to create it, and I very much hope it can give you a few interesting games.
I welcome all feedback in the appropriate forums listed below.  And if you happen to play in a RealTime game with 6 Humans and 1 Albert, you are
hereby required to publically announce if Albert achieves a solo victory. :o)  And send me the log file. :o) And describe the game. :o)  
I ask for no other payment.

Cheers,
Jason van Hal ~ (Stretch)



************
USEFUL LINKS
************
- Forums
	http://games.groups.yahoo.com/group/dipai/
	http://games.groups.yahoo.com/group/rtnow/
	http://games.groups.yahoo.com/group/rtdip/


- My webpage for Diplomacy
	http://ca.geocities.com/stretchy@rogers.com/

- DAIDE webpage
	http://www.daide.org.uk/index.xml

- David Norman's webpage
	http://www.ellought.demon.co.uk/dipai/

- Wikipedia - Diplomacy 
	http://en.wikipedia.org/wiki/Diplomacy_game

- Diplomatic Pouch
	http://www.diplom.org/index.py

- Diplomacy Archive
	http://www.diplomacy-archive.com/



*********
COPYRIGHT
*********
Albert is created from David Norman's AIclient framework. 
I have taken Dumbbot and eventually have changed almost everything.  
So all code that is not Dumbbot_v4 is copyright © to Jason van Hal 2006-2007.  
Albert and KissMyBot can not be used for any commercial use unless agreed to in writing by me.

*********



