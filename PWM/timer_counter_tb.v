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
    reg [5:0] control;
    reg [31:0] prescalor, compare, max_count;
    wire timer_out0, timer_out1, timer_out2, timer_out3;
    
    timer_counter dut(clk, reset, control, prescalor, max_count, compare,
                      timer_out0, timer_out1, timer_out2, timer_out3);
    
    initial begin
        clk = 0;
        reset = 1;
        max_count = 999;
        compare = 999;
        prescalor = 999;
        forever #5 clk = ~clk;
    end
    
    initial begin
        #10;
        reset = 0;
        
        control = 6'b000101;
        #10000000;
        
        control = 6'b001001;
        #10000000;
        
        control = 6'b010001;
        #10000000;
        
        control = 6'b100001;
        #10000000;
        $finish;
    end

endmodule
