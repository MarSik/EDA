#include <avr/interrupts.h>
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/sleep.h>

void main(void)
{
    setup();

    while(1) {
        loop();
    }
}
