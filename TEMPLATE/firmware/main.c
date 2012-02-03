#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>

int main(void)
{
    setup();

    while(1) {
        loop();
    }

    return 0;
}
