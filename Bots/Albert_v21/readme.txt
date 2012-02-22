
                                                                                                                                                       
Albert runs on the DAIDE server and Mapper available from the main site.
http://www.daide.org.uk/index.xml


COMMANDLINE PARAMETER
-d[0-100] Depth_of_Thought
This parameter sets how deeply Albert will think.  It changes both the number of iterations per turn
as well as the number of ordersets generated for each power.  Using -d0 results in a bot that is only
slightly better than KMB.  The default Albert runs with a -d50.  Using a -d100 will cause Albert to
iterate almost twice as much and generate approximately 50% more order sets per turn.  I believe there
is a point of diminishing returns.  

Due to the length of time it takes to run tournaments I have not tested Albert at each level.  
I have mainly worked at level -d50 which I feel is decent comprimise.  Originally I was only going to
allow Albert to be made quicker, but maybe some people want to try it out at higher levels.  

-d20 or -d30 should already begin to yield better results, and still be relatively quick.



RELEASE NOTES

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



*****
Albert has a lot of KMB code which was based on Dumbbot v4 and the client framework. 
I have taken Dumbbot and started by making minor changes, and then eventually have changed almost everything.  
So I guess all code that is not Dumbbot_v4 is copyright © to Jason van Hal 2006 .  
Albert and  KissMyBot can not be used for any commercial use unless agreed to in writing by me. :o)


Enjoy..

Cheers,
Jason van Hal ~ (Stretch)
