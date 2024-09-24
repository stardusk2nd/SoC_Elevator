`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/09/07 14:42:30
// Design Name: 
// Module Name: EdgeDetector
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


module edge_detector(
    input clk, reset,
    input cp,
    output pedge, nedge
    );
    
    reg master, slave;
    always @(posedge clk, posedge reset) begin
        if(reset) begin
            master = 0;
            slave = 0;
        end
        else begin
            slave = master;
            master = cp;
        end
    end
    
    assign pedge = ({master,slave} == 2'b10)? 1:0;
    assign nedge = ({master,slave} == 2'b01)? 1:0;
    
endmodule
