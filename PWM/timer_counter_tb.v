`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/09/24 17:41:31
// Design Name: 
// Module Name: timer_counter_tb
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


module timer_counter_tb();

    reg clk, reset;
    reg [1:0] control;
    reg [15:0] compare, max_count;
    wire signal;
    
    timer_counter uut(clk, reset, control, max_count, compare, signal);
    
    initial begin
        clk = 0;
        reset = 1;
        max_count = 1000;
        compare = 250;
        forever #5 clk = ~clk;
    end
    
    initial begin
        #10;
        reset = 0;
        
        control = 2'b00;
        #1000;
        
        control = 2'b01;
        #97500;
        
        control = 2'b10;
        #100000;
        
        control = 2'b11;
        #10000;
        $finish;
    end

endmodule
