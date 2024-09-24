`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/09/24 16:39:29
// Design Name: 
// Module Name: timer_counter
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


module timer_counter(
    input clk, reset,
    input [1:0] control,
    input [15:0] max_count,
    input [15:0] compare,
    output reg signal
    );
    
    wire [1:0] mode = control[1:0];
    reg [1:0] prev_mode;
    
    reg [15:0] count;
    always @(posedge clk, posedge reset) begin
        if(reset) begin
            count = 0;
            signal = 0;
            prev_mode = 0;
        end
        else begin
            if(prev_mode != mode) begin
                count = 0;
                signal = 0;
                prev_mode = mode;
            end
            // int
            if(mode == 2'b01) begin
                if(count < max_count) begin
                    count = count + 1;
                    signal = 0;
                end
                else begin
                    count = 0;
                    signal = 1;
                end
            end
            else if(mode == 2'b10) begin
                if(count < max_count)
                    count = count + 1;
                else
                    count = 0;
                if(count < compare)
                    signal = 1;
                else
                    signal = 0;
            end
        end
    end
    
endmodule