#include "timer.h"
#include "REG51F020.H"

void timer_init(int timer_index)
{
if (timer_index==0)
{
/*--------------------------------------
Set Timer0 for 16-bit timer mode.  The
timer counts to 65535, overflows, and
generates an interrupt.

Set the Timer0 Run control bit.
--------------------------------------*/
TMOD = (TMOD & 0xF0) | 0x01;  /* Set T/C0 Mode */
ET0 = 1;                      /* Enable Timer 0 Interrupts */
TR0 = 1;                      /* Start Timer 0 Running */
EA = 1;                       /* Global Interrupt Enable */
TL0=0x00;
TH0=0x00;
}
if(timer_index==1) {
/*--------------------------------------
Set Timer1 for 16-bit timer mode.  The
timer counts to 65535, overflows, and
generates an interrupt.

Set the Timer1 Run control bit.
--------------------------------------*/
TMOD = (TMOD & 0x0F) | 0x10;  /* Set T/C0 Mode */
ET1 = 1;                      /* Enable Timer 1 Interrupts */
TR1 = 1;                      /* Start Timer 1 Running */
EA = 1;                       /* Global Interrupt Enable */
TL1=0x00;
TH1=0x00;
}
}


void timer_close(int timer_index)
{
if(timer_index==0)
{
/*--------------------------------------
Clear the Timer0 Run control bit.
--------------------------------------*/
ET0 = 0;                      /* disable Timer 0 Interrupts */
TR0 = 0;                      /* Stop Timer 0 Running */
EA = 0;                       /* Global Interrupt Disable */
TL0=0x00;
TH0=0x00;
}
if(timer_index==1)
{
/*--------------------------------------
Clear the Timer1 Run control bit.
--------------------------------------*/
ET1 = 0;                      /* disable Timer 1 Interrupts */
TR1 = 0;                      /* Stop Timer 1 Running */
EA = 0;                       /* Global Interrupt Disable */
TL1=0x00;
TH1=0x00;
}
}

/***How to use Interrupt with timer:
 * ------------------------------------------------
*Timer 1 Interrupt Service Routine.
 
 timer1 has the code inturrupt 3
 timer 0 has the code 1
------------------------------------------------
static unsigned long overflow_count = 0;
static unsigned long seconds=4;

void timer1_ISR (void) interrupt 3
{
overflow_count++;   //Increment the overflow count 
if (overflow_count == 15*seconds)  // 15 is the magical number which make it accuratly a second//8*
{
    P1=0x01;
    timer_close(1);
}
}
*/




void delay(unsigned  long int  num)
{
 unsigned long int j;
 for(j=0;j<1257*5* num;j++);  // for 1s delay //5*1257
}



//another code for delay:
// void delay (num_of_milliscnds)
//   {
//     unsigned long int i=0;
//     for (i=0;i<10*num_of_milliscnds;i++)
//     TMOD = 0x01;    // Timer 0 Mode 1
//     TH0= 0;     //initial value for 1ms
//     TL0 = 0;
//     TR0 = 1;     // timer start
//     while (TF0 == 0); // check overflow condition
//     TR0 = 0;    // Stop Timer
//     TF0 = 0;   // Clear flag
//   }
