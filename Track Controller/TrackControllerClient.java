/* Needed on Client only */
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.*;
import java.io.*;
import project.*;

public class TrackControllerClient {
	
	private static final boolean CONFIGURABLE = false;
	private static final boolean FULLSYSTEM = true;
	
    private static final int PORTNUMBER = 0;
    private static final String cpuIP = "18.216.251.172";
    private static ServerInterface mini;
    private static boolean connected = false;
    
    //initialization variables 
    private static Integer redTrackSize;
    private static Integer greenTrackSize;
    private static int[][] redConnectivity;			//0th row = switch, 1st row = crossing
    private static int[][] greenConnectivity;		//2nd row = direction of travel
    public static trackController controller;
    public static loginGUI login;
    public static trackControllerGUI controllerGUI;
    
    //deployment variables
    public static Object nodes;
    
    //monitoring variables
    private static boolean redBackedup;
    private static int redFault;
    private static boolean greenBackedup;
    private static int greenFault;
    private static Boolean redToYard;
    private static Boolean greenToYard;
    private static boolean[][] redStatus;				//0th row = train presence //2nd row = circuit failure
    private static boolean[][] greenStatus;				//1st row = broken track	//3rd row = power failure 
    private static Boolean[] redPosition;
    private static Boolean[] greenPosition; 
    private static Boolean[] redMaintenance;
    private static Boolean[] greenMaintenance;
    private static Boolean[] authorityRedCorrection;
    private static Boolean[] authorityGreenCorrection;
    
    public static void main(String[] args) {

    	//configure track
    	if (CONFIGURABLE) {
	    	redTrackSize = (Integer)receive("redTrackSize");
	    	greenTrackSize = (Integer) receive("greenTrackSize");
	    	while (redTrackSize == null || greenTrackSize == null)
	    	{
	    		System.out.println("Waiting for track Configurations");
	    		redTrackSize = (Integer)receive("redSize");
	        	greenTrackSize = (Integer) receive("greenSize");
	    	}
		    trackController controller = new trackController(redTrackSize, greenTrackSize);
	    	redConnectivity = (int[][])receive("redConfigurationInt");		
	    	greenConnectivity = (int[][])receive("greenConfigurationInt");	
    	}
    	else {
    		redTrackSize = 79;
    		redConnectivity = new int[redTrackSize][3];
    		redConnectivity[9][0] = 1;
    		redConnectivity[77][0] = -1;
    		redConnectivity[10][0] = -1;
    		redConnectivity[16][0] = 2;
    		redConnectivity[1][0] = -2;
    		redConnectivity[15][0] = -2;
    		redConnectivity[27][0] = 3;
    		redConnectivity[28][0] = -3;
    		redConnectivity[76][0] = -3;
    		redConnectivity[33][0] = 4;
    		redConnectivity[32][0] = -4;
    		redConnectivity[72][0] = -4;
    		redConnectivity[38][0] = 5;
    		redConnectivity[39][0] = -5;
    		redConnectivity[71][0] = -5;
    		redConnectivity[44][0] = 6;
    		redConnectivity[43][0] = -6;
    		redConnectivity[67][0] = -6;
    		redConnectivity[52][0] = 7;
    		redConnectivity[53][0] = -7;
    		redConnectivity[66][0] = -7;
    		redConnectivity[47][1] = 1;
    		redStatus = new boolean[redTrackSize][4];
    		for (int i=0; i<redTrackSize; i++) {
    			redStatus[i][1] = false;
    			redStatus[i][2] = false;
    			redStatus[i][3] = false;
    		}
    		greenTrackSize = 153;
    		greenConnectivity = new int[greenTrackSize][3];
    		greenConnectivity[13][0] = 1;
    		greenConnectivity[12][0] = -1;
    		greenConnectivity[1][0] = -1;
    		greenConnectivity[29][0] = 2;
    		greenConnectivity[30][0] = -2;
    		greenConnectivity[150][0] = -2;
    		greenConnectivity[57][0] = 3;
    		greenConnectivity[58][0] = -3;
    		greenConnectivity[151][0] = -3;
    		greenConnectivity[63][0] = 4;
    		greenConnectivity[152][0] = -4;
    		greenConnectivity[62][0] = -4;
    		greenConnectivity[77][0] = 5;
    		greenConnectivity[76][0] = -5;
    		greenConnectivity[101][0] = -5;
    		greenConnectivity[85][0] = 6;
    		greenConnectivity[86][0] = -6;
    		greenConnectivity[100][0] = -6;
    		greenConnectivity[19][1] = 1;
    		greenStatus = new boolean[greenTrackSize][4];
    		for (int i=0; i<greenTrackSize; i++) {
    			greenStatus[i][1] = false;
    			greenStatus[i][2] = false;
    			greenStatus[i][3] = false;
    		}
    	}
    	
    	//instantiate track
    	authorityRedCorrection = new Boolean[redTrackSize];
    	for (int i=0; i<redTrackSize; i++) authorityRedCorrection[i] = false;
    	authorityGreenCorrection = new Boolean[greenTrackSize];
    	for (int i=0; i<greenTrackSize; i++) authorityGreenCorrection[i] = false;
    	redPosition = new Boolean[redTrackSize];
    	greenPosition = new Boolean[greenTrackSize];
    	controller = new trackController(redTrackSize, greenTrackSize);
    	
    	//set crossing and switch existence
    	for (int i=0; i<redTrackSize; i++) {
    		boolean switchExistence = false;
    		boolean crossingExistence = false;
    		if (redConnectivity[i][0] > 0) switchExistence = true;
    		controller.setSwitchExistence("red", i, switchExistence);
    		if (redConnectivity[i][1] == 1) crossingExistence = true;
    		controller.setCrossingExistence("red", i, crossingExistence);
    		redPosition[i] = false;
    	}
    	for (int i=0; i<greenTrackSize; i++) {
    		boolean switchExistence = false;
    		boolean crossingExistence = false;
    		if (greenConnectivity[i][0] > 0) switchExistence = true;
    		controller.setSwitchExistence("green", i, switchExistence);
    		if (greenConnectivity[i][1] == 1) crossingExistence = true;
    		controller.setCrossingExistence("green", i, crossingExistence);
    		greenPosition[i] = false;
    	}
    	
    	//instantiate GUIs 
	    login = new loginGUI();
	    while (login.frame.isShowing()) {
	    	//prompt user for login credentials
	    	System.out.println("Please enter login credentials to access Wayside Control Center.");
	    	//implement delay and prompt user again if no action is taken
	    	for (int delay=-1000000000; delay<1000000000; delay++)
	    		for (int doubleDelay=-1000000000; doubleDelay<1000000000; doubleDelay++);
	    }
	    System.out.println("Thank You");
	    controllerGUI = new trackControllerGUI();
	    
	    //create backup in case server crashes
	    boolean[][] redStatusBackup = redStatus;
	    boolean[][] greenStatusBackup = greenStatus;
	    Boolean[] redPositionBackup = redPosition;
	    Boolean[] greenPositionBackup = greenPosition;
	 
	    
	    while(true) {
	    	//update backups
	    	redStatusBackup = redStatus;
	    	greenStatusBackup = greenStatus;
	    	redPositionBackup = redPosition;
	    	greenPositionBackup = greenPosition;
	    	
	    	//receive inputs
	    	if (FULLSYSTEM) {
    			nodes = receive("CTCOfficeTrainNodes");
    			redToYard = (Boolean) receive("TrackModelRedToYard");
		    	greenToYard = (Boolean) receive("TrackModelGreenToYard");
		    	Object redMaint = receive("CTCOfficeRedMaintenance");
		    	Object greenMaint = receive("CTCOfficeGreenMaintenance");
		    	redMaintenance = (Boolean[])redMaint;
		    	greenMaintenance = (Boolean[])greenMaint;
		    	Object redPos = receive("CTCOfficeRedPosition");
		    	Object greenPos = receive("CTCOfficeGreenPosition");
		    	//check for server crash
		    	if (redPos != null) 
		    		redPosition = (Boolean[])redPos;
		    	else redPosition = redPositionBackup;
		    	if (greenPos != null)
		    		greenPosition = (Boolean[])greenPos;
		    	else greenPosition = greenPositionBackup; 
		    	Object redStat = receive("redStatus");
		    	Object greenStat = receive("greenStatus");
		    	//check for server crash
	        	if (redStat != null) {
	        		System.out.println("Red Status Received");
	        		redStatus = (boolean[][])redStat;
	        	}
	        	else redStatus = redStatusBackup;
		    	if (greenStat != null) {
		    		System.out.println("Green Status Received");
		    		greenStatus = (boolean[][])greenStat;
		    	}
		    	else greenStatus = greenStatusBackup;
	    	}
	    	if (nodes != null) System.out.println("Dispatch Recieved");
	    	else System.out.println("Waiting for Dispatch");
	    	
	    	//set variables
	    	if (redToYard != null) controller.redToYard = redToYard;
	    	if (greenToYard != null) controller.greenToYard = greenToYard;
	    	for (int i=1; i<redTrackSize; i++) {
	    		controller.setTrainPresence("red", i, redStatus[i][0]);
	    		controller.setTrackStatus("red", i, (redStatus[i][1] || redStatus[i][2] || redStatus[i][3]));
	    		if (redMaintenance!=null && redMaintenance[i]!=null) {
	    			controller.setTrackStatus("red", i, (controller.getTrackStatus("red", i) || redMaintenance[i]));
	    			//if in maintenance mode, check for CTC office switch change
	    			if (redMaintenance[i]) controller.setSwitchPosition("red", i, redPosition[i]);
	    			else redPosition[i] = controller.getSwitchPosition("red", i);
	    		}
	    		else redPosition[i] = controller.getSwitchPosition("red", i);
	    	}
	    	for (int i=1; i<greenTrackSize; i++) {
	    		controller.setTrainPresence("green", i, greenStatus[i][0]);
	    		controller.setTrackStatus("green", i, (greenStatus[i][1] || greenStatus[i][2] || greenStatus[i][3]));
	    		if (greenMaintenance!=null && greenMaintenance[i]!=null) {
	    			controller.setTrackStatus("green", i, (controller.getTrackStatus("green", i) || greenMaintenance[i]));
	    			//if in maintenance mode, check for CTC office switch change
	    			if (greenMaintenance[i]) controller.setSwitchPosition("green", i, greenPosition[i]);
	    			else greenPosition[i] = controller.getSwitchPosition("green", i);
	    		}
	    		else greenPosition[i] = controller.getSwitchPosition("green", i);
	    	}
	    	
	    	//check safety;
	    	checkTrackSafety();
	    	if (controller.redStop) {
				authorityRedCorrection[controller.stop] = true;
				controller.redStop = false;
			}
			if (controller.greenStop) {
				authorityGreenCorrection[controller.stop] = true;
				controller.greenStop = false;
			}
	    	
	        //send outputs
	    	if (FULLSYSTEM) {
	    		if (nodes != null) send("TrackControllerTrainNodes", nodes);
		    	if (redPosition != null) send("TrackControllerRedPosition", redPosition);
		    	if (redStatus != null) send("TrackControllerRedStatus", redStatus);
		    	if (greenPosition != null) send("TrackControllerGreenPosition", greenPosition);
		    	if (greenStatus != null) send("TrackControllerGreenStatus", greenStatus);
		    	if (redMaintenance != null) send("TrackControllerRedMaintenance", redMaintenance);
		    	if (greenMaintenance != null) send("TrackControllerGreenMaintenance", greenMaintenance);		    	
		    	send("authorityRedCorrection", authorityRedCorrection);
		    	send("authorityGreenCorrection", authorityGreenCorrection);
	    	}
			
			//runPLCs
			controllerGUI.run(); 
	    }
    }
    
    public static void checkTrackSafety () { 
    	//check for red backups
    	boolean stillBackedup = false; 
    	for (int i=1; i<redTrackSize-2; i++) {
    		//change authority if safety train present or track failure    		
    		if (controller.getTrainPresence("red", i)&&(controller.getTrainPresence("red",i+1) || controller.getTrackStatus("red",i+1) || controller.getTrainPresence("red",i-1) || controller.getTrackStatus("red",i-1))) {
    			if ((controller.getTrainPresence("red",i+1)&&controller.getTrackStatus("red",i-1)) || (controller.getTrainPresence("red",i-1)&&controller.getTrackStatus("red",i+1))) {
        			redBackedup = true;
        			redFault = i;
        			stillBackedup = true;
        		}
    			else if (i==redFault && redBackedup && !controller.getTrackStatus("red", redFault+1) && !controller.getTrackStatus("red", redFault-1)) {
    				authorityRedCorrection[i] = false; 
    				stillBackedup = true;
    			}
    			else if (redBackedup && ((controller.getTrainPresence("red",i+1) && !controller.getTrainPresence("red",i-1)) || (controller.getTrainPresence("red",i-1) && !controller.getTrainPresence("red",i+1)))) {
    				authorityRedCorrection[i] = false;
    				stillBackedup = true;
    			}
    			else authorityRedCorrection[i] = true;
    		}
    		else authorityRedCorrection[i] = false;
    	}
    	redBackedup = stillBackedup;
    	//reset to check for green backups
    	stillBackedup = false;
    	for (int i=1; i<greenTrackSize-2; i++) {
    		//change authority if safety train present or broken track
    		if (controller.getTrainPresence("green", i)&&(controller.getTrainPresence("green",i+1) || controller.getTrackStatus("green",i+1) || controller.getTrainPresence("green",i-1) || controller.getTrackStatus("green",i-1))) {
    			if ((controller.getTrainPresence("green",i+1)&&controller.getTrackStatus("green",i-1)) || (controller.getTrainPresence("green",i-1)&&controller.getTrackStatus("green",i+1))) {
        			greenBackedup = true;
        			greenFault = i;
        			stillBackedup = true;
        		}
    			else if (i==greenFault && greenBackedup && !controller.getTrackStatus("green", greenFault+1) && !controller.getTrackStatus("green", greenFault-1)) {
    				authorityGreenCorrection[i] = false; 
    				stillBackedup = true;
    			}
    			else if (greenBackedup && ((controller.getTrainPresence("green",i+1) && !controller.getTrainPresence("green",i-1)) || (controller.getTrainPresence("green",i-1) && !controller.getTrainPresence("green",i+1)))) {
    				authorityGreenCorrection[i] = false;
    				stillBackedup = true;
    			}
    			else authorityGreenCorrection[i] = true;
    		}
    		else authorityGreenCorrection[i] = false;
    	}
    	greenBackedup = stillBackedup;
    }
    
    static boolean send(String key, Object value) {
        if (!connected) {            connectClient();
        }
        try {
            mini.serverSend(key, value);
            return true;
        } catch (Exception e) {
            System.out.printf("Failed to set %s to %s\n", key, value);
            return false;
        }
    }
    static Object receive(String key) {
        if (!connected) {
            connectClient();
        }
        try {
            return mini.serverReceive(key);
        } catch (Exception e) {
            System.out.printf("Failed to receive %s\n", key);
            System.out.println(e);
        }
        return null;
    }
    private static void connectClient() {
        if (!connected) {
            try {
                Registry registry = LocateRegistry.getRegistry(cpuIP, PORTNUMBER);
                mini = (ServerInterface) registry.lookup("ServerInterface");
                connected = true;
            } catch (Exception e) {
                System.err.println("Client exception: " + e.toString());
                e.printStackTrace();
            }
        }
    }
    static void call(String key, Object...argv) {
        if (!connected) {
            connectClient();
        }
        try {
            mini.serverCall(key, argv);
        } catch (Exception e) {
            System.err.println("Client exception: " + e.toString());
            e.printStackTrace();
        }
    }
    static Object[] getCall(String key) {
        if (!connected) {
            connectClient();
        }
        try {
            return mini.serverGetCall(key);
        } catch (Exception e) {
            System.err.println("Client exception: " + e.toString());
            e.printStackTrace();
            return null;
        }
    }
}


