#include <REG51F020.H>
#include "DCMOTOR.h"
#include "timer.h"
#include "button.h"
#include "seven_segment.h"
#include "LED.h"




void int_INIT(void);
char goingUp(void);
char goingDown(void);
void move(void);
void elevatorUp(void);
void elevatorDown(void);
void elevatorStop(void);





char floorSelected[] = {0, 0, 0, 0, 0};
unsigned char OpenDoorFlag=0;
char currentFloor = 0;


static unsigned long overflow_count = 0;
static unsigned long seconds=4;
// timer1 has the code inturrupt 3
// timer 0 has the code 1

char movingUp = 1;
char movingDown = 0;
unsigned char timer1Set = 0;
char moving = 0;



void main()
{
    int_INIT();
	//initialize button
	seven_seg_vinit('2');
	seven_seg_write('2', 0);
	initDCMotor('1', 5, 6, 7);


	// UP PUSHBUTTONS INSIDE ELEVATOR
	button_vInit('0', 0);
	button_vInit('0', 1);
	button_vInit('0', 2);
	button_vInit('0', 3);
	button_vInit('0', 4);
	button_vInit('0', 5);

	
    IT0 = 1;
    EX0 = 1;
    EA=1;


	// LEDs to check button Readings
	
	LED_vInit('1', 0);
	LED_vInit('1', 1);
	LED_vInit('1', 2);
	LED_vInit('1', 3);
	

	LED_vTurnOff('1', 0);
	LED_vTurnOff('1', 1);
	LED_vTurnOff('1', 2);
	LED_vTurnOff('1', 3);


	while(1)
	{
	if(button_u8read('0', 0))
	{
		floorSelected[0] = 1;
		delayM(200);
	}
	if(button_u8read('0', 1))
	{
		floorSelected[1] = 1;
		delayM(200);       
	}
	if(button_u8read('0', 2))
	{
		floorSelected[2] = 1;
		delayM(200);
	}
	if(button_u8read('0', 3))
	{
		floorSelected[3] = 1;
		delayM(200);
	}
	if(button_u8read('0', 4))
	{
		floorSelected[4] = 1;
		delayM(200);
	}
	move();
	}

   
}


void int_INIT(void) {
    // setting interrupt zero 
    IT0 = 1 ; // type Edge Sensitive 
    // PX0 = 1 ; // Set HIGH priority  
    // setting interrupt one
    IT1 = 1; // type Edge Sensitive
    // PX1 = 1 ; // Set HIGH priority  
    
    // Enable 
    // EX0 = 1; 
    // EA = 1 ;
    // EX1 = 1 ;
    IE = 0x85; 
}

char goingUp()
{
	char i = currentFloor+1;
	char state = 0;
	for(;i<5;i++)
	{
		if(floorSelected[i]==1)
		{
			state =  1;
			break;
		}
	}
	if (state == 1)
	{
		LED_vTurnOn('1', 0);
	}
	else
	{
		LED_vTurnOff('1', 0);
	}
	return state;
}


char goingDown()
{
	char i = currentFloor;
	char state = 0;
	for(;i>0;i--)
	{
		if(floorSelected[i-1]==1)
		{
			state =  1;
			break;
		}
	}
	if (state == 1)
	{
		LED_vTurnOn('1', 2);
	}
	else
	{
		LED_vTurnOff('1', 2);
	}
	return state;
}

void move()
{
	char upState = 0;
	char downState = 0;
	upState = goingUp();
	downState = goingDown();
	if(moving==0)
	{
		upState = goingUp();
		downState = goingDown();
		if(movingUp == 1)
		{
			if(upState == 1)
			{
				elevatorUp();
			}
			else if (downState == 1)
			{
				movingDown=1;
				movingUp=0;
				elevatorDown();
			}
		}
		else if(movingDown==1)
		{
			if(downState==1)
			{
				elevatorDown();
			}
			else if(upState==1)
			{
				movingUp=1;
				movingDown=0;
				elevatorUp();
			}		
		}

	}

}


void elevatorUp()
{
	rotate_f('1', 5, 6, 7);
	timer_init(1);
	timer1Set = 1;
	moving = 1;
}


void elevatorDown()
{
	rotate_b('1', 5, 6, 7);
	timer_init(1);
	timer1Set = 1;
	moving = 1;
}

void elevatorStop()
{
	breaks('1', 5, 6, 7);
	moving = 0;
}


void timer1_ISR (void) interrupt 3
{
	overflow_count++;   //Increment the overflow count
	if (overflow_count == 15*seconds)  // 15 is the magical number which make it accuratly a second//8*
	{
	    timer_close(1);
		overflow_count=0;
		moving = 0;
		elevatorStop();
		if(movingUp==1)
		{
			currentFloor++;
		}
		else if(movingDown==1)
		{
			currentFloor--;
		}
		seven_seg_write('2', currentFloor);
		if(floorSelected[currentFloor] == 1)
		{
			LED_vTurnOn('1', 1);
			delay(5);
			LED_vTurnOff('1', 1);
			floorSelected[currentFloor]=0;
		}
	}
}

void ex0_isr (void) interrupt 0
{
	
    //P1=0x01;//led on
	unsigned char i =0;
	LED_vToggle('1', 3);
    for(i=0; i<5;i++)//LOOPING IN ALL floors button
	{
		if(i!=currentFloor)
    	floorSelected[i]= floorSelected[i] | button_u8read('0',i);//reading the value of buttons and keeeping it
	}
    OpenDoorFlag= OpenDoorFlag|button_u8read('0',5);//reading the value of openDoor and keeeping it

}

