package project;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;


public class loginGUI extends trackController {

	//declare elements
	public static JFrame frame;
	private static JPanel panel;
	private static JLabel title;
	private static JLabel user;
	private static JLabel pass;
	private static JTextField userID;
	private static JPasswordField passText;
	private static JButton login;
	private static JLabel support;
	
	public loginGUI() {
		
		//create elements
		frame = new JFrame();
		panel = new JPanel();
		title = new JLabel("Wayside Control Center");
		user = new JLabel("Please enter user ID:");
		pass = new JLabel("Please enter password:");
		userID = new JTextField(20);
		passText = new JPasswordField(20);
		login = new JButton("Login");
		support = new JLabel("Contact Support: (724) 757-9356");
		
		//configure frame
		frame.setSize(1200, 1000);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setTitle("Login");;
		frame.setVisible(true);

		//configure panel
		frame.add(panel, BorderLayout.CENTER);
		panel.setLayout(null);
		
		//configure elements
		//title
		title.setBounds(250,100,700,100);
		title.setFont(new Font("TimesRoman", Font.BOLD, 64));
		panel.add(title);
		//userID
		user.setBounds(300, 400, 200, 25);
		panel.add(user);
		userID.setBounds(700, 400, 200, 25);
		panel.add(userID);
		//password
		pass.setBounds(300, 500, 200, 25);
		panel.add(pass);
		passText.setBounds(700,500, 200, 25);
		panel.add(passText);
		//support
		support.setForeground(Color.BLUE.darker());
		support.setBounds(950, 900, 200, 25);
		panel.add(support);
		
		//login button
		login.setBounds(300, 550, 80, 25);
		login.setActionCommand("Login");
		login.addActionListener(e -> {
			frame.dispose();
		});
		panel.add(login);
	}
}
