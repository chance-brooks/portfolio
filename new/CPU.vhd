library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;


entity CPU is
    generic (
		n : positive := 32
	);
    Port ( MemOut       : in std_logic_vector(n-1 downto 0);
           clk          : in std_logic;
           rst          : in std_logic;
           MemIn        : out std_logic_vector(n-1 downto 0);
           MemWriteData : out std_logic_vector(n-1 downto 0);
           MemWrite     : out std_logic;
           state        : out integer
           );
end CPU;

architecture Behavioral of CPU is

    signal PCWriteCond, PCWrite, IorD, MemRead, IRWrite, MemEn, RegWrite : std_logic;
    signal RegA, RegB, ALUOut, zero, overflow, PCEn, MultRst, Hi, Lo, done: std_logic;
    
    signal PCSource, ALUSrcA, ALUSrcB, MemSrc, RegDst : std_logic_vector(1 downto 0);
    signal MemtoReg : std_logic_vector(2 downto 0);
    signal ALUOpCode : std_logic_vector(3 downto 0);
    signal WriteReg, SHAMT : std_logic_vector(4 downto 0);
    signal ALUOp : std_logic_vector(5 downto 0);
    signal PCin, PC : std_logic_vector(n-1 downto 0);
    signal Instruction, WriteData, ReadData1, ReadData2, RegAOut, RegBOut : std_logic_vector(n-1 downto 0);
    signal ALUAIn, ALUBIn, ALUResult, ALUOutput : std_logic_vector(n-1 downto 0);  
    signal Immediate, MemDataIn, MemData, LeadingOnes, HiOut, LoOut: std_logic_vector(n-1 downto 0);
    signal Product : std_logic_vector(63 downto 0);
    
    component ALU is
        port (
           A        : in STD_LOGIC_VECTOR (n-1 downto 0);
           B        : in STD_LOGIC_VECTOR (n-1 downto 0);
           ALUOp    : in STD_LOGIC_VECTOR (3 downto 0);
           SHAMT    : in STD_LOGIC_VECTOR (4 downto 0);
           R        : out STD_LOGIC_VECTOR (n-1 downto 0);
           Zero     : out STD_LOGIC;
           Overflow : out STD_LOGIC
        );
    end component; 
    
    component ALU_Control is
        port (
           clk          : in STD_LOGIC;
           Func         : in STD_LOGIC_VECTOR (5 downto 0);
           ALUOp        : in STD_LOGIC_VECTOR(5 downto 0);
           RegA         : in STD_LOGIC_VECTOR (n-1 downto 0);
           ALUOpCode    : out STD_LOGIC_VECTOR (3 downto 0)
        );
    end component;
    
    component Control_Unit is 
        port (
            clk            : in std_logic;
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
    end component;
    
    component NBitRegister is 
        port (
            CLK   : in  std_logic;
            D     : in  std_logic_vector(n-1 downto 0);
            EN    : in  std_logic;
            RST   : in std_logic;
            Q     : out std_logic_vector(n-1 downto 0)
        );
    end component; 
    
    component RegFile is
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
    end component; 
    
    component Multiplier is 
        port (
            A       : in STD_LOGIC_VECTOR (31 downto 0);
            B       : in STD_LOGIC_VECTOR (31 downto 0);
            clk     : in STD_LOGIC;
            rst     : in STD_LOGIC;
            R       : out STD_LOGIC_VECTOR (63 downto 0);
            done    : out STD_LOGIC
        );
    end component;
begin

    Ctrl : Control_Unit
        port map (
            clk => clk,
            rst => rst,
            Op => Instruction(31 downto 26),
            Func => Instruction(5 downto 0),
            done => done,
            RegAMSB => RegAOut(n-1),
            PCWriteCond => PCWriteCond,
            PCWrite => PCWrite,
            IorD => IorD,
            MemRead => MemRead,  
            MemWrite => MemWrite,
            MemtoReg => MemtoReg,
            IRWrite => IRWrite,
            PCSource => PCSource,
            ALUOp => ALUOp,
            ALUSrcA => ALUSrcA,
            ALUSrcB => ALUSrcB,
            RegWrite => RegWrite,
            RegDst => RegDst,
            RegA => RegA,
            RegB => RegB,  
            ALUout => ALUOut,
            MemEN => MemEn,
            MemSrc => MemSrc,
            MultRst => MultRst,
            Hi => Hi,
            Lo => Lo,
            state => state);
            
    ALUCtrl : ALU_Control
        port map (
            clk => clk,
            Func => Instruction(5 downto 0),
            ALUOp => ALUOp,
            RegA => RegAOut,
            ALUOpCode => ALUOpCode);
            
    ALUComp : ALU
        port map (
            A => ALUAIn,
            B => ALUBIn,
            ALUOp => ALUOpCode,
            SHAMT => SHAMT,
            R => ALUResult,
            Zero => zero,
            Overflow => overflow);
            
    MultComp : Multiplier
        port map (
            A => RegAOut,
            B => RegBOut,
            clk => clk,
            rst => MultRst,
            R => Product,
            done => done);
        
    Regs : RegFile
        port map (
            clk => clk,
            rst => rst,
            RegWrite => RegWrite,
            ReadReg1 => Instruction(25 downto 21),
            ReadReg2 => Instruction(20 downto 16),
            WriteReg => WriteReg,
            WriteData => WriteData,
            ReadData1 => ReadData1, 
            ReadData2 => ReadDAta2);
            
    ProgCount : NBitRegister
        port map (
            CLK => clk,
            D => PCIn,
            EN => PCEn,
            RST => rst,
            Q => PC);
    
    Instr : NBitRegister
        port map (
            CLK => clk,
            D => MemOut,
            EN => IRWrite,
            RST => rst,
            Q => instruction);
            
    A : NBitRegister
        port map (
            CLK => clk,
            D => ReadData1,
            EN => RegA,
            RST => rst,
            Q => RegAOut);
            
    B : NBitRegister
        port map (
            CLK => clk,
            D => ReadData2,
            EN => RegB,
            RST => rst,
            Q => RegBOut);
            
    HiReg : NBitRegister
        port map (
            CLK => clk,
            D => Product(63 downto 32),
            EN => Hi,
            RST => rst,
            Q => HiOut);
            
    LoReg : NBitRegister
        port map (
            CLK => clk,
            D => Product(n-1 downto 0),
            EN => Lo,
            RST => rst,
            Q => LoOut);
            
    ALUO : NBitRegister
        port map (
            CLK => clk,
            D => ALUResult,
            EN => ALUOut,
            RST => rst,
            Q => ALUOutput);
            
    MemDat : NBitRegister
        port map (
            CLK => clk,
            D => MemDataIn,
            EN => MemEn,
            RST => rst,
            Q => MemData);
        
    PCEN <= PCWrite OR (PCWRiteCond AND NOT(zero));
    
    SHAMT <= RegAOut(4 downto 0) when Instruction(5 downto 0) = "000100" else
             Instruction(10 downto 6); 
             
    Immediate <= std_logic_vector(resize(signed(Instruction(15 downto 0)), Immediate'length));
    
    MemWriteData <= RegBOut;
    
    with RegDst select WriteReg <= 
        Instruction(20 downto 16) when "00",
        Instruction(15 downto 11) when "01",
        "11111" when others;
        
    with MemtoReg select WriteData <=
        ALUOutput when "000",
        MemData when "001",
        LeadingOnes when "010",
        HiOut when "011",
        LoOut when "100",
        PC when others;
        
    with ALUSrcA select ALUAIn <=
        PC when "00",
        RegAOut when "01",
        RegBOut when others;
        
    with ALUSrcB select ALUBIn <=
        RegBOut when "00",
        x"00000004" when "01",
        Immediate when "10",
        Immediate(n-3 downto 0) & "00" when others;
         
    with PCSource select PCIn <=
        ALUResult when "00",
        ALUOutput when "01",
        PC(n-1 downto 28) & Instruction(25 downto 0) & "00" when "10",
        ReadData1 when others;
        
   with IorD select MemIn <=
        PC when '0',
        ALUOutput when others;
        
   with MemSrc select MemDataIn <=
        std_logic_vector(resize(signed(MemOut(7 downto 0)), MemDataIn'length)) when "00",
        std_logic_vector(resize(signed(MemOut(15 downto 0)), MemDataIn'length)) when "01",
        Immediate(15 downto 0) & "0000000000000000" when "10",
        MemOut when others;
        
    process (RegAOut)
        variable onesCount, i : integer;
    begin
        i := n-1;
        onesCount := 0;
        while (i >= 0 and RegAOut(i) = '1') loop
            onesCount := onesCount + 1;
            i := i - 1;
        end loop;
        LeadingOnes <= std_logic_vector(to_unsigned(onesCount, LeadingOnes'length));
    end process;
    
end Behavioral;
