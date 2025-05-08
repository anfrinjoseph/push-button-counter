#include <xc.h>
#include<stdio.h>

// Define output and input ranges for conversion
#define output_start 0
#define output_end 150ul
#define input_start 0
#define input_end 308ul

void data(char b);          // Send data (character) to LCD
void cmd(int a);            // Send command to LCD
void display(const char *p); // Display a string on LCD

void main(void) {
    // Configure ports
    TRISD = 0X00;  // Set PORTD as output (for LCD data)
    TRISC = 0X00;  // Set PORTC as output (for LCD control signals)
    TRISA = 0XFF;  // Set PORTA as input (for ADC input)
    
    // Configure ADC
    ADCON1 = 0X00; // Configure AN0 as analog input and rest as digital
    ADCON2 = 0X86; // Configure ADC acquisition time and conversion clock
    ADCON0 = 0X01; // Enable ADC on channel 0 (AN0)

    // Initialize LCD display with basic commands
    cmd(0X38);
    cmd(0X01);
    cmd(0x0C);
    cmd(0X80); 
    
    while(1) {
        int adc;
        char cnv[15];                // Buffer to hold temperature string
        for(int i = 0; i <= 1000; i++);
        GO = 1;                      // Start ADC conversion
        while(ADIF == 0);            // Wait until conversion is complete
        ADIF = 0;                    // Clear ADC interrupt flag
        adc = ADRES;                 // Read ADC result (10-bit ADC value from 0 to 1023)
        
        // Convert ADC value to temperature
        int a = output_start + ( (float)(output_end - output_start) / (input_end - input_start) ) * (adc - input_start);
        sprintf(cnv, "TEMP value  %4d", a);  // Format temperature string
        
        cmd(0X80);  // Set cursor to first line, first position
        display(cnv); // Display temperature
    }
}

// Function to send a data byte (character) to LCD
void data(char b) {
    PORTD = b;    
    RC0 = 1;     
    RC1 = 1;     
    for(long int i = 0; i < 100; i++);  
    RC1 = 0;     
    for(long int i = 0; i < 5000; i++); 
}

// Function to send a command byte to LCD
void cmd(int a) {
    PORTD = a;    
    RC0 = 0;      
    RC1 = 1;      
    for(long int i = 0; i < 100; i++);  
    RC1 = 0;      
    for(long int i = 0; i < 5000; i++); 
}

// Function to display a string on LCD by sending characters one by one
void display(const char *p) {
    while(*p != '\0') {  
        data(*p);        
        p++;             
    }
}
