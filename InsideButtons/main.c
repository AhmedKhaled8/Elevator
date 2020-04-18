#include "lib/REG51F020.h"
#include "lib/button.h"
#include "lib/timer.h"


unsigned char Floors_wanted[5]={0,0,0,0,0};//value of the buttons for floor choosing
unsigned char OpenDoorFlag=0;


void ex0_isr (void) interrupt 0
{
    //P1=0x01;//led on
    unsigned char i =0;
    for(i=0; i<5;i++)//LOOPING IN ALL floors button
    Floors_wanted[i]= Floors_wanted[i] | button_u8read('0',i);//reading the value of buttons and keeeping it
    OpenDoorFlag= OpenDoorFlag|button_u8read('0',5);//reading the value of openDoor and keeeping it
}

void main(void)
{
    //P0=0x00;//initialize button
    P1=0x00;
    P2=0x00;
    P3=0x04;
    
    button_vInit('0',0);
    button_vInit('0',1);
    button_vInit('0',2);
    button_vInit('0',3);
    button_vInit('0',4);
    button_vInit('0',5);


    IT0 = 1;
    EX0 = 1;
    EA=1;

    while (1){}
}

