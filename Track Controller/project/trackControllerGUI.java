package project;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;


public class trackControllerGUI extends trackController {

	//declare variables
	private static String redFailString = "";
	private static String greenFailString = "";
	private static String line;
	private static int section;
	private static boolean pause;
	private static String switchString;
	private static String crossingString;
	private static String presenceString;
	private static String brokenString;
	private static String[] switchList = {"Upper Block", "Lower Block"};
	private static String[] crossingList = {"Activate", "Deactivate"};
	private static ArrayList<java.io.File> PLCs = new ArrayList<java.io.File>();
	public static PLCInterpreter interpreter;
	
	//declare swing elements
	private static JFrame frame;
	private static JPanel panel;
	
	private static JLabel failLabelRed;
	private static JLabel failLabelGreen;
	private static JButton failCheck;
	private static JButton failSend;
	
	private static JLabel trackSelectionLabel;
	private static ButtonGroup trackGroup;
	private static JRadioButton redSelection;
	private static JRadioButton greenSelection;
	
	private static JLabel trackSectionLabel;
	private static JTextField sectionInput;
	private static JLabel emptySection;
	private static JLabel invalidSection;
	
	private static JLabel switchPosition;
	private static JLabel crossingStatus;
	private static JLabel trainPresence;
	private static JLabel trackStatus;
	
	private static JLabel switchComboLabel;
	private static JLabel crossingComboLabel;
	private static JComboBox<String> switchChoice;
	private static JComboBox<String> crossingChoice;
	private static JButton submit;
	
	private static JButton PLC1;
	private static JButton PLC2;
	private static JLabel noPLC;
	private static JLabel mismatchPLC;
	
	public trackControllerGUI() {
		
		//create elements
		frame = new JFrame();
		panel = new JPanel();
		
		failLabelRed = new JLabel(redFailString);
		failLabelGreen = new JLabel(greenFailString);
		failCheck = new JButton("Scan for Track Failures");
		failSend = new JButton("Send Failure Information to CTC");
		
		trackSelectionLabel = new JLabel("Track Selection:");
		trackGroup = new ButtonGroup();
		redSelection = new JRadioButton("Red Line");
		greenSelection = new JRadioButton("Green Line");
		
		trackSectionLabel = new JLabel("Track Section (input as single int with no space then press enter):");
		sectionInput = new JTextField();
		emptySection = new JLabel("Please input a track section!");
		invalidSection = new JLabel
				("Please input a VALID track section (red: < " +super.redLine.length + " green: < " +super.greenLine.length);
		
		switchPosition = new JLabel();
		crossingStatus = new JLabel();
		trainPresence = new JLabel();
		trackStatus = new JLabel();
		
		switchComboLabel = new JLabel("Set Switch Position:");
		crossingComboLabel = new JLabel("Activate Crossing:");
		switchChoice = new JComboBox<>(switchList);
		crossingChoice = new JComboBox<>(crossingList);
		submit = new JButton("Submit");
		
		PLC1 = new JButton("Select Primary PLC");
		PLC2 = new JButton("Select Redundant PLC");
		noPLC = new JLabel("No Valid PLC Selected: Please Select .txt File");
		mismatchPLC = new JLabel("PLC Files Do NOT Match");
		
		//configure frame
		frame.setSize(1200, 1000);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setTitle("Wayside Control Center");;
		frame.setVisible(true);

		//configure panel
		frame.add(panel);
		panel.setLayout(null);
		
		//configure error notification
		failLabelRed.setBounds(350,75,500,25);
		failLabelRed.setForeground(Color.RED);
		failLabelRed.setVisible(false);
		panel.add(failLabelRed);
		failLabelGreen.setBounds(350,125,500,25);
		failLabelGreen.setForeground(Color.GREEN);
		failLabelGreen.setVisible(false);
		panel.add(failLabelGreen);
		failCheck.setBounds(100,100,200,25);
		failCheck.addActionListener(e -> {
			//check for track failures in track arrays
			for (int i=1; i<super.redLine.length-2; i++)
				if (super.getTrackStatus("red", i)) 
					redFailString = redFailString + i + ", ";
			for (int i=1; i<super.greenLine.length-2; i++)
				if (super.getTrackStatus("green", i))
					greenFailString = greenFailString + i + ", ";
			failLabelRed.setText(redFailString);
			failLabelRed.setVisible(true);
			failLabelGreen.setText(greenFailString);
			failLabelGreen.setVisible(true);
		});
		panel.add(failCheck);
		failSend.setBounds(850,100,250,25);
		failSend.addActionListener(e -> {
			//acts purely as acknowledgement for simulation then track is "fixed"
			for (int i=0; i<super.redLine.length; i++)
				super.setTrackStatus("red", i, false);
			for (int i=0; i<super.greenLine.length; i++)
				super.setTrackStatus("green", i, false);
			redFailString = "";
			greenFailString = "";
			failLabelRed.setVisible(false);
			failLabelGreen.setVisible(false);
		});
		panel.add(failSend);
		
		//configure track selection button
		trackSelectionLabel.setBounds(100,200,200,25);
		panel.add(trackSelectionLabel);
		redSelection.setBounds(500,200,200,25);
		trackGroup.add(redSelection);
		panel.add(redSelection);
		redSelection.setSelected(true);
		greenSelection.setBounds(900,200,200,25);
		trackGroup.add(greenSelection);
		panel.add(greenSelection);
		
		//configure section select
		trackSectionLabel.setBounds(100,400,400,25);
		panel.add(trackSectionLabel);
		sectionInput.setBounds(700,400,300,25);
		emptySection.setBounds(475,300,300,25);
		emptySection.setForeground(Color.RED);
		panel.add(emptySection);
		emptySection.setVisible(false);				//set false until empty input detected
		invalidSection.setBounds(400,300,400,25);
		invalidSection.setForeground(Color.RED);
		panel.add(invalidSection);
		invalidSection.setVisible(false);			//set false until invalid input detected
		
		//update current data
		sectionInput.addActionListener(e -> {
			//check for input
			if(sectionInput.getText().equals("")) {
				invalidSection.setVisible(false);
				emptySection.setVisible(true);
				pause = true;
				return;
			}
			emptySection.setVisible(false);			//clear alert when corrected
			//check for valid section choice
			section = Integer.parseInt(sectionInput.getText());
			if ((redSelection.isSelected() && section>=super.redLine.length) || (greenSelection.isSelected() && section>=greenLine.length)) {
				invalidSection.setVisible(true);
				pause = true;
				return;
			}
			invalidSection.setVisible(false);		//clear alert when corrected
			pause = false;
			//update data
			if (redSelection.isSelected())
				line = "red";
			else if (greenSelection.isSelected())
				line = "green";
			if (super.getSwitchPosition(line, section)) switchString = "Higher Block";
			else switchString = "Lower Block";
			if (super.getCrossingStatus(line, section)) crossingString = "Active";
			else crossingString = "Inactive";
			if (super.getTrainPresence(line, section)) presenceString = "Yes";
			else presenceString = "No";
			if (!super.getTrackStatus(line, section)) brokenString = "Normal";
			else brokenString = "Broken";
			if (!super.getSwitchExistence(line, section)) switchString = "N/A";
			if (!super.getCrossingExistence(line, section)) crossingString = "N/A";
			switchPosition.setText("Switch Position: " +switchString);
			crossingStatus.setText("Crossing Status: " +crossingString);
			trainPresence.setText("Train Present: " +presenceString);
			trackStatus.setText("Track Status: " +brokenString); 
		});
		panel.add(sectionInput);
		
		//ensure valid section input
		while(pause);
		
		//configure current section data
		switchPosition.setBounds(100,500,200,25);
		panel.add(switchPosition);
		crossingStatus.setBounds(350,500,150,25);
		panel.add(crossingStatus);
		trainPresence.setBounds(650,500,150,25);
		panel.add(trainPresence);
		trackStatus.setBounds(900,500,150,25);
		panel.add(trackStatus);
		
		//configure crossing and switch dictation
		switchComboLabel.setBounds(100, 600, 200, 25);
		panel.add(switchComboLabel);
		switchChoice.setBounds(300,600,100,25);
		panel.add(switchChoice);
		crossingComboLabel.setBounds(800,600,200,25);
		panel.add(crossingComboLabel);
		crossingChoice.setBounds(1000,600,100,25);
		panel.add(crossingChoice);
		submit.setBounds(550,650,100,25);
		//update switch and crossing arrays
		//if section doesn't have switch/crossing changing those values has no impact so no test is needed
		submit.addActionListener(e -> {
			//get active line
			if (redSelection.isSelected())
				line = "red";
			else if (greenSelection.isSelected())
				line = "green";
			if (switchChoice.getSelectedIndex() == 0)
				super.setSwitchPosition(line, section, true);
			else 
				super.setSwitchPosition(line, section, false);
			if (crossingChoice.getSelectedIndex() == 0)
				super.setCrossingStatus(line, section, true);
			else
				super.setCrossingStatus(line, section, false);
		});
		panel.add(submit);
		
		//configure PLC uploader
		interpreter = new PLCInterpreter();
		noPLC.setBounds(450,850,500,25);
		noPLC.setForeground(Color.RED);
		noPLC.setVisible(false);
		panel.add(noPLC);
		PLC1.setBounds(100,800,200,25);
		PLC1.addActionListener(e -> {
			//upload first PLC
			interpreter.openFirstFile();
			if (!interpreter.getFileType().equals("txt")) {
				noPLC.setVisible(true);
				mismatchPLC.setVisible(false);
			}
			else if (interpreter.getFileType().equals("txt")) noPLC.setVisible(false);
		});
		panel.add(PLC1);
		mismatchPLC.setBounds(450,850,500,25);
		mismatchPLC.setForeground(Color.RED);
		mismatchPLC.setVisible(false);
		panel.add(mismatchPLC);
		PLC2.setBounds(900,800,200,25);
		PLC2.addActionListener(e -> {
			//upload second PLC
			interpreter.openSecondFile();
			if (!interpreter.compareFiles()) {
				mismatchPLC.setVisible(true);
				noPLC.setVisible(false);
			}
			else if (interpreter.compareFiles()) {
				mismatchPLC.setVisible(false);
				PLCs.add(interpreter.file1);

			}
		});
		panel.add(PLC2);
		
		//double check all elements are on panel
		panel.repaint();
	}
	
	public void run() {
		//run uploaded PLCS
			System.out.println("Running PLCs");
			for (int i=0; i<PLCs.size(); i++)
				interpreter.runPLC(PLCs.get(i));
	}
}

