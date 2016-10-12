#include <avr/io.h>
#include "button.h"

#define BIT(x) (1 << (x))

/** Return non-zero if button pressed.  */
int button_pressed_p (void)
{
    return (PIND & BIT(7));
}


/** Initialise button1.  */
void button_init (void)
{
    DDRD |= (0 << 7);
}

