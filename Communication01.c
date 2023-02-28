#include <p18f4620.h>

#include <stdio.h>
#include <stdlib.h>
#include <delays.h>
#include <usart.h>
#include <adc.h>
#include <string.h>

#include "../My_Header_Lib/CCS.h"
#include "../My_Header_Lib/IO_Device.h"

float mapping(unsigned int data, float a, float b, float c, float d);

int main()
{
    int count = 0;
    unsigned char cache = 0x00;

    Init_RS232C();
    
    printf("----- Start -----\n");

    setup_adc(ADC_CLOCK_DIV_64);
    setup_adc_ports(AN0);
    set_adc_channel(0);

    while (1)
    {

        unsigned char sw = input_b();
        cache |= sw;
        if(DataRdyUSART() == 1){
            char c = ReadUSART();


            if(c == '1') {
                printf("AA\n");
            }else if(c == '2') {
                printf("BBB\n");
            }else if(c == '3') {
                count++;
                printf("%d\n", count);
            }else if(c == 'I') {
                printf("%02X\n", input_b());
            }else if(c == 'S') {
                printf("%d\n", read_adc());
            }else if(c == 'L') {
                //output_bit(PIN_D0, 1);
                output_bit(PIN_D7, 1);
                //printf("L\n");
            }else if(c == 'l') {
                //output_bit(PIN_D0, 0);
                output_bit(PIN_D7, 0);
                //printf("l\n");
            }else if(c == 'D') {
                unsigned char b = input_b() & 0x70;
                printf("%02X\n", b >> 4);
            }else if(c == '0') {
                unsigned int adc = read_adc();
                float tmp = mapping(adc, 0.0f, 1000.0f, -10.0f, 40.0f);
                printf("%d.%01u\n", (int) tmp, (((int) ((tmp > 0 ? tmp : -tmp) * 10)) % 10));
            }else if(c == 'C') {
                printf("%02X\n", cache);
                cache = 0x00;
            }else{
                printf("***** Cmd Error %c *****\n", c);
            }
        }
    }

    return 0;
}

float mapping(unsigned int data, float a, float b, float c, float d){
    if(data < a) {
        return c;
    }else if(data >= b) {
        return d;
    }

    return (data - a) * (d - c) / (b - a) + c;
}