library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
USE ieee.numeric_std.all;

entity Shift_Unit is
    Generic (
        n   : positive := 32);
    Port ( 
        A       : IN     std_logic_vector (n-1 DOWNTO 0);
        SHAMT   : IN     std_logic_vector (4 DOWNTO 0);
        Op      : IN     std_logic_vector (1 DOWNTO 0);
        R       : OUT    std_logic_vector (n-1 DOWNTO 0)
        );
        
end Shift_Unit;

architecture struct of Shift_Unit is

begin

process(A,SHAMT,Op)
    variable SHAMTint : integer;
begin
    
    SHAMTint := to_integer(unsigned(SHAMT));
    
    if (Op(1) = '0') then
        R <= (others => '0');
        R(n-1 downto SHAMTint) <= A(n-1-SHAMTint downto 0);
    else
        R <= (others => Op(0) and A(n-1));
        R(n-1-SHAMTint downto 0) <= A(n-1 downto SHAMTint);
    end if;
end process;
end struct;