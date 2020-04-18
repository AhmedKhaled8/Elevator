#include <REG51F020.H>
#include "lib\DCMOTOR.h"
#include "lib\timer.h"
#include "lib\button.h"
#include "lib\seven_segment.h"
#include "lib\LED.h"




void int_INIT(void);
char goingUp(void);
char goingDown(void);
void move(void);
void elevatorUp(void);
void elevatorDown(void);
void elevatorStop(void);
void orUp(void);
void orDown(void);


unsigned char up_array[5] = {0, 0, 0, 0, 0};
unsigned char down_array[5] = {0, 0, 0, 0, 0};

void btn_isr(void);
void init(void) ;



char floorSelected[] = {0, 0, 0, 0, 0};

char totalUpSelected[] = {0, 0, 0, 0, 0};
char totalDownSelected[] = {0, 0, 0, 0, 0};

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
	init();    // int_INIT();
    P1 = 0x3f ; // input 

	seven_seg_vinit('2');
	seven_seg_write('2', 0);
	initDCMotor('3', 5, 6, 7);


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
	
	LED_vInit('0', 6);
	LED_vInit('0', 7);
	LED_vInit('3', 4);
	

	LED_vTurnOff('0', 6); //UP
	LED_vTurnOff('0', 7); //WAIT
	LED_vTurnOff('3', 4); //DOWN
	

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
		if(totalUpSelected[i]==1)
		{
			state =  1;
			break;
		}
	}
	if (state == 1)
	{
		LED_vTurnOn('0', 6);
	}
	else
	{
		LED_vTurnOff('0', 6);
	}
	return state;
}


char goingDown()
{
	char i = currentFloor;
	char state = 0;
	for(;i>0;i--)
	{
		if(totalDownSelected[i-1]==1)
		{
			state =  1;
			break;
		}
	}
	if (state == 1)
	{
		LED_vTurnOn('3', 4);
	}
	else
	{
		LED_vTurnOff('3', 4);
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
	rotate_f('3', 5, 6, 7);
	timer_init(1);
	timer1Set = 1;
	moving = 1;
}
void elevatorDown()
{
	rotate_b('3', 5, 6, 7);
	timer_init(1);
	timer1Set = 1;
	moving = 1;
}
void elevatorStop()
{
	breaks('3', 5, 6, 7);
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
		if(movingUp==1)
		{
			if(totalUpSelected[currentFloor] == 1)
			{
				LED_vTurnOn('0', 6);
				delay(5);
				LED_vTurnOff('0', 6);
				totalUpSelected[currentFloor]=0;
			}
		}
		else if (movingDown==1)
		{
			if(totalDownSelected[currentFloor] == 1)
			{
				LED_vTurnOn('1', 1);
				delay(5);
				LED_vTurnOff('1', 1);
				totalDownSelected[currentFloor]=0;
			}
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
	orUp();
	orDown();
}
void init(void){
    
	EA  = 1;	// Enable global interrupt
	EX1 = 1;    // Enable Ext. interrupt0 
	IT1 = 1;    // Select Ext. interrupt0 on falling edge 
}
void btn_isr (void) interrupt 2 {
    /*
        interrupt Service Routine 
        - sets the up and down calls 
    */
    unsigned char x, y;
    // up array check 
    for(x =0; x <= 8; x+=2){
        up_array[x/2]= up_array[x/2] | READ_BIT(P1, x);
        
        // Testing Lines 
        // if(up_array[x/2] == 1){
        //     SET_BIT(P2, x);
        // }
    } 
    
    // down array check 
    for(y =1; y <= 8; y+=2){
        down_array[(y-1)/2]= down_array[(y-1)/2] | READ_BIT(P1, y);
        
        // Testing Lines 
        // if(down_array[(y-1)/2] == 1){
        //     SET_BIT(P2, y);
        // }
    }
	orUp();
	orDown();
}
void orUp()
{
	char i = 0;
	for(i=0; i<5; i++)
	{
		totalUpSelected[i] = floorSelected[i] | up_array[i];
	}
}
void orDown()
{
	char i = 0;
	for(i = 0; i<5; i++)
	{
		totalDownSelected[i] = floorSelected[i] | down_array[i];
	}
}