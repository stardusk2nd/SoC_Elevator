`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/09/07 14:54:13
// Design Name: 
// Module Name: ClockDivider
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


module clock_divider #(parameter N = 1000)(
    input clk, reset,
    input enable,
    output reg div_edge
    );
    
    reg [$clog2(N)-1:0] count;
    always @(posedge clk or posedge reset) begin
        if(reset) begin
            count = 0;
            div_edge = 0;
        end
        else if(enable)begin
            if(count < N - 1) begin
                count = count + 1;
                div_edge = 0;
            end
            else begin
                count = 0;
                div_edge = 1;
            end
        end
        else begin
            count = 0;
            div_edge = 0;
        end
    end

endmodule
