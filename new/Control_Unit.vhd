library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity Control_Unit is
    generic (
		n : positive := 32
	);
    port (
	    clk            : in  std_logic;
	    rst            : in std_logic;
	    Op             : in std_logic_vector(5 downto 0);
	    Func           : in std_logic_vector(5 downto 0);
	    done           : in std_logic;
	    RegAMSB        : in std_logic;
	    PCWriteCond    : out std_logic;
	    PCWrite        : out std_logic;
	    IorD           : out std_logic;
	    MemRead        : out std_logic;
	    MemWrite       : out std_logic;
	    MemtoReg       : out std_logic_vector(2 downto 0);
	    IRWrite        : out std_logic;
	    PCSource       : out std_logic_vector(1 downto 0);
	    ALUOp          : out std_logic_vector(5 downto 0);
	    ALUSrcA        : out std_logic_vector(1 downto 0);
	    ALUSrcB        : out std_logic_vector(1 downto 0);
	    RegWrite       : out std_logic;
	    RegDst         : out std_logic_vector(1 downto 0);
	    RegA           : out std_logic;
	    RegB           : out std_logic;
	    ALUout         : out std_logic;    
	    MemEn          : out std_logic;
	    MemSrc         : out std_logic_vector(1 downto 0);
	    MultRst        : out std_logic;
	    Hi             : out std_logic;
	    Lo             : out std_logic;
	    state          : out integer
	    );
end Control_Unit;

architecture Behavioral of Control_Unit is
    -- Build an enumerated type for the state machine
	type state_type is (InF, PC_INCR, DECODE, EXE, R_TYPE_COMP, J_TYPE_COMP, BRANCH_COMP, LD_ACCESS, MEM_COMP, SW_ACCESS, WAIT_ST);
	
	-- Register to hold the current state
	signal pr_state, nx_state   : state_type;

begin
    process (clk, rst) 
	begin
		if rst = '1' then
			pr_state <= InF;
		elsif (clk'event AND clk = '1') then
		    pr_state <= nx_state;
		end if;
	end process;

    process(pr_state, Op, Func, done)
	begin 
        case pr_state is
        
            when InF =>
                PCWrite <= '0';
                RegWrite <= '0';
                PCWriteCond <= '0';
                MemWrite <= '0';
                Hi <= '0';
                Lo <= '0';
                
                IRWrite <= '1';
                IorD <= '0';
                state <= 1;
                nx_state <= PC_INCR; 
              
            when PC_INCR =>
                IRWrite <= '0';
                
                RegA <= '1';     --loading for future states
                RegB <= '1';     --loading for future states
                MultRst <= '1';
                ALUSrcA <= "00";
                ALUSrcB <= "01";
                ALUOp <= "000000";
                PCSource <= "00";
                PCWrite <= '1';
                state <= 2;
                nx_state <= DECODE;
                 
            when DECODE =>
                RegA <= '0';
                RegB <= '0';
                MultRst <= '0';
                PCWrite <= '0';
                state <= 3;
            
                --J-Type
                if (Op = "000010") then                             --J
                    PCSource <= "10";
                    nx_state <= J_TYPE_COMP;
                    
                elsif (Op = "000000" AND Func = "001000") then      --JR
                    PCSource <= "11";
                    nx_state <= J_TYPE_COMP;
                    
                elsif (Op = "000101") then                          --BNE
                    --Add offset to PC
                    ALUSrcA <= "00";
                    ALUSrcB <= "11";
                    ALUOp <= "000000";
                    ALUOut <= '1';
                    nx_state <= BRANCH_COMP;
                    
                elsif (Op = "000001") then                          --BLTZAL
                    if (RegAMSB = '1') then
                        ALUSrcA <= "00";
                        ALUSrcB <= "11";
                        ALUOp <= "000000";
                        ALUOut <= '1';
                        RegDst <= "10";
                        MemtoReg <= "101";
                        RegWrite <= '1';
                        nx_state <= BRANCH_COMP;
                    else 
                        nx_state <= InF;
                    end if;
                    
                --Mult
                elsif (Op = "000000" AND Func = "011001") then
                    MultRst <= '0';
                    nx_state <= WAIT_ST;
                    
                --MFHI
                elsif (Op = "000000" AND Func = "010000") then
                    MemtoReg <= "011";
                    RegDst <= "01";
                    RegWrite <= '1';
                    nx_state <= InF;
                    
                --MFLO
                 elsif (Op = "000000" AND Func = "010010") then
                    MemtoReg <= "100";
                    RegDst <= "01";
                    RegWrite <= '1';
                    nx_state <= InF;
                    
                --R-Type
                elsif (Op = "000000") then
                    if (Func = "000100" or Func = "000011") then ALUSrcA <= "10";
                    else ALUSrcA <= "01";
                    end if;
                    ALUSrcB <= "00";
                    ALUOp <= "000001";
                    MemtoReg <= "000";
                    RegDst <= "01";
                    state <= 4;
                    nx_state <= EXE;
                
                --I-Type
                elsif (Op = "001000" OR Op = "001101" OR Op = "001010") then
                    ALUSrcA <= "01";
                    ALUSrcB <= "10";
                    ALUOp <= Op;
                    MemtoReg <= "000";
                    RegDst <= "00";
                    nx_state <= EXE;
                    
                --LD/SW
                elsif (Op = "100000" OR Op = "100001" OR Op = "001111" OR Op = "100011" OR Op = "101011") then
                    if (Op = "100000") then MemSrc <= "00";     --LB
                    elsif (Op = "100001") then MemSrc <= "01";  --LHW
                    elsif (Op = "001111") then MemSrc <= "10";  --LUI
                    elsif (Op = "100011") then MemSrc <= "11";  --LW
                    end if;
                    ALUSrcA <= "01";
                    ALUSrcB <= "10";
                    ALUOp <= "000000";
                    ALUOut <= '1';
                    IorD <= '1';
                    if (Op = "101011") then 
                        nx_state <= SW_ACCESS;
                    else
                        nx_state <= LD_ACCESS;
                    end if;
                    
                --CLO
                elsif (Op = "011100" AND Func = "100001") then
                    RegDst <= "01";
                    MemtoReg <= "010";
                    RegWrite <= '1';
                    nx_state <= Inf;
                end if;
                
            when EXE =>
                ALUout <= '1';
                MemtoReg <= "000";
                state <= 5;
                nx_state <= R_TYPE_COMP;
                
            when LD_ACCESS =>
                ALUOut <= '0';
                
                MemEn <= '1';
                MemtoReg <= "001";
                nx_state <= MEM_COMP;
                
            when SW_ACCESS =>
                ALUOut <= '0';
                
                IorD <= '1';
                MemWrite <= '1';
                nx_state <= InF;
                
            when WAIT_ST =>
                if (done = '0') then
                    nx_state <= WAIT_ST;
                else
                    Hi <= '1';
                    Lo <= '1';
                    nx_state <= InF;
                end if;
                
            when R_TYPE_COMP =>
                ALUOut <= '0';
                
                RegWrite <= '1';
                state <= 6;
                nx_state <= InF;                    

            when J_TYPE_COMP =>
                PCWrite <= '1';
                nx_state <= InF;      
                
            when BRANCH_COMP =>
                ALUOut <= '0';
                RegWrite <= '0';
                
                PCSource <= "01";
                ALUSrcA <= "01";
                ALUSrcB <= "00";
                ALUOp <= "000010";
                if (Op = "000101") then
                    PCWriteCond <= '1';
                elsif (Op = "000001") then
                    PCWrite <= '1';
                end if;
                nx_state <= InF;
                
            when MEM_COMP =>
                MemEn <= '0';
                
                RegWrite <= '1';
                MemtoReg <= "001";
                RegDst <= "00";
                nx_state <= InF;
                    
            end case;
    end process;
end Behavioral;
