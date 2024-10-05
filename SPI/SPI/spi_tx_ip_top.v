`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/10/03 11:54:28
// Design Name: 
// Module Name: spi_tx_ip_top
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module spi_tx_ip_top(
    input clk, reset,
    input [9:0] control,
    input [7:0] data_in,
    output cs,
    output dc,
    output scl, sda,
    output valid
    );
    
    /* control register */
    wire enable          = control[0];      // communication on-off
    wire data_mode       = control[1];      // data or command selection. 0: com, 1: data
    wire [7:0] prescalor = control[9:2];    // prescalor for scl
    assign dc = data_mode;
    assign cs = enable;
    
    /* spi module instane */
    spi_tx_ip spi_tx_ip_inst(clk, reset, data_in, prescalor, cs, scl, sda, valid);
    
endmodule
