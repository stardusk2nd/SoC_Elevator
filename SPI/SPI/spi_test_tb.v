`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/09/14 14:00:24
// Design Name: 
// Module Name: spi_test_tb
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


module spi_test_tb();
    
    reg clk, reset;
    reg [3:0] btn = 0;
    wire cs, scl, sda, dc;
    spi_test uut(clk, reset, btn, cs, scl, sda, dc);
    
    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end
    
    integer i;
    initial begin
        reset = 1;
        #10;
        reset = 0;
        
        for(i = 0; i < 4; i = i + 1) begin
            btn[i] = 1;
            #10;
            btn[i] = 0;
            #10000;
        end
        
        $finish;
    end
    
endmodule
