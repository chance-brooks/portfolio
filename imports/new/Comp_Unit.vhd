library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
USE ieee.numeric_std.all;

entity Comp_Unit is
    Generic (
        n   : positive := 32);
    Port ( 
        A_31    : IN     std_logic;
		B_31    : IN     std_logic;
		S_31    : IN     std_logic;
		CO      : IN     std_logic;
        Op      : IN     std_logic_vector (1 DOWNTO 0);
        R       : OUT    std_logic_vector (n-1 DOWNTO 0)
        );
        
end Comp_Unit;

architecture struct of Comp_Unit is
    
    signal temp : std_logic_vector(5 downto 0);
    
begin

    temp <= (Op & A_31 & B_31 & S_31 & CO);
    R <= x"00000001" when std_match(temp, "10001-") else
         x"00000001" when std_match(temp, "10111-") else
         x"00000001" when std_match(temp, "1010--") else
         x"00000001" when std_match(temp, "11---0") else
         x"00000000";

end struct;
