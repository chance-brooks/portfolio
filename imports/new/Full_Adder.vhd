library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity Full_Adder is
	port (
		A     : in  std_logic;
		B     : in  std_logic;
		Cin   : in  std_logic;
		S     : out std_logic;
		Cout  : out std_logic
	);
end entity;

architecture rtl of Full_Adder is
begin
	S <= (A XOR B) XOR Cin;
	Cout <= (A AND (Cin OR B)) OR (Cin AND B);
end architecture;

