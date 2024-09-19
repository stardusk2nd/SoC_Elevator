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
    output reg dc,
    output reg lcd_reset
    );
    
    reg onoff;
    reg [7:0] data_in;
    wire valid;
    spi spi_inst(
        clk, reset, onoff, data_in, cs, scl, sda, valid
    );
    
    parameter IDLE = 0;
    parameter INIT = 1;
    parameter RESET = 2;
    parameter SLEEP_OUT = 3;
    parameter DISP_OFF = 4;
    parameter DISP_ON = 5;
    
    reg [2:0] state, next_state;
    always @(negedge clk, posedge reset) begin
        if(reset)
            state = IDLE;
        else
            state = next_state;
    end
    
    wire [3:0] btn_nedge;
    btn_debouncer btn_debouncer_inst[3:0](clk, reset, btn, btn_nedge);
    
    reg init;
    reg [22:0] init_cnt;
    always @(posedge clk, posedge reset) begin
        if(reset) begin
            next_state = IDLE;
            onoff = 0;
            dc = 0;
            data_in = 0;
            init = 0;
            init_cnt = 0;
            lcd_reset = 1;
        end
        else begin
            case(state)
                IDLE: begin
                    if(!init) begin
                        next_state = INIT;
                    end
                    else if(btn_nedge[0]) begin
                        onoff = 1;
                        next_state = RESET;
                    end
                    else if(btn_nedge[1]) begin
                        onoff = 1;
                        next_state = SLEEP_OUT;
                    end
                    else if(btn_nedge[2]) begin
                        onoff = 1;
                        next_state = DISP_OFF;
                    end
                    else if(btn_nedge[3]) begin
                        onoff = 1;
                        next_state = DISP_ON;
                    end
                end
                INIT: begin
                    if(init_cnt < 50_000_00 - 1) begin
                        init_cnt = init_cnt + 1;
                        lcd_reset = 0;
                    end
                    else begin
                        lcd_reset = 1;
                        init = 1;
                        next_state = IDLE;
                    end
                end
                RESET: begin
                    if(valid) begin
                        next_state = IDLE;
                        onoff = 0;
                    end
                    else begin
                        data_in = 8'h01;
                        dc = 0;
                    end
                end
                SLEEP_OUT: begin
                    if(valid) begin
                        next_state = IDLE;
                        onoff = 0;
                    end
                    else begin
                        data_in = 8'h11;
                        dc = 0;
                    end
                end
                DISP_OFF: begin
                    if(valid) begin
                        next_state = IDLE;
                        onoff = 0;
                    end
                    else begin
                        data_in = 8'h28;
                        dc = 0;
                    end
                end
                DISP_ON: begin
                    if(valid) begin
                        next_state = IDLE;
                        onoff = 0;
                    end
                    else begin
                        data_in = 8'h29;
                        dc = 0;
                    end
                end
            endcase
        end
    end
endmodule
