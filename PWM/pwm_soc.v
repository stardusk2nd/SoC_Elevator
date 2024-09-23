`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/09/11 11:48:42
// Design Name: 
// Module Name: pwm_soc
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

module pwm_soc(
    input clk, reset,
    input [5:0] control,
    input [15:0] duty_cycle,
    output pwm
    );
    
    wire pwm_on = control[0];
    
    wire [4:0] duty_bit = control[2:1] == 2'b00 ? 8 :
                          control[2:1] == 2'b01 ? 10 : 
                          control[2:1] == 2'b10 ? 12 :
                          control[2:1] == 2'b11 ? 16 : 0;
    
    wire [12:0] prescaler = control[5:3] == 3'b000 ? 1    - 1 :
                            control[5:3] == 3'b001 ? 8    - 1 :
                            control[5:3] == 3'b010 ? 64   - 1 :
                            control[5:3] == 3'b011 ? 256  - 1 :
                            control[5:3] == 3'b100 ? 1024 - 1 :
                            control[5:3] == 3'b101 ? 2048 - 1 :
                            control[5:3] == 3'b110 ? 4096 - 1 :
                            control[5:3] == 3'b111 ? 8192 - 1 : 0;
    
    wire [15:0] duty_resol = (1 << duty_bit) - 1;
    
    reg [12:0] count;
    reg [15:0] cnt_pwm;
    always @(posedge clk, posedge reset) begin
        if(reset) begin
            count = 0;
            cnt_pwm = 0;
        end
        else if(pwm_on) begin
            if(count < prescaler)
                count = count + 1;
            else begin
                count = 0;
                if(cnt_pwm < duty_resol)
                    cnt_pwm = cnt_pwm + 1;
                else
                    cnt_pwm = 0;
            end
        end
    end
    
    assign pwm = ((cnt_pwm < duty_cycle) && pwm_on)? 1 : 0;
    
endmodule