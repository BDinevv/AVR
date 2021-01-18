/*
 * Praktikum 2.c
 *
 * Created: 24.10.2020 13:50:15
 * Author : User_1
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
 int i = 0;
 int k = 0;
 
 void Hin()
 {
	   // HIN
	for (i=0;i<8;i++) // goes from D0 - D7
	{
		   
		PORTD=(1<<i); // and let them shine!
		_delay_ms(200); // Wait !
		   
			if (i == 7) // when we reach D7
		    {
				for (k = 0;k<2;k++) // go from B0-B1
			     {
					   PORTD = 0x00;// we turn D off
					   PORTB=(1<<k); // turn B0 on and then B1 on
					   _delay_ms(200); // wait
					   if (k == 1) //when we reach B1 -> we turn B off.
					   {
						   
						   PORTB = 0x00;
					   }
			      }
		     }//end if
	   } //end for
 }
 void Ruck()
 {
	 // RUCK
	 PORTB=(1<<0); // Port B0 gets to shine, just B0 because B1 was just on!!
	 _delay_ms(200);
	 PORTB=0x00; // then we let it be and turn it off
	 
	 for (i=7;i>0;i--) // then we go back from D7 to D0
	 {
		 PORTD=(1<<i);
		 _delay_ms(200);
	 }
 }
int main(void)
{
	DDRD = 0b11111111; // Datenrichtungsregister setzen for D and B.
	DDRB = 0b11111111;
    
  while(1)
  {
	  
	//1/5 sec = 0,2 mili-sec
	//Stimmen die Zeiten nicht, ist das ein deutlicher Hinweis darauf, dass die Tankfrequenz nicht mit der in F_CPU ubereinstimmt (zB nehmen wir ein LED der 1 s. aus sein muss)
	// e) 176 bytes <-> 434 bytes
	// ||                                ||                                        ||                                    ||
	//			O1									434 bytes
	//			O2									456 bytes
	//			O3									444 bytes
	//			Os									458 bytes									
	Hin();
	Ruck();	
				 
	}
  }
   

