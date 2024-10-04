`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/09/14 12:18:46
// Design Name: 
// Module Name: spi_test
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


module spi_tx_test(
    input clk, reset,
    input [3:0] btn,
    output scl, sda,
    output reg cs, dc
    );
    
    reg [7:0] data_in;
    wire valid;
    spi_tx spi_inst(
        clk, reset, data_in, cs, scl, sda, valid
    );
    
    parameter IDLE          = 0,
              SLEEP_OUT     = 1,
              DISPLAY_ON    = 2,
              COLMOD1       = 3,
              COLMOD2       = 4,
              MEMORY_WRITE  = 5,
              SEND_BLANK1   = 6,
              SEND_BLANK2   = 7,
              SEND_RED1     = 8,
              SEND_RED2     = 9,
              SEND_GREEN1   = 10,
              SEND_GREEN2   = 11,
              SEND_BLUE1    = 12,
              SEND_BLUE2    = 13;
    
    reg [3:0] state, next_state;
    always @(negedge clk, posedge reset) begin
        if(reset)
            state = IDLE;
        else
            state = next_state;
    end
    
    reg init;
    always @(posedge clk, posedge reset) begin
        if(reset) begin
            next_state = IDLE;
            cs = 1;
            dc = 0;
            data_in = 0;
            init = 0;
        end
        else begin
            case(state)
                IDLE: begin
                    if(!init) begin
                        cs = 0;
                        next_state = SLEEP_OUT;
                    end
                    else begin
                        if(btn[0]) begin
                            cs = 0;
                            next_state = SEND_BLANK1;
                        end
                        else if(btn[1]) begin
                            cs = 0;
                            next_state = SEND_RED1;
                        end
                        else if(btn[2]) begin
                            cs = 0;
                            next_state = SEND_GREEN1;
                        end
                        else if(btn[3]) begin
                            cs = 0;
                            next_state = SEND_BLUE1;
                        end
                    end
                end
                SLEEP_OUT: begin
                    if(valid) begin
                        next_state = DISPLAY_ON;
                    end
                    else begin
                        data_in = 8'h11;
                        dc = 0;
                    end
                end
                DISPLAY_ON: begin
                    if(valid) begin
                        next_state = COLMOD1;
                    end
                    else begin
                        data_in = 8'h29;
                    end
                end
                COLMOD1: begin
                    if(valid) begin
                        next_state = COLMOD2;
                    end
                    else begin
                        data_in = 8'h3A;
                    end
                end
                COLMOD2: begin
                    if(valid) begin
                        next_state = MEMORY_WRITE;
                    end
                    else begin
                        data_in = 8'h55;
                        dc = 1;
                    end
                end
                MEMORY_WRITE: begin
                    if(valid) begin
                        cs = 1;
                        init = 1;
                        next_state = IDLE;
                    end
                    else begin
                        data_in = 8'h2C;
                        dc = 0;
                    end
                end
                SEND_BLANK1: begin
                    if(valid) begin
                        next_state = SEND_BLANK2;
                    end
                    else begin
                        data_in = 8'b0;
                        dc = 1;
                    end
                end
                SEND_BLANK2: begin
                    if(valid) begin
                        if(btn[1])
                            next_state = SEND_BLANK1;
                        else begin
                            cs = 1;
                            next_state = IDLE;
                        end 
                    end
                    else begin
                        data_in = 8'b0;
                    end
                end
                SEND_RED1: begin
                    if(valid) begin
                        next_state = SEND_RED2;
                    end
                    else begin
                        data_in = 8'b1111_1000;
                        dc = 1;
                    end
                end
                SEND_RED2: begin
                    if(valid) begin
                        if(btn[1])
                            next_state = SEND_RED1;
                        else begin
                            cs = 1;
                            next_state = IDLE;
                        end 
                    end
                    else begin
                        data_in = 8'b0;
                    end
                end
                SEND_GREEN1: begin
                    if(valid) begin
                        next_state = SEND_GREEN2;
                    end
                    else begin
                        data_in = 8'b0000_0111;
                        dc = 1;
                    end
                end
                SEND_GREEN2: begin
                    if(valid) begin
                        if(btn[2])
                            next_state = SEND_GREEN1;
                        else begin
                            cs = 1;
                            next_state = IDLE;
                        end 
                    end
                    else begin
                        data_in = 8'b1110_000;
                    end
                end
                SEND_BLUE1: begin
                    if(valid) begin
                        next_state = SEND_BLUE2;
                    end
                    else begin
                        data_in = 8'b0;
                        dc = 1;
                    end
                end
                SEND_BLUE2: begin
                    if(valid) begin
                        if(btn[3])
                            next_state = SEND_BLUE1;
                        else begin
                            cs = 1;
                            next_state = IDLE;
                        end 
                    end
                    else begin
                        data_in = 8'b0001_1111;
                    end
                end
            endcase
        end
    end
endmodule