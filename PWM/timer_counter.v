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


<<<<<<< HEAD
//module timer_counter_x4(
//    input wire [1:0] control0,
//    input wire [31:0] prescalor0, max_count0, compare0,
//    output timer_out0
//);
    
    
//    timer_counter timer_counter_inst0(
//        clk, reset, control, prescalor, max_count, compare, timer_out
//    );
//    wire [1:0] control1;
//    wire [31:0] prescalor1, max_count1, compare1;
//    wire timer_out1;
//    timer_counter timer_counter_inst1(
//        clk, reset, control, prescalor, max_count, compare, timer_out
//    );
//    timer_counter timer_counter_inst2(
//        clk, reset, control, prescalor, max_count, compare, timer_out
//    );
//    timer_counter timer_counter_inst3(
//        clk, reset, control, prescalor, max_count, compare, timer_out
//    );

//endmodule

//module timer_counter(
//    input clk, reset,
//    input [5:0] control,
//    input [31:0] prescalor,
//    input [31:0] max_count,
//    input [31:0] compare,
//    output reg timer_out0,
//    output reg timer_out1,
//    output reg timer_out2,
//    output reg timer_out3
//    );
    
//    wire [1:0] mode = control[1:0];
//    wire [3:0] channel = control[5:2];
//    reg [1:0] prev_mode;
    
//    reg [31:0] cnt_pres, count;
//    always @(posedge clk, posedge reset) begin
//        if(reset) begin
//            cnt_pres = 0;
//            count = 0;
//            timer_out0 = 0;
//            timer_out1 = 0;
//            timer_out2 = 0;
//            timer_out3 = 0;
//            prev_mode = 0;
//        end
//        else begin
//            if(prev_mode != mode) begin
//                cnt_pres = 0;
//                count = 0;
//                timer_out0 = 0;
//                timer_out1 = 0;
//                timer_out2 = 0;
//                timer_out3 = 0;
//                prev_mode = mode;
//            end
//            // int
//            if(mode == 2'b01) begin
//                if(channel[0])begin
//                    if(cnt_pres < prescalor)
//                        cnt_pres = cnt_pres + 1;
//                    else begin
//                        cnt_pres = 0;
//                        if(count < max_count) begin
//                            count = count + 1;
//                            timer_out0 = 0;
//                        end
//                        else begin
//                            count = 0;
//                            timer_out0 = 1;
//                        end
//                    end
//                end
//                else if(channel[1])begin
//                    if(cnt_pres < prescalor)
//                        cnt_pres = cnt_pres + 1;
//                    else begin
//                        cnt_pres = 0;
//                        if(count < max_count) begin
//                            count = count + 1;
//                            timer_out1 = 0;
//                        end
//                        else begin
//                            count = 0;
//                            timer_out1 = 1;
//                        end
//                    end
//                end
//                else if(channel[2])begin
//                    if(cnt_pres < prescalor)
//                        cnt_pres = cnt_pres + 1;
//                    else begin
//                        cnt_pres = 0;
//                        if(count < max_count) begin
//                            count = count + 1;
//                            timer_out2 = 0;
//                        end
//                        else begin
//                            count = 0;
//                            timer_out2 = 1;
//                        end
//                    end
//                end
//                else if(channel[3])begin
//                    if(cnt_pres < prescalor)
//                        cnt_pres = cnt_pres + 1;
//                    else begin
//                        cnt_pres = 0;
//                        if(count < max_count) begin
//                            count = count + 1;
//                            timer_out3 = 0;
//                        end
//                        else begin
//                            count = 0;
//                            timer_out3 = 1;
//                        end
//                    end
//                end
//                else begin
//                    timer_out0 = 0;
//                    timer_out1 = 0;
//                    timer_out2 = 0;
//                    timer_out3 = 0;
//                end
//            end
//            else if(mode == 2'b10) begin
//                if(cnt_pres < prescalor)
//                    cnt_pres = cnt_pres + 1;
//                else begin
//                    cnt_pres = 0;
//                    if(count < max_count)
//                        count = count + 1;
//                    else
//                        count = 0;
//                    if(channel[0])begin
//                        if(count < compare)
//                            timer_out0 = 1;
//                        else
//                            timer_out0 = 0;
//                    end
//                    else if(channel[1])begin
//                        if(count < compare)
//                            timer_out1 = 1;
//                        else
//                            timer_out1 = 0;
//                    end
//                    else if(channel[2])begin
//                        if(count < compare)
//                            timer_out2 = 1;
//                        else
//                            timer_out2 = 0;
//                    end
//                    else if(channel[3])begin
//                        if(count < compare)
//                            timer_out3 = 1;
//                        else
//                            timer_out3 = 0;
//                    end
//                    else begin
//                        timer_out0 = 0;
//                        timer_out1 = 0;
//                        timer_out2 = 0;
//                        timer_out3 = 0;
//                    end
//                end
//            end
//        end
//    end
    
//endmodule

=======
>>>>>>> e8dcf67e362838e082719eecc5e03d93dba935f3
module timer_counter(
    input clk, reset,
    input [1:0] control,
    input [31:0] prescalor,
    input [31:0] max_count,
    input [31:0] compare,
<<<<<<< HEAD
    output reg int, pwm
=======
    output reg timer_int, pwm
>>>>>>> e8dcf67e362838e082719eecc5e03d93dba935f3
    );
    
    wire [1:0] mode = control[1:0];
    reg [1:0] prev_mode;
    
    reg [31:0] cnt_pres, count;
    always @(posedge clk, posedge reset) begin
        if(reset) begin
            cnt_pres = 0;
            count = 0;
<<<<<<< HEAD
            int = 0;
=======
            timer_int = 0;
>>>>>>> e8dcf67e362838e082719eecc5e03d93dba935f3
            pwm = 0;
            prev_mode = 0;
        end
        else begin
            if(prev_mode != mode) begin
                cnt_pres = 0;
                count = 0;
<<<<<<< HEAD
                int = 0;
                pwm = 0;
                prev_mode = mode;
            end
            // int
=======
                timer_int = 0;
                pwm = 0;
                prev_mode = mode;
            end
            // timer_int
>>>>>>> e8dcf67e362838e082719eecc5e03d93dba935f3
            if(mode == 2'b01) begin
                if(cnt_pres < prescalor)
                    cnt_pres = cnt_pres + 1;     
                else begin
                    cnt_pres = 0;
                    if(count < max_count) begin
                        count = count + 1;
<<<<<<< HEAD
                        int = 0;
                    end
                    else begin
                        count = 0;
                        int = 1;
=======
                        timer_int = 0;
                    end
                    else begin
                        count = 0;
                        timer_int = 1;
>>>>>>> e8dcf67e362838e082719eecc5e03d93dba935f3
                    end
                end
            end           
            else if(mode == 2'b10) begin
                if(cnt_pres)
                    cnt_pres = cnt_pres + 1;
                else begin
                    if(count < max_count)
                        count = count + 1;
                    else
                        count = 0;
                    if(count < compare)
                        pwm = 1;
                    else
                        pwm = 0;
                end
            end
        end
    end
    
endmodule