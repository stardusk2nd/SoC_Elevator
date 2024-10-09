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

module spi_tx(
    input clk, reset,
    input [7:0] data_in,// 1-byte data to send
    input cs,           // on/off control signal (active-low)
    output reg scl,     // spi clock pin
    output reg sda,     // spi data pin (tx, master to slave)
    output reg valid    // end of communication signal
    );
    
    parameter SCL_FREQ = 15_000_000;                    // frequency of scl: 15MHz
    parameter integer SCL_PSC = `CLK_FREQ / SCL_FREQ;   // prescaler for clock divider, generating 'scl'
    
    /* Generate 'SCL_FREQ'Hz SCL
       cpol(polarity) = 0, cpha(phase) = 0 */
    // width of count, for clock divider
    reg [$clog2(SCL_PSC)-1 : 0] count;
    // timing for loading the data_in onto the sda, one bit at a time
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
    
    /* send 1-byte */
    reg [2:0] index;
    reg waiting;
    always @(posedge clk, posedge reset) begin
        if(reset) begin
            sda = 0;
            // sending sequence from HSB -> LSB
            index = 7;
            waiting = 0;
            valid = 0;
        end
        else if(valid)
            valid = 0;
        else begin
            // load 1-bit at 'sda_sampling'
            if(!cs && sda_sampling) begin
                sda = data_in[index];
                if(index > 0)
                    index = index - 1;
                else begin
                    index = 7;
                    waiting = 1;
                end
            end
            // wait until last bit sended
            if(waiting && count == 3 * SCL_PSC / 4) begin
                waiting = 0;
                valid = 1;
            end
        end
    end
    
endmodule
