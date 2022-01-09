package project;

import java.util.*;

public class trackController {
	
	//variable declaration
	public static boolean redToYard;
	public static boolean greenToYard;
	public static List<Queue<Boolean>> redQueue; 
	public static List<Queue<Boolean>> greenQueue;
	public static int stop;
	public static boolean redStop;
	public static boolean greenStop;
	public static float authority;
	public static int redLine[][]; 		//1st dimension is track section (0 represents yard)
	public static int greenLine[][]; 	//2nd is switch position, crossing status, train presence, track status, switch existence, crossing existence 
										//each element in the first array will correspond alpha-numerically (with the 0th element being skipped)
	
	//constructors
	public trackController () {
	}
	
	public trackController(int redTrackSize, int greenTrackSize) {
		redLine = new int[redTrackSize+1][6];		//add 1 so track section numbers match index numbers
		greenLine = new int[greenTrackSize+1][6];
	}
	
	
	//setters and getters
	public void setSwitchPosition(String line, int section, boolean position) {
		if (line.equals("red")) redLine[section][0] = (position) ? 1:0;
		else if (line.equals("green")) greenLine[section][0] = (position) ? 1:0;
	}
	
	public boolean getSwitchPosition(String line, int section) {
		int returnVal = 0; 
		//get integer return value
		if (line.equals("red")) returnVal = redLine[section][0];
		else if (line.equals("green")) returnVal = greenLine[section][0];
		//convert from integer to boolean
		if (returnVal==1) return true;
		return false;
	}
	
	public void setCrossingStatus(String line, int section, boolean active) {
		if (line.equals("red")) redLine[section][1] = (active) ? 1:0;
		else if (line.equals("green")) greenLine[section][1] = (active) ? 1:0;
	}
	
	public boolean getCrossingStatus(String line, int section) {
		int returnVal = 0; 
		//get integer return value
		if (line.equals("red")) returnVal = redLine[section][1];
		else if (line.equals("green")) returnVal = greenLine[section][1];
		//convert from integer to boolean
		if (returnVal==1) return true;
		return false;
	}
	
	public void setTrainPresence(String line, int section, boolean presence) {
		if (line.equals("red")) redLine[section][2] = (presence) ? 1:0;
		else if (line.equals("green")) greenLine[section][2] = (presence) ? 1:0; 
	}
	
	public boolean getTrainPresence(String line, int section) {
		int returnVal = 0; 
		//get integer return value
		if (line.equals("red")) returnVal = redLine[section][2];
		else if (line.equals("green")) returnVal = greenLine[section][2];
		//convert from integer to boolean
		if (returnVal==1) return true;
		return false;
	}
	
	public void setTrackStatus(String line, int section, boolean status) {
		if (line.equals("red")) redLine[section][3] = (status) ? 1:0;
		else if (line.equals("green")) greenLine[section][3] = (status) ? 1:0; 
	}
	
	public boolean getTrackStatus(String line, int section) {
		int returnVal = 0; 
		//get integer return value
		if (line.equals("red")) returnVal = redLine[section][3];
		else if (line.equals("green")) returnVal = greenLine[section][3];
		//convert from integer to boolean
		if (returnVal==1) return true;
		return false;
	}
	
	public void setSwitchExistence(String line, int section, boolean exists) {
		if (line.equals("red")) redLine[section][4] = (exists) ? 1:0;
		else if (line.equals("green")) greenLine[section][4] = (exists) ? 1:0; 
	}
	
	public boolean getSwitchExistence(String line, int section) {
		int returnVal = 0; 
		//get integer return value
		if (line.equals("red")) returnVal = redLine[section][4];
		else if (line.equals("green")) returnVal = greenLine[section][4];
		//convert from integer to boolean
		if (returnVal==1) return true;
		return false;
	}
	
	public void setCrossingExistence(String line, int section, boolean exists) {
		if (line.equals("red")) redLine[section][5] = (exists) ? 1:0;
		else if (line.equals("green")) greenLine[section][5] = (exists) ? 1:0;
	}
	
	public boolean getCrossingExistence(String line, int section) {
		int returnVal = 0; 
		//get integer return value
		if (line.equals("red")) returnVal = redLine[section][5];
		else if (line.equals("green")) returnVal = greenLine[section][5];
		//convert from integer to boolean
		if (returnVal==1) return true;
		return false;
	}
	
	public void setAuthority(float newAuthority) {
		authority = newAuthority;
	}
}
