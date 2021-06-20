
module NIOSDuino_Core (
	clk_in_clk,
	i2c_sda_in,
	i2c_scl_in,
	i2c_sda_oe,
	i2c_scl_oe,
	pi_export,
	pio_export,
	pwm_out,
	reset_reset_n,
	sdram_addr,
	sdram_ba,
	sdram_cas_n,
	sdram_cke,
	sdram_cs_n,
	sdram_dq,
	sdram_dqm,
	sdram_ras_n,
	sdram_we_n,
	spi_MISO,
	spi_MOSI,
	spi_SCLK,
	spi_SS_n,
	spi_flash_dclk,
	spi_flash_ncs,
	spi_flash_data,
	uart_rxd,
	uart_txd);	

	input		clk_in_clk;
	input		i2c_sda_in;
	input		i2c_scl_in;
	output		i2c_sda_oe;
	output		i2c_scl_oe;
	input	[31:0]	pi_export;
	inout	[31:0]	pio_export;
	output	[31:0]	pwm_out;
	input		reset_reset_n;
	output	[11:0]	sdram_addr;
	output	[1:0]	sdram_ba;
	output		sdram_cas_n;
	output		sdram_cke;
	output		sdram_cs_n;
	inout	[15:0]	sdram_dq;
	output	[1:0]	sdram_dqm;
	output		sdram_ras_n;
	output		sdram_we_n;
	input		spi_MISO;
	output		spi_MOSI;
	output		spi_SCLK;
	output		spi_SS_n;
	output		spi_flash_dclk;
	output		spi_flash_ncs;
	inout	[3:0]	spi_flash_data;
	input		uart_rxd;
	output		uart_txd;
endmodule
