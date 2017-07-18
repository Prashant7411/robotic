#include<reg52.h>
#include<stdio.h>
unsigned int a;
void receive()
{
TMOD=0X20;	 	//TIMER 1 AUTO RELOAD
TH1=0XFD;	 	// SET BAUDRATE 9600
SCON=0X50;       	// to set 8bit UART
TR1=1;		  	//starts timer

  while(RI==0);		//receive interrupt flag bit 
  a=SBUF; 		// value form bluetooth stores to sbuf, which is trasnferd to "a"
  RI=0;  
}
void main()
{         		//setting P0 to Zero
while(1)
{
receive(); 		// go to recevie function
if(a=='f') 		// compares value if "a" is equal to value alpha 'a' form bluetooth
{
  P2=0x0a; 	        // set realy to zero
}
if(a=='b') 		//compares value if "a" is equal to value alpha 'A' form bluetooth
{
  P2=0x05;	// set realy to high
}
if(a=='l') 		//similalry continous
{
  P2=0x06;
}
if(a=='r')
{
  P2=0x09;
}
if(a=='s')
{
  P2=0x00;
}
}
}

