`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/10/03 11:54:28
// Design Name: 
// Module Name: spi_tx_ip_top
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

module spi_tx_ip_top(
    input clk, reset,
    input [9:0] control,
    input [7:0] data_in,
    output reg cs,
    output dc,
    output scl, sda,
    output valid
    );
    
    /* control register */
    wire enable          = control[0];      // communication on-off
    wire data_mode       = control[1];      // data or command selection. 0: com, 1: data
    wire [7:0] prescalor = control[9:2];    // prescalor for scl
    assign dc = data_mode;
    
    /* spi module instane */
    spi_tx_ip spi_tx_ip_inst(clk, reset, data_in, prescalor, cs, scl, sda, valid);
    
    /* state machine control */
    parameter IDLE      = 0;
    parameter SEND_CMD  = 1;
    parameter SEND_DATA = 2;
    
    reg [1:0] state, nxt_state;
    always @(negedge clk, posedge reset) begin
        if(reset)
            state = IDLE;
        else
            state = nxt_state;    
    end
    
    /* state machine */
    always @(posedge clk, posedge reset) begin
        if(reset) begin
            nxt_state = IDLE;
            cs = 1;
        end
        else begin
            case(nxt_state)
                IDLE: begin
                    // communication start
                    if(enable) begin
                        if(!data_mode)
                            nxt_state = SEND_CMD;
                        else
                            nxt_state = SEND_DATA;
                    end
                end
                // Vitis: single transmission
                SEND_CMD: begin
                    if(valid) begin
                        cs = 1;
                        nxt_state = IDLE;
                    end
                    else
                        cs = 0;
                end
                // Vitis: multiple transmission fuction needed
                SEND_DATA: begin
                    if(valid) begin
                        if(enable)
                            nxt_state = SEND_DATA;
                        else begin
                            cs = 1;
                            nxt_state = IDLE;
                        end
                    end
                    else
                        cs = 0;
                end
            endcase
        end
    end
    
endmodule
