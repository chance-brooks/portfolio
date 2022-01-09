package project;

import java.util.*;
import javax.swing.*;

public class PLCInterpreter extends trackController {
	
	//declare variables
	private static JFileChooser fileChooser1 = new JFileChooser();
	private static JFileChooser fileChooser2 = new JFileChooser();
	public static java.io.File file1;
	public static java.io.File file2;
	private static String fileName1;
	private static String fileName2;
	private static Scanner compare1;
	private static Scanner compare2;
	private static Scanner input;
	
	private static String fileName;
	private static String line;
	private static int section;
	private static int source;
	private static int higher;
	private static int lower;
	private static String condition;
	private static String action;
	private static boolean value;
	
	//first file opening method
	public void openFirstFile() {
		if (fileChooser1.showOpenDialog(null) == JFileChooser.APPROVE_OPTION) {
			
			//get file
			file1 = fileChooser1.getSelectedFile();
			fileName1 = file1.getName();
			
			try {
				//create scanner
				compare1 = new Scanner(file1);
			} catch (Exception e) {
				System.out.println("cancelled");
			}
		}
	}
	
	//second file opening method
	public void openSecondFile() {
		if (fileChooser2.showOpenDialog(null) == JFileChooser.APPROVE_OPTION) {
			
			//get file
			file2 = fileChooser2.getSelectedFile();
			fileName2 = file2.getName();
			
			try {
				//create scanner
				compare2 = new Scanner(file2);
			} catch (Exception e) {
				System.out.println("cancelled");
			}
			
		}
	}

	//get file extension
	public String getFileType() {
		return fileName1.substring(fileName1.length()-3);
	}
	
	//create perfect voter to ensure files match
	public boolean compareFiles() {
		while (compare1.hasNext() && compare2.hasNext())
			if (!compare1.nextLine().equals(compare2.nextLine())) {
				compare1.close();
				compare2.close();
				return false; 
			}
		return true;
	}
	
	//convert PLC to running java code
	public void runPLC(java.io.File file) {
		//specify line and section for PLC
		fileName = file.getName();
		if (fileName.contains("Red")) line = "red";
		else if (fileName.contains("Green")) line = "green";
		if (fileName.contains("Switch")) action = "switch";
		else if(fileName.contains("Crossing")) action = "crossing";
		section = Integer.parseInt(fileName.substring(fileName.length()-6, fileName.length()-4));
		
		try {
			//create scanner
			input = new Scanner(file);
		} catch (Exception e) {
			System.out.println("cancelled");
		}
		while (input.hasNext()) {
			//get next line to interpret
			String command = input.nextLine();
			
			//get PLC functionality
			if (command.contains("IF") && command.contains("THEN")) {
				//get sections to check presence
				ArrayList<Integer> allSections = new ArrayList<Integer>();
				int startIndex = command.indexOf("ON") + 3;
				int endIndex = command.indexOf("THEN") - 1;
				String trackSections = command.substring(startIndex, endIndex);
				Scanner intScanner = new Scanner(trackSections);
				while (intScanner.hasNext())
					allSections.add(intScanner.nextInt());
				
				//get value to set on true
				int executeIndex = command.indexOf("=");
				if (executeIndex >= 0)
					value = Boolean.valueOf(command.substring(executeIndex+2));
				
				//test conditions
				boolean execute = true;
				if (allSections.size()<=3) {
					for (int i=0; i<allSections.size(); i++) 
						if (!super.getTrainPresence(line, allSections.get(i))) 
							execute = false;
				}
				else {
					execute = false;
					for (int i=0; i<allSections.size(); i++) 
						if (super.getTrainPresence(line, allSections.get(i)))
							execute = true;
				}
				
				//execute command
				if (command.contains("toYard") && execute) { 
					if (line.equals("red")) {
						super.setSwitchPosition(line, section, super.redToYard);
						super.redToYard = false;
					}
					if (line.equals("green")) {
						super.setSwitchPosition(line, section, super.greenToYard);
						super.greenToYard = false;
					}
				}
				if (command.contains("STOP") && execute) {
					super.stop = section;
					if (line.equals("red")) super.redStop = true;
					if (line.equals("green")) super.greenStop = true;
				}
				else if (action.equals("switch") && execute)
					super.setSwitchPosition(line, section, value);
				else if (action.equals("crossing") && execute) 
					super.setCrossingStatus(line, section, value);
			}
		} 
	}
}
