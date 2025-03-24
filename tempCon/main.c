static void orgit() __naked {
  __asm
  org     0x140D
    nop
    nop
    nop
    __endasm;
}

void main2();

void main() {

  main2();
}

#include "../NABULIB/NABU-LIB.h"
#include <stdio.h>
#include <conio.h>  // For Atari console I/O
#include <stdlib.h>


// Function to convert Fahrenheit to Celsius (integer approximation)
int convertToCelsius(int f) {
    // °C = (F - 32) * 5 / 9
    // To keep it integer-based, we compute it directly
    int c = (f - 32) * 5 / 9;
    return c;
}

int main2(void) {
    char name[40];     // Buffer for name
    char tempStr[10];  // Buffer for temperature input
    int tempF;         // Fahrenheit temperature
    int tempC;         // Celsius temperature

    clrscr();  // Clear the screen
    cprintf("What is your name? ");
    gets(name);  // Get the name
   
    cprintf("\r\nHello, %s!\r\n", name);
    cprintf("Enter temperature in F: ");
    gets(tempStr);  // Get temperature as string
    tempF = atoi(tempStr);  // Convert to integer

    tempC = convertToCelsius(tempF);  // Convert to Celsius
   
    clrscr();  // Clear for result
    cprintf("Hello, %s!\r\n", name);
    cprintf("%d F = %d C\r\n", tempF, tempC);  // Show result as integers
    cprintf("Press any key to exit.\r\n");
    cgetc();  // Wait for keypress
   
    return 0;
}