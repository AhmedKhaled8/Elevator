#include <REG51F020.H>
#include "std_macros.h"
#include "DIO.h"
#include "seven_segment.h"
#include "LED.h"
#include "button.h"
#include "timer.h"
#include "DCMOTOR.h"

static unsigned long overflow_count = 0;
static unsigned long seconds = 4;
static char currentFloor = 0;
unsigned char timer1Set = 0;
unsigned char movingUp = 0;
unsigned char movingDown = 0;

void moveUp(void);
void moveDown(void);

int main(void)
{
    LED_vInit('1', 0);
    initDCMotor('2', 0, 1, 2);
    button_vInit('1', 7);
    button_vInit('1', 6);
    seven_seg_vinit('3');
    while (1)
    {
        if (!button_u8read('1', 7))
        {
            moveDown();
        }
        if (!button_u8read('1', 6))
        {
            moveUp();
        }
        seven_seg_write('3',currentFloor);
    }
    return 0;
}

void timer1_ISR(void) interrupt 3
{
    overflow_count++;                   //Increment the overflow count
    if (overflow_count >= 15 * seconds) // 15 is the magical number which make it accuratly a second//8*
    {
        overflow_count = 0;
        LED_vToggle('1', 0);
        breaks('2', 0, 1, 2); //Stop motor
        timer_close(1);
        timer1Set = 0;
        if (movingUp && currentFloor < 4)
        {
            currentFloor++;
            movingUp = 0;
        }
        if (movingDown && currentFloor > 0)
        {
            currentFloor--;
            movingDown = 0;
        }
    }
}

void moveUp(void)
{
    if (!timer1Set && currentFloor < 4)
    {
        rotate_f('2', 0, 1, 2); // Start motor up
        timer_init(1);
        timer1Set = 1; // Set timer 1 flag
        movingUp = 1;  // Set Up flag
    }
}
void moveDown(void)
{
    if (!timer1Set && currentFloor > 0)
    {
        rotate_b('2', 0, 1, 2); // Start motor down
        timer_init(1);
        timer1Set = 1;  // Set timer 1 flag
        movingDown = 1; // Set Down flag
    }
}
