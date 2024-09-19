`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/09/09 11:49:15
// Design Name: 
// Module Name: btn_debouncer
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


module btn_debouncer(
    input clk, reset,
    input btn,
    output btn_nedge
    );
    
    wire div_edge;
    count_divider #(18) counter_divider_inst(
        clk, reset, div_edge
    );
    
    reg btn_sampled;
    edge_detector edge_detector_inst(
        .clk(clk), .reset(reset), .cp(btn_sampled), .pedge(), .nedge(btn_nedge) 
    );
    
    always @(posedge clk, posedge reset) begin
        if(reset)
            btn_sampled = 0;
        else if(div_edge) begin
            btn_sampled = btn;
        end
    end
    
endmodule
