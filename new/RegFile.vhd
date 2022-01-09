library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity RegFile is
	generic (
		n : positive := 32
	);
	port (
	    clk           : in  std_logic;
	    rst           : in std_logic;
	    RegWrite      : in  std_logic;
	    ReadReg1      : in std_logic_vector(4 downto 0);
	    ReadReg2      : in std_logic_vector(4 downto 0);
	    WriteReg      : in std_logic_vector(4 downto 0);
		WriteData     : in  std_logic_vector(n-1 downto 0);
		ReadData1     : out std_logic_vector(n-1 downto 0);
		ReadData2     : out std_logic_vector(n-1 downto 0)
	);
end entity;

architecture rtl of RegFile is

    type reg is array (n-1 downto 0) of std_logic_vector(n-1 downto 0);
    signal regArray : reg; 

begin        
    ReadData1 <= regArray(to_integer(unsigned(ReadReg1)));
    ReadData2 <= regArray(to_integer(unsigned(ReadReg2))); 

    CLKD : process(clk, rst)
    begin
        if(rst = '1') then
           for i in 0 to n-1 loop
                regArray(i) <= (others => '0');
           end loop;
        elsif(clk'event AND clk = '1') then         
           if(RegWrite = '1') then
              regArray(to_integer(unsigned(WriteReg))) <= WriteData;
           end if;
        end if;
    end process CLKD;
end architecture;
