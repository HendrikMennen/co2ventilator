--============================================================================= 
--
-- bidir buffer - I2C
--
--============================================================================= 

library ieee;
    use ieee.std_logic_1164.all;
    use ieee.std_logic_unsigned.all;

entity bidir_buf is
generic( 
    freq_hz     : integer := 50_000_000 -- source clk = 50Mhz, needed clk = 100khz
);
port(
    SCL         : inout std_logic;
    SDA         : inout std_logic;
    err_cnt     : out std_logic_vector(31 downto 0);    
    sda_in      : out std_logic;
    scl_in      : out std_logic;
    resetn      : out std_logic;
    mode        : in std_logic_vector(2 downto 0);
    scl_oe      : in std_logic; 
    sda_oe      : in std_logic;
    clk         : in std_logic
);
end entity;

architecture behaviour of bidir_buf is
    constant cnt_max    : integer := freq_hz/100000;
    signal cnt_div      : integer range 0 to cnt_max;
    signal clk_div      : std_logic;
    signal cnt_err      : std_logic_vector(31 downto 0);
    signal resetn_i     : std_logic;
begin
    -- bidir buffer - I2C
    SCL         <= '0' when scl_oe = '1' and mode < 7 else
                    clk_div when SDA = '0' and mode = 7 else
                    'Z';
    scl_in      <= SCL;
    
    SDA         <= '0' when sda_oe = '1' else
                    'Z';
    sda_in      <= SDA;
    
    -- reset control
    resetn_i    <= '0' when SDA = '1' and mode = 7 else '1';
    resetn      <= resetn_i;
    
    --error counter
    process(resetn_i,clk)
    begin
        if falling_edge(resetn_i) then
            if mode = 7 then  
                cnt_err <= cnt_err + 1;
            end if;
        end if;
    end process;
    err_cnt     <= cnt_err;
    
    -- clk divider
    process(clk)
    begin
        if rising_edge(clk) then
            if cnt_div = (cnt_max - 1) then  
                cnt_div <= 0;
            else
                cnt_div <= cnt_div + 1;                 
            end if;
        end if;
    end process;
    clk_div <= '0' when cnt_div < (cnt_max / 2) else '1'; 

end architecture;
