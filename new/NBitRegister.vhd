library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity NBitRegister is
	generic (
		n : positive := 32
	);
	port (
	    CLK   : in  std_logic;
		D     : in  std_logic_vector(n-1 downto 0);
		EN    : in  std_logic;
		RST   : in std_logic;
		Q     : out std_logic_vector(n-1 downto 0)
	);
end entity;

architecture rtl of NBitRegister is
begin
    CLKD : process(CLK, RST)
    begin
        if(RST = '1') then
           Q <= (others => '0');
        elsif(CLK'event AND CLK = '1') then
           if(EN = '1') then
              Q <= D;
           end if;
        end if;
    end process CLKD;
end architecture;
