
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity ALU is
    Generic (
        n   : positive := 32);
    Port ( A : in STD_LOGIC_VECTOR (n-1 downto 0);
           B : in STD_LOGIC_VECTOR (n-1 downto 0);
           ALUOp : in STD_LOGIC_VECTOR (3 downto 0);
           SHAMT : in STD_LOGIC_VECTOR (4 downto 0);
           R : out STD_LOGIC_VECTOR (n-1 downto 0);
           Zero : out STD_LOGIC;
           Overflow : out STD_LOGIC);
end ALU;

architecture Struct of ALU is
    signal LogOUT, ArithOUT, CompOUT, ShiftOUT : std_logic_vector(n-1 downto 0);
    signal ArithCarry : std_logic;

    component Log_Unit is 
	   port (
	       A   : in std_logic_vector(n-1 downto 0);
	       B   : in std_logic_vector(n-1 downto 0);
	       Op  : in std_logic_vector(1 downto 0);
	       R   : out std_logic_vector(n-1 downto 0));
    end component;
    
    component Arith_Unit is
       port ( 
          A       : IN     std_logic_vector (n-1 DOWNTO 0);
          B       : IN     std_logic_vector (n-1 DOWNTO 0);
          C_op    : IN     std_logic_vector (1 DOWNTO 0);
          CO      : OUT    std_logic;
          OFL     : OUT    std_logic;
          S       : OUT    std_logic_vector (n-1 DOWNTO 0);
          Z       : OUT    std_logic);
    end component;
    
    component Comp_Unit is 
	   port (
	      A_31    : IN     std_logic;
		  B_31    : IN     std_logic;
		  S_31    : IN     std_logic;
		  CO      : IN     std_logic;
          Op      : IN     std_logic_vector (1 DOWNTO 0);
          R       : OUT    std_logic_vector (n-1 DOWNTO 0));
    end component;
    
    component Shift_Unit is 
	   port (
	       A        : in std_logic_vector(n-1 downto 0);
	       SHAMT   : in std_logic_vector(4 downto 0);
	       Op       : in std_logic_vector(1 downto 0);
	       R        : out std_logic_vector(n-1 downto 0));
    end component;

begin
    U1 : Log_Unit
        port map (
            A => A,
            B => B,
            Op => ALUOp(1 downto 0),
            R => LogOUT);
            
    U2 : Arith_Unit
        port map (
            A => A,
            B => B,
            C_op => ALUOp(1 downto 0),
            CO => ArithCarry,
            OFL => Overflow,
            S => ArithOUT,
            Z => Zero);
            
    U3 : Comp_Unit
        port map (
            A_31 => A(n-1),
            B_31 => B(n-1),
            S_31 => ArithOUT(n-1),
            CO => ArithCarry,
            Op => ALUOp(1 downto 0),
            R => CompOUT);
            
    U4 : Shift_Unit
        port map (
            A => A,
            SHAMT => SHAMT,
            Op => ALUOp(1 downto 0),
            R => ShiftOUt);

    WITH ALUOp(3 downto 2) SELECT
        R <= LogOUT when "00",
             ArithOUT when "01",
             CompOUT when "10",
             ShiftOUT when others;
end Struct;
