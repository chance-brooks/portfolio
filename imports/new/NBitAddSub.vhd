library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity NBitAddSub is
	generic (
		WIDTH : positive := 64
	);
	port (
	    K     : in  std_logic; -- 0 for +, 1 for -
		A     : in  std_logic_vector(WIDTH-1 downto 0);
		B     : in  std_logic_vector(WIDTH-1 downto 0);
		S     : out std_logic_vector(WIDTH-1 downto 0);
		Cout  : out std_logic
	);
end entity;

architecture rtl of NBitAddSub is
component Full_Adder is
    port (
		A     : in  std_logic;
		B     : in  std_logic;
		Cin   : in  std_logic;
		S     : out std_logic;
		Cout  : out std_logic
	);
end component;
signal OP: std_logic_vector(WIDTH-1 downto 0);
signal carry: std_logic_vector(WIDTH-1 downto 0);

begin
    OP(0) <= B(0) XOR K;
	U0: Full_Adder port map(A(0), OP(0), K, S(0), carry(0));
	
	L1: FOR i IN 1 TO WIDTH-1 GENERATE
	   OP(i) <= B(i) XOR K;
	   U1: Full_Adder port map(A(i), OP(i), carry(i-1), S(i), carry(i)); 
	END GENERATE;
	
	Cout <= carry(WIDTH-1);
end architecture;
