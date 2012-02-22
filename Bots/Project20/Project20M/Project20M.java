// TestBot.jar by Henrik Bylund used as a base for Project20M


/*****************************************************************************
 * $Id: TestBot.java,v 1.41 2004/12/01 11:45:28 vc02 Exp $
 *
 * Copyright © 2002, 2004 by Henrik Bylund
 * This code is released in the public domain.
 *****************************************************************************/

import dip.daide.comm.MessageListener;
import dip.daide.comm.DisconnectedException;
import dip.daide.comm.UnknownTokenException;
import dip.daide.comm.Server;

import java.net.InetAddress;
import java.net.UnknownHostException;
import java.io.IOException;
import java.net.ConnectException;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.*;

import state.*;

/**
 * An interactive client for testing the communication.
 * Lines are read from <code>System.in</code> and all incoming
 * messages are printed to <code>System.out</code>.
 * When breaking the input into tokens, whitespace is not allowed within
 * text tokens (Any text surrounded by <code>'</code>)
 *
 * @author <a href="mailto:heb@ludd.luth.se">Henrik Bylund</a>
 * @version 1.0
 */

public class Project20M implements MessageListener {
    static String VERSION = "v 0.1";
    static String name = "Project20M";
    Server serv;
    boolean atPrompt;
    boolean Connected=false;
    boolean Started=false;
    String Country;
    String Passcode;

    state.Map map;
    Tactics t;
    Coordinator n;
    
    PrimeMinister pm;


    void printPrompt(){
        if(!atPrompt){
            System.out.print("Enter message: ");
            atPrompt = true;
        }
    }

    void printMsg(String who, String[] message){
	StringBuffer sbuf = new StringBuffer();
	for(int i = 0; i < message.length; i++){
	    String m = message[i];
	    sbuf.append(m);
	    boolean last = (i+1) == message.length;
	    if(m.equals("(") || (!last && message[i+1].equals(")"))){
		continue;
	    } else {
		if(!last){
		    sbuf.append(' ');
		}
	    }
	}
        if(atPrompt){
            System.out.println();
            atPrompt = false;
        }
	System.out.println("<" + who + ">: " + sbuf.toString());
    }

    public Project20M(InetAddress ip, int port){

	try {

		String line;
		String[] nme;
		serv = new Server(ip, port);
		serv.addMessageListener(this);
                try{
		serv.connect();
                }
                catch (ConnectException e){
                  System.out.println("Failed to Connect - have you started DAIDE?");
                  System.exit(0);
                }
		pm = new PrimeMinister(serv);
		BufferedReader br =
		    new BufferedReader(new InputStreamReader(System.in), 1); // should this BufferedReader be bigger?
		/*
		System.out.println("Are you reconnecting(Y or N)");
		line = br.readLine();
		if(line.equals("Y"))
		{
			System.out.println("Please enter your country and passcode");
			line = br.readLine();
			String[] game = line.split(" ");
			nme = new String[]{"IAM",
				"(", game[0], ")",
				"(", "'" + game[1]+"'", ")"};

		} else {*/
		nme = new String[]{"NME", "(", "'" + name + "'", ")",	"(", "'" + VERSION + "'", ")"};
		//}
		printMsg(name, nme);
		System.err.println("about to send nme to the server");
		serv.send(nme);
		System.err.println("sent nme to the server");
		while(!Started);
	    	while(true){
	    	System.err.println("in busy loop?");
                	printPrompt();
			line = br.readLine();
			String[] order = line.split(" ");
                	atPrompt = false;
			try {
				printMsg(name, order);
				serv.send(order);
			} catch (UnknownTokenException ute){
				System.err.println("Unknown token '" + ute.getToken() +
					"' - Message not sent.");
			} catch (DisconnectedException de){
				System.err.println("Disconnected from server, " +
					"command not sent");
			}
		}
	} catch (IOException ioe){
	    ioe.printStackTrace();
	} catch (DisconnectedException de){
	    System.out.println("Ok, we're disconnected. Exiting...");
	    System.exit(0);
	} catch (UnknownTokenException ute){
	    System.err.println("Unknown token '" + ute.getToken() + "'");
	    System.exit(1);
	}

    }

    public void messageReceived(String[] message){ // is the string passed to this sometimes too long?
	  //printMsg("srv", message); // Print the message
          pm.addMessage(message);  // send all other messages to the prime minister
    }


    public static void main(String[] args){
       System.out.println("Project20M started");
	try {
		new Project20M(InetAddress.getByName(args[0]),
		Integer.parseInt(args[1]));

	} catch (ArrayIndexOutOfBoundsException be){
	    usage();
	} catch (UnknownHostException uhe){
	    System.err.println("Unknown host: " + uhe.getMessage());
	} catch (NumberFormatException nfe){
	    usage();
	}
    }

    public static void usage(){
	System.err.println("Usage:\n" +
			   "  Project20M <ip> <port>");
    }

}

