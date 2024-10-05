`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/09/24 16:39:29
// Design Name: 
// Module Name: timer_counter
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


module timer_counter(
    input clk, reset,
    input [1:0] control,
<<<<<<< Updated upstream
    input [15:0] max_count,
    input [15:0] compare,
    output reg signal
=======
    input [31:0] prescalor,
    input [31:0] max_count,
    input [31:0] compare,
    output reg timer_int, pwm
>>>>>>> Stashed changes
    );
    
    wire [1:0] mode = control[1:0];
    reg [1:0] prev_mode;
    
    reg [15:0] count;
    always @(posedge clk, posedge reset) begin
        if(reset) begin
            count = 0;
<<<<<<< Updated upstream
            signal = 0;
=======
            timer_int = 0;
            pwm = 0;
>>>>>>> Stashed changes
            prev_mode = 0;
        end
        else begin
            if(prev_mode != mode) begin
                count = 0;
<<<<<<< Updated upstream
                signal = 0;
=======
                timer_int = 0;
                pwm = 0;
>>>>>>> Stashed changes
                prev_mode = mode;
            end
            // timer_int
            if(mode == 2'b01) begin
<<<<<<< Updated upstream
                if(count < max_count) begin
                    count = count + 1;
                    signal = 0;
                end
                else begin
                    count = 0;
                    signal = 1;
                end
            end
=======
                if(cnt_pres < prescalor)
                    cnt_pres = cnt_pres + 1;     
                else begin
                    cnt_pres = 0;
                    if(count < max_count) begin
                        count = count + 1;
                        timer_int = 0;
                    end
                    else begin
                        count = 0;
                        timer_int = 1;
                    end
                end
            end
            // pwm          
>>>>>>> Stashed changes
            else if(mode == 2'b10) begin
                if(count < max_count)
                    count = count + 1;
                else
                    count = 0;
                if(count < compare)
                    signal = 1;
                else
                    signal = 0;
            end
        end
    end
    
endmodule