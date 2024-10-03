`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/09/15 16:19:13
// Design Name: 
// Module Name: spi_lcd
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


module spi_tx_ip(
    input clk, reset,
    input [15:0] data_in,
    output cs, scl, sda,
    output reg dc
    );
    
    wire [7:0] data_buffer;
    wire onoff, valid;
    spi_tx spi_inst(
        .clk(clk), .reset(reset),
        .onoff(onoff),
        .data_in(data_buffer),
        .cs(cs), .scl(scl), .sda(sda),
        .valid(valid)
    );
    
    always @(posedge clk, posedge reset) begin
        if(reset) begin
            dc = 0;
        end
        else begin
        end
    end
    
endmodule
