Man'chi Diplomacy AI -- Binary Distribution README
Copyright 2002-2003 Brian Roberts (brian@mirror.org)

This program is an attempt to develop an AI capable of playing the
board game Diplomacy.  This is a very preliminary version that does
not understand many, many important concepts.

-----------------------------------------------------------------------
License:

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

See license.txt or visit http://www.gnu.org/licenses/licenses.html

-----------------------------------------------------------------------
Installing Man'chi:

Man'chi is written in the Java programming language, version 1.4.  It
is a Swing application, not an applet, and does not require an Internet
browser.

To run the application you will require the Java 2 Runtime Environment
(JRE) for your platform.  Implementations for Windows and Linux are
available from Sun:
    http://java.sun.com/j2se/1.4.1/download.html

You'll also need to get and unpack this binary distribution, which I 
assume you've already done since you're reading this file.  The source
distribution includes a separate readme.txt with compiling notes.

Once installed, start Man'chi with the manchi.sh/bat script, or this 
command:
    java -jar manchi.jar
Or most platforms have other methods: in Windows Explorer for example, 
just double-click the manchi.jar file.

-----------------------------------------------------------------------
Using Man'chi:

Note that Man'chi is mainly an experiment, not a complete program. 
There are several incomplete sections and assorted pieces that work but 
are not connected to other pieces yet.

Man'chi can play a game on a DAIDE server.  Choose File|New and enter
the server details, including the robot you want (list below).  By
default it plays automatically, however you can turn off automatic
play (Think|Automatic) and manually choose a bot and submit orders for
each phase.

The application can also save and load games (in its own file format),
and do any type of analysis on any phase in a loaded game.  Its easy
to save a DAIDE game after playing, and go back later to re-analyze any 
phase.  The source distro also includes a few sample (non-DAIDE) games 
for experimentation.

For any game, all analysis and calculations can be displayed.  Choose
View|Analysis and select the phase, country and analysis tab you want 
to see.  To generate every possible analysis (including several not 
yet used for move generation), run DebugBot for all countries.

To assist with my AI programming, Man'chi can also load a tactical 
situation test.  This is a miniature position from a game, pre-made 
strategic/operational decisions, and expected orders with different 
ratings.  The source distro includes a few of these tests and a test 
runner that calculates a average rating.

For testing, a game or tactest filename can be specified on the command
line, or a DAIDE game can be started automatically.  Use "manchi -h"
for details.

Comments, suggestions, complaints and bug reports are welcome.

-----------------------------------------------------------------------
Man'chi Robots:

The following robots are available, intended mainly for DAIDE play.  
All suffer from the lack of a proper "friend or foe" strategic analysis.  
However, they make acceptable moves within their limitations.

AngryBot:  Attacks all neighbours constantly.  No proper defence and
some other simple assumptions.  However, it does pretty well against 
a RandBot or David's DumbBot.

AttackBot:  Initially picks a random neighbour and attacks.  Continues
that attack until somebody else attacks it, then targets that country.  
Pays some attention to defence and DMZs with other countries (but often
too much so).  The detection of "somebody attacks it" is very weak, 
looking for dislodged units or captured SCs only.  This plays closest 
in style to a (very poor) human player.

ChargeBot:  Picks a random neighbour and says "Charge!".  Continues
the attack until the target is dead, then picks a new target.  Ignores 
DMZs, no defence, never looks back, easy to stab.

DefenseBot:  The scared version of AttackBot, or at least that's the
long term intent.  Heavy emphasis on defensive goals with minor attacks 
against its "target".  If is has not been attacked, almost always sits
still like a ParanoidBot.

ParanoidBot:  Assumes all neighbours are going to attack and issues
completely defensive moves.  Huddles in a corner and has zero chance 
of winning.

RevengeBot:  Pretends to be a ParanoidBot until attacked, then plays
like a ChargeBot against the attacker.  If that attacker is eliminated, 
goes back to huddling in its corner.

RandBot:  Random moves.  Interesting only because the random moves
include valid support and convoy orders and no obvious mistakes like
bouncing itself.

DebugBot:  Intended for testing, not play.  Generates every possible 
analysis for display and debugging, then submits random moves.

-----------------------------------------------------------------------
