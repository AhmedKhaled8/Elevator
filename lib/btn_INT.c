#include "std_macros.h"
#include "INT.h"
#include <stdio.h>
#include "INT.h"
#include "button.h"
#include "DIO.h"
#include "REG51F020.h"

sbit led = P2^0;
sbit led2 = P2^1;
unsigned char up_array[4] = {0, 0, 0, 0};
unsigned char down_array[4] = {0, 0, 0, 0};

void btn_isr(void);
void delay(int value); 
void init(void) ;

unsigned char x, y;


void main(void)
{

    init();    // int_INIT();
    P1 = 0xff ; // input 

    // For Testing 
    P0 = 0x00 ; // output
    P2 = 0x00 ; // OUT

    while (1){ 

    }
}

void init(void){
    
	EA  = 1;	// Enable global interrupt
	EX0 = 1;    // Enable Ext. interrupt0 
	IT0 = 1;    // Select Ext. interrupt0 on falling edge 
}


void btn_isr (void) interrupt 0 {
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
}


void delay(int value )   //Delay Function
{
    int i,j;
    for(i=0;i<=value;i++)
    for(j=0;j<=5;j++);
}


