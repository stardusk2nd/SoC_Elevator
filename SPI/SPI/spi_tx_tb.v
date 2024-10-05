`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/09/14 00:45:34
// Design Name: 
// Module Name: scl_tb
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


module spi_tx_tb();

    reg clk, reset, cs;
    reg [7:0] data_in = 8'b11100010;
    wire scl, sda, valid;
    spi_tx uut(clk, reset, data_in, cs, scl, sda, valid);

    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end
    
    initial begin
        reset = 1;
        #10;
        reset = 0;
        
        cs = 0;
        #5000;
        cs = 1;
        #1000;
        $finish;
    end

endmodule
