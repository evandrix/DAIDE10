Readme file for DiploBot

Date: December 1st, 2003
Version: 1.0
Author : François McNeil
Current Email: francois_mcneil@yahoo.com
Dependancies: JDK1.4.2, DAIDE Server


DiploBot 1.0 is a Java AI that plays Diplomacy via the DAIDE Server.
Version 1.0 only performs a tactical analysis of the board and therefore can only play no-press games.
The AI has been tested with a couple of different maps (standard, world3, versailles).
Version 2.0 is currently planned for Q1-2004 and will support Press Level 10.

Important features missing from the planned v1.0:
	- Sometimes the AI will fail to connect to the Server. Just kill it and restart.
	- The tactical module isn't complete yet but it is sufficient for now.
	- The AI can't tell (yet) if the units surrounding a province are owned by different powers.
	- The build and remove strategies are barely better than random.
	
Important notes:
	- To launch DiploBot, type this in a command shell from the directory where the diplobot.jar is.
		java -jar diplobot.jar AI_LEVEL IP PORT
	Example: 
		java -jar diplobot.jar 0 10.254.233.12 16713
	
Enjoy !
François