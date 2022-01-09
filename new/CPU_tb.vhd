

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;


entity CPU_tb is
    generic (
		n : positive := 32
	);
    Port ( clock : in STD_LOGIC;
           reset : in STD_LOGIC);
end CPU_tb;

architecture Behavioral of CPU_tb is

    signal MemWrite : std_logic;
    signal Address, DataIn, DataOut : std_logic_vector(n-1 downto 0);
    signal state : integer;

    component CPU is 
        Port ( 
           MemOut       : in std_logic_vector(n-1 downto 0);
           clk          : in std_logic;
           rst          : in std_logic;
           MemIn        : out std_logic_vector(n-1 downto 0);
           MemWriteData : out std_logic_vector(n-1 downto 0);
           MemWrite     : out std_logic;
           state        : out integer
           );
    end component;
    
    component CPU_memory IS
        PORT( 
            Clk      : IN     std_logic;
            MemWrite : IN     std_logic;
            addr     : IN     std_logic_vector (31 DOWNTO 0);
            dataIn   : IN     std_logic_vector (31 DOWNTO 0);
            dataOut  : OUT    std_logic_vector (31 DOWNTO 0)
        );

    END component;

begin
    U_0 : CPU
        port map (
            MemOut => DataOut,
            clk => clock,
            rst => reset,
            MemIn => address,
            MemWriteData => DataIn,
            MemWrite => MemWrite,
            state => state);

    U_1 : CPU_memory
        port map (
            Clk => clock,
            MemWrite => MemWrite,
            addr => Address,
            dataIn => DataIn,
            dataOut => DataOut);
            
    
            

end Behavioral;
