library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.numeric_std.all;

entity Multiplier is
    generic (
		n : positive := 64
	);
    Port ( A : in STD_LOGIC_VECTOR (31 downto 0);
           B : in STD_LOGIC_VECTOR (31 downto 0);
           clk : in STD_LOGIC;
           rst : in STD_LOGIC;
           R : out STD_LOGIC_VECTOR (63 downto 0);
           done : out STD_LOGIC);
end Multiplier;

architecture Behavioral of Multiplier is

    -- Build an enumerated type for the state machine
	type state_type is (Start, Test, Shift, Add, Fin);
	
	-- Register to hold the current state
	signal pr_state, nx_state   : state_type;
	
	signal Load, RegEN, ShiftEN, Cout : std_logic;
	signal Multiplicand, RegIN, Product, Sum : std_logic_vector(n-1 downto 0); 
	signal Multiplier : std_logic_vector(31 downto 0);
	
	signal countTest : integer;
	
	component NBitAddSub is
        port (
            K     : in  std_logic; -- 0 for +, 1 for -
            A     : in  std_logic_vector(n-1 downto 0);
            B     : in  std_logic_vector(n-1 downto 0);
            S     : out std_logic_vector(n-1 downto 0);
            Cout  : out std_logic
        );
    end component;

    component NBitReg is
        port (
            CLK   : in  std_logic;
            D     : in  std_logic_vector(n-1 downto 0);
            EN    : in  std_logic;
            RST   : in std_logic;
            Q     : out std_logic_vector(n-1 downto 0)
        );
    end component;

    component NBitShiftLeft is
        port (
            CLK   : in  std_logic;
            D     : in  std_logic_vector(n-1 downto 0);
            EN    : in  std_logic;
            RST   : in  std_logic;
            Load  : in  std_logic;
            Q     : inout std_logic_vector(n-1 downto 0);
            S     : in  std_logic
        );
    end component;
    
    component NBitShiftRight is
        port (
            CLK   : in  std_logic;
            D     : in  std_logic_vector(31 downto 0);
            EN    : in  std_logic;
            RST   : in  std_logic;
            Load  : in  std_logic;
            Q     : inout std_logic_vector(31 downto 0);
            S     : in  std_logic
        );
    end component;

begin
                
    U1 : NBitReg
        port map (
            CLK => clk,
            D => RegIN,
            EN => RegEN,
            RST => rst,
            Q => Product);
            
    U2 : NBitShiftLeft
        port map (
            CLK => clk,
            D(n-1 downto 32) => x"00000000",
            D(31 downto 0) => A,
            EN => ShiftEN,
            RST => rst,
            Load => Load,
            Q => Multiplicand,
            S => '0');
            
    U3 : NBitShiftRight
        port map (
            CLK => clk,
            D => B,
            EN => ShiftEN,
            RST => rst,
            Load => Load,
            Q => Multiplier,
            S => '0');
            
    U4 : NBitAddSub
        port map (
            K => '0',
            A => Product,
            B => Multiplicand,
            S => Sum,
            Cout => Cout);
            

	-- Logic to advance to the next state
	process (clk, rst)
	   variable test : std_logic; 
	begin
		if rst = '1' then
			pr_state <= Start;
		elsif (clk'event AND clk = '1') then
		    pr_state <= nx_state;
		end if;
	end process;
	
	
	process(pr_state, Multiplier(0))
	   variable count : integer := 0;
	begin 	
        case pr_state is
            when Start =>
                R <= (others => '0');       --reset/initialize R
                Load <= '1';                --Load A,B,
                ShiftEN <= '1';
                RegIN <= (others => '0');   --initialize product (not R);
                RegEN <= '1';
                count := 0;                 --reset count
                done <= '0';                --reset done
                nx_state <= Test;
                
            when Test =>
                Load <= '0';                --A, B, R loaded at rising edge after Start, reset flags
                RegEN <= '0';
                ShiftEN <= '0';             --reset after load coming from Start or after shift coming from Shift
                if Multiplier(0) = '1' then --Test B(0) from shift register
                    nx_state <= Add;
                else
                    nx_state <= Shift;
                end if;
                
            when Add =>
                RegIN <= Sum;
                RegEN <= '1';               --R += A happens continuously, only load sum at rising edge after this state
                nx_state <= Shift;
                
            when Shift =>
                RegEN <= '0';
                shiftEN <= '1';             --both shifts happened at rising edge,
                count := count + 1;
                countTest <= count;
                if count<32 then 
                    nx_state <= Test;
                else 
                    nx_state <= Fin;
                end if;
                    
            when Fin =>
                R <= product;
                done <= '1'; 
        end case;
	end process;
end Behavioral;
