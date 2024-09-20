`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/09/13 23:45:28
// Design Name: 
// Module Name: spi
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


`include "defines.vh"

module spi(
    input clk, reset,
    input onoff,
    input [7:0] data_in,
    output cs,
    output reg scl, sda,
    output reg valid
    );
    
    parameter SCL_FREQ = 15_000_000; // 15MHz
    parameter integer SCL_PSC = `CLK_FREQ / SCL_FREQ;   // Prescaler for clock divider
    parameter integer WAIT_FOR_VALID = SCL_PSC * 0.9;
    
    /* Generate 'SCL_FREQ'Hz SCL */
    reg [$clog2(SCL_PSC)-1 : 0] count;
    reg sda_sampling;
    always @(posedge clk, posedge reset) begin
        if(reset) begin
            scl = 0;
            count = 0;
            sda_sampling = 0;
        end
        else begin
            if(!cs) begin
                if(count < SCL_PSC - 1) begin
                    count = count + 1;
                    if(count < SCL_PSC / 2)
                        scl = 0;
                    else
                        scl = 1;
                    if(count == SCL_PSC / 4)
                        sda_sampling = 1;
                    else
                        sda_sampling = 0;
                end
                else begin
                    count = 0;
                    scl = 0;
                end
            end
        end
    end
    
    assign cs = ~onoff;
    
    reg [2:0] index;
    reg waiting_p;
    always @(posedge clk, posedge reset) begin
        if(reset) begin
            sda = 0;
            index = 7;
            waiting_p = 0;
        end
        else if(!cs && sda_sampling) begin
            sda = data_in[index];
            if(index > 0) begin
                index = index - 1;
            end
            else begin
                index = 7;
                waiting_p = 1;
            end
        end
        else
            waiting_p = 0;
    end
    
    reg [$clog2(WAIT_FOR_VALID)-1 : 0] wait_cnt;
    reg waiting;
    always @(posedge clk, posedge reset) begin
        if(reset) begin
            valid = 0;
            wait_cnt = 0;
            waiting = 0;
        end
        else if(valid)
            valid = 0;
        else begin
            if(waiting_p)
                waiting = 1;
            if(waiting) begin
                if(wait_cnt < WAIT_FOR_VALID - 1)
                    wait_cnt = wait_cnt + 1;
                else begin
                    wait_cnt = 0;
                    waiting = 0;
                    valid = 1;
                end
            end
        end
    end
    
endmodule