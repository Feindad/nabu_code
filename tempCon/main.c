#include <stdio.h>
#include <stdlib.h>

// Function to convert Fahrenheit to Celsius (integer approximation)
int convertToCelsius(int f) {

    // °C = (F - 32) * 5 / 9
    // To keep it integer-based, we compute it directly
    int c = (f - 32) * 5 / 9;
    return c;
}

int main(void) {

    char name[40];     // Buffer for name
    char tempStr[10];  // Buffer for temperature input
    int tempF;         // Fahrenheit temperature
    int tempC;         // Celsius temperature

    
    cprintf("What is your name? ");
    gets(name);  // Get the name
   
    cprintf("\r\nHello, %s!\r\n", name);
    cprintf("Enter temperature in F: ");
    gets(tempStr);  // Get temperature as string
    tempF = atoi(tempStr);  // Convert to integer

    tempC = convertToCelsius(tempF);  // Convert to Celsius
   
    cprintf("Hello, %s!\r\n", name);
    cprintf("%d F = %d C\r\n", tempF, tempC);  // Show result as integers
    cprintf("Press any key to exit.\r\n");
    
   
    return 0;
}