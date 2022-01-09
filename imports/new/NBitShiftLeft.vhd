library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity NBitShiftLeft is
	generic (
		n : positive := 64
	);
	port (
	    CLK   : in  std_logic;
		D     : in  std_logic_vector(n-1 downto 0);
		EN    : in  std_logic;
		RST   : in  std_logic;
		Load  : in  std_logic;
		Q     : inout std_logic_vector(n-1 downto 0);
		S     : in  std_logic
	);
end entity;

architecture rtl of NBitShiftLeft is
begin
    CLKD : process(CLK, RST)
    begin
        if(RST = '1') then
           Q <= (others => '0');
           
        elsif(CLK'event AND CLK = '1') then
           if (Load = '1') then
                if (EN = '1') then
                    Q <= D;
                end if;
           else
                if (EN = '1') then
                    Q(n-1 downto 1) <= Q(n-2 downto 0);
                    Q(0) <= S; 
                end if;
           end if;
        end if;
    end process CLKD;
end architecture;
