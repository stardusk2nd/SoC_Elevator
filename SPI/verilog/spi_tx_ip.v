`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/09/15 16:19:13
// Design Name: 
// Module Name: spi_lcd
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


module spi_tx_ip(
    input clk, reset,
    input [7:0] data_in,    // 1-byte data to send
    input [7:0] prescaler,  // prescaler for scl clock generating
    input cs,               // on/off control signal (active-low)
    output reg scl,         // spi clock pin
    output reg sda,         // spi data pin (tx, master to slave)
    output reg valid        // end of communication signal
    );
    
    // count for clock divider
    reg [7:0] count;
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
                if(count < prescaler) begin
                    count = count + 1;
                    if(count <= prescaler >> 1)
                        scl = 0;
                    else
                        scl = 1;
                    if(count == prescaler >> 2)
                        sda_sampling = 1;
                    else
                        sda_sampling = 0;
                end
                else begin
                    count = 0;
                    scl = 0;
                end
            end
            else begin
                scl = 0;
                count = 0;
                sda_sampling = 0;
            end
        end
    end
    
    wire scl_p;
    edge_detector edge_detector_inst(
        .clk(clk), .reset(reset), .cp(scl),
        .pedge(scl_p), .nedge()
    );
    
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
            if(waiting && count == scl_p) begin
                waiting = 0;
                valid = 1;
            end
        end
    end
    
endmodule
