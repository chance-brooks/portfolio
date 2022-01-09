library IEEE;
use IEEE.STD_LOGIC_1164.ALL;


entity ALU_Control is
    generic (
		n : positive := 32
	);
    Port ( clk          : in STD_LOGIC;
           Func         : in STD_LOGIC_VECTOR (5 downto 0);
           ALUOp        : in STD_LOGIC_VECTOR(5 downto 0);
           RegA         : in STD_LOGIC_VECTOR (n-1 downto 0);
           ALUOpCode    : out STD_LOGIC_VECTOR (3 downto 0));
end ALU_Control;

architecture Behavioral of ALU_Control is

begin
    process(Func, ALUOp)
	begin
	   --increament PC
	   if (ALUOp = "000000") then
	       ALUOpCode <= "0100";
	   end if;
	   
	   --R-type instructions
	   if (ALUOp = "000001") then 
            if (Func = "100001") then       --ADDU
                ALUOpCode <= "0101";
            elsif (Func = "100010") then    --SUB
                ALUOpCode <= "0110";
            elsif (Func = "100100") then    --AND
                ALUOpCode <= "0000";
            elsif (Func = "000011") then    --SRA
                ALUOpCode <= "1111";
            elsif (Func = "000000") then    --SLL
                ALUOpCode <= "1100";
            elsif (Func = "000100") then     --SLLV
                ALUOpCode <= "1100";
            end if;
        
        --I-Type Instructions
        elsif (ALUOp = "001000") then       --ADDI
            ALUOpCode <= "0100";
        elsif (ALUOp = "001101") then       --ORI
            ALUOpCode <= "0001";
        elsif (ALUOp = "001010") then       --SLTI
            ALUOpCode <= "1010";
            
        --J-Type Instructions
        elsif (ALUOp = "000010") then       --BNE
            ALUOpCode <= "0110";
        end if;
    end process;
end Behavioral;
