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
    reg [9:0] control = 0;
    reg [7:0] data_in = 0;
    wire cs, dc, scl, sda;
    wire valid;
    spi_tx_ip_top uut(clk, reset, control, data_in, cs, dc, scl, sda, valid);

    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end

    initial begin
        #10;
        reset = 0;
        
        control = 10'b0001_1000_0_0;
        
        data_in = 8'h11;
        control[0] = 1;
        @(posedge valid);
        #10;
        control[0] = 0;
        #10;
        data_in = 8'h29;
        control[0] = 1;
        @(posedge valid);
        #10;
        control[0] = 0;
        #10
        
        $finish;
    end

endmodule
