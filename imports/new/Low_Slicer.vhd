----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 10/14/2021 06:28:18 PM
-- Design Name: 
-- Module Name: Low_Slicer - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity Low_Slicer is
    Port ( A : in STD_LOGIC_VECTOR (63 downto 0);
           R : out STD_LOGIC_VECTOR (31 downto 0));
end Low_Slicer;

architecture Behavioral of Low_Slicer is

begin
    R <= A(31 downto 0);
end Behavioral;
