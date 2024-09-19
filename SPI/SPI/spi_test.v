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


module spi_test(
    input clk, reset,
    input [3:0] btn,
    output cs, scl, sda,
    output reg dc
    );
    
    reg onoff;
    reg [7:0] data_in;
    wire valid;
    spi spi_inst(
        clk, reset, onoff, data_in, cs, scl, sda, valid
    );
    
    parameter IDLE          = 0,
              SLEEP_OUT     = 1,
              DISPLAY_ON    = 2,
              COLMOD1       = 3,
              COLMOD2       = 4,
              MEMORY_WRITE  = 5,
              SEND_RED1     = 6,
              SEND_RED2     = 7,
              SEND_GREEN1   = 8,
              SEND_GREEN2   = 9,
              SEND_BLUE1    = 10,
              SEND_BLUE2    = 11;
    
    reg [3:0] state, next_state;
    always @(negedge clk, posedge reset) begin
        if(reset)
            state = IDLE;
        else
            state = next_state;
    end
    
    wire [3:0] btn_nedge;
    btn_debouncer btn_debouncer_inst[3:0](clk, reset, btn, btn_nedge);
    
    reg init;
    always @(posedge clk, posedge reset) begin
        if(reset) begin
            next_state = IDLE;
            onoff = 0;
            dc = 0;
            data_in = 0;
            init = 0;
        end
        else begin
            case(state)
                IDLE: begin
                    if(!init) begin
                        onoff = 1;
                        next_state = SLEEP_OUT;
                    end
                    else begin
                        if(btn_nedge[0]) begin
                            onoff = 1;
                            next_state = MEMORY_WRITE;
                        end
                        else if(btn[1]) begin
                            onoff = 1;
                            next_state = SEND_RED1;
                        end
                        else if(btn[2]) begin
                            onoff = 1;
                            next_state = SEND_GREEN1;
                        end
                        else if(btn[3]) begin
                            onoff = 1;
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
                        onoff = 0;
                        init = 1;
                        next_state = IDLE;
                    end
                    else begin
                        data_in = 8'h55;
                        dc = 1;
                    end
                end
                MEMORY_WRITE: begin
                    if(valid) begin
                        onoff = 0;
                        next_state = IDLE;
                    end
                    else begin
                        data_in = 8'h2C;
                        dc = 0;
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
                        onoff = 0;
                        next_state = IDLE;
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
                        onoff = 0;
                        next_state = IDLE;
                    end
                    else begin
                        data_in = 8'b1110_0000;
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
                        onoff = 0;
                        next_state = IDLE;
                    end
                    else begin
                        data_in = 8'b0001_1111;
                    end
                end
            endcase
        end
    end
endmodule