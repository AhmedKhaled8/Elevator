#include "std_macros.h"
#include "INT.h"
#include <stdio.h>
#include <REG51.H>
#include "INT.h"
#include "button.h"
#include "DIO.h"

sbit led = P2^0;
sbit led2 = P2^1;
unsigned char up_array[4] = {0, 0, 0, 0};
unsigned char down_array[4] = {0, 0, 0, 0};

void ex0_isr(void);
void delay(int value); 
void init(void) ;
void check_service(void);

unsigned char x, y;


void main(void)
{

    init();    // int_INIT();
    P1 = 0xff ; // input
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
    // for(x =0; x <= 8; x+=2){
    //         up_array[x/2]= up_array[x/2] | 1;

    //         // led = 1 ;
    //         // delay(6000);
    //         // led = 0 ;
    // } 
    check_service();
}

void delay(int value )   //Delay Function
{
    int i,j;
    for(i=0;i<=value;i++)
    for(j=0;j<=5;j++);
}

void check_service(void){
    unsigned char x, y;

    // up array check 
    for(x =0; x <= 8; x+=2){
        led = 1 ;
        delay(6000);
        led = 0 ;
        delay(2000);
        led = 1 ;
        delay(6000);
        led = 0 ;

        up_array[x/2]= up_array[x/2] | DIO_u8read('1', x);
        // DIO_write('2', x, up_array[x/2]);
    } 
    
    // down array check 
    // for(y =1; y <= 8; y+=2){
        // down_array[(y-1)/2]= down_array[(y-1)/2] | DIO_u8read('1', y);
        // DIO_write('2', y, down_array[(y-1)/2]);
    // }
}

