library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity Log_Unit is
    Generic (
        n   : positive := 32);
    Port ( 
        A       : IN     std_logic_vector (n-1 DOWNTO 0);
        B       : IN     std_logic_vector (n-1 DOWNTO 0);
        Op      : IN     std_logic_vector (1 DOWNTO 0);
        R       : OUT    std_logic_vector (n-1 DOWNTO 0)
        );
end Log_Unit;

architecture struct of Log_Unit is

begin

    WITH Op SELECT
        R <= A AND B when "00",
             A OR B when "01",
             A XOR B when "10",
             A NOR B when others;
end struct;
