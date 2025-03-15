#include <pic18.h>
#include <string.h>
#include <stdio.h>

void cmd(int);
void data(char);
void delay();
void display(const char *);

int main(void) { // Change void to int
    TRISD = 0X00; // Set PORTD as output
    TRISC = 0X00; // Set PORTC as output
    TRISB = 0XFF; // Set PORTB as input
    ADCON1=0X0F; // Set all pins to digital I/O
    
    cmd(0x38); // Initialize LCD
    cmd(0x80); // Set cursor position
    cmd(0x01); // Clear display
    cmd(0x06); // Entry mode
    cmd(0x0E); // Display on, cursor on

    int count = 0;
    char num[10]; // Character array to hold the string representation
    sprintf(num, "%d", count); // Convert count to string
    display(num); // Display the count
    
    while(1) { 
        if(RB0 == 1) { // Button press detected
            cmd(0x01); // Clear display
            count++; // Increment count
            sprintf(num, "%d", count); // Convert count to string
            display(num); // Display the count
            delay();
        }
        if(RB1 == 1) { // Reset button press detected
            cmd(0x01); // Clear display
            count = 0; // Reset count
            sprintf(num, "%d", count); // Convert count to string
            display(num); // Display the count
            delay();
        }
    }

    return 0; // Return 0 at the end of main
}

void delay() {
    int i, j;
    for(i = 0; i <= 200; i++) {
        for(j = 0; j <= 200; j++) {
            // Do nothing, just delay
        }
    }
}

void display(const char *p) {
    while(*p != '\0') {
        data(*p); // Send each character to the display
        p++;
    }
}

void cmd(int a) {
    LATD = a; // Send command to LCD
    RC0 = 0; // RS = 0 for command
    RC1 = 1; // Enable high
    delay();
    RC1 = 0; // Enable low
}

void data(char b) {
    LATD = b; // Send data to LCD
    RC0 = 1; // RS = 1 for data
    RC1 = 1; // Enable high
    delay();
    RC1 = 0; // Enable low
}