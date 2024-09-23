`timescale 1ns / 1ps

module pwm_soc_tb();
    
    reg clk, reset;
    reg [4:0] control;
    reg [9:0] duty_cycle;
    wire pwm;
    pwm_soc uut(
        clk, reset, control, duty_cycle, pwm
    );
    
    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end
    
    initial begin
        reset = 1;
        #10;
        reset = 0;
        
        duty_cycle = 100;
        #100;
        control = 5'b00_00_1;
        #100;
        control = 5'b01_01_1;
        #10000;
        $finish;
    end

endmodule