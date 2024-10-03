`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/10/03 16:55:16
// Design Name: 
// Module Name: spi_tx_ip_top_tb
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


module spi_tx_ip_top_tb();

    reg clk;
    reg reset = 1;
    reg [9:0] control;
    reg [7:0] data_in;
    wire cs, dc, scl, sda;
    spi_tx_ip_top uut(clk, reset, control, data_in, cs, dc, scl, sda);

    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end

    initial begin
        control = 10'b0000_1001_0_0;
        data_in = 8'b1001_1010;
        #10;
        reset = 0;
        
        control[0] = 1;
        #5000;
        control[0] = 0;
        #1000;
        
        control[1] = 1;
        control[0] = 1;
        #5000;
        control[0] = 0;
        #1000
        
        $finish;
    end

endmodule
