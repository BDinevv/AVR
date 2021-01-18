/*
 * ARBK_Praktikum.c
 *
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>

// Zähler ms seit programmstart
volatile uint32_t system_clock = 0;
 
ISR(TIMER0_COMPA_vect)
{
	system_clock++;
}

 void waitFor(int32_t ms){
	
	int32_t cl2 = system_clock + ms;
	
	while(system_clock != cl2) {}
	//system_clock = 0;
	//return;
}

void waitUntil(int32_t ms)
{
	
	while(system_clock < ms) {}
	//extraTime = 0;
	//return;
}

int main(void)
{
	// Source: https://www.youtube.com/watch?v=cAui6116XKc&ab_channel=humanHardDrive
	// TC0 - Timer/Counter0 ( 8-bit )
	// Precaler(64) - reduces every tick to a 64th of a tick || increment every 64 clock cycles
	
	// TCNTn - [holds all the ticks] timer counts based on the number of clock cycles plugged into prescaler
	// CTC - clear timer on compare -> jump to interrupt and clear timer for us!
	// BUT Timer wont start until we give it a prescaler
	
	
	//138 - TCCR0A - Timer Count Control Register A - we need to put CTC here WGM01
	
	// 1. Output bestimmen!
	DDRB = 0x03; 
	
	//2. TCCR0A [Timer/Counter Control Register] - WG [Waveform Generator], um output zu generieren
	// we choose - WGM01 = CTC - clear timer on compare match
	// 0 because the 8bit timer
	TCCR0A |= (1 << WGM01);
	
	//3. OCR0A [Output Compare Register, 0 because the 8bit timer] 14 für 1 sec bei 16 Mhz Output Compare Register
	//OCRn =  [ (clock_speed / Prescaler_wert) * timeInSeconds ] - 1
	//OCRn = [ (16000000/1024) * 0,001 ] - 1
	//	   = 14.625
	// => OCRn = OxE;
	OCR0A = 0xE;
	
	//TIMSK [Timer Interrupt Mask] Interrupt wenn Vergleich von Counter und Output Compare Register gleich sind
	// OCIE0A - Sets the ISR COMPA vect, which we use in the interrupt funktion
	TIMSK0 |= (1<< OCIE0A);

	//interrupts activieren
	sei();
	
	// setze prescaler auf 1024
	// |= bitwise OR.
	TCCR0B |= (1 << CS02) | (1 << CS00);
	
	
	//1 * 1000 = 1s
	waitUntil(5000);
	PORTB ^= (1<<PORTB0);	// Port B0 - on
	waitFor(1000);
	PORTB ^= (1<<PORTB0);	// Port B0 - off
	
	while(1)
	{
		//Wait until X ms are reached
		waitFor(2000);
		PORTB ^= (1<<PORTB1);	// Port B0 - on and off ...
	}
}

/*
a) Wie lange dauert es, bis der Zähler überläuft?

1. F(systemClock) = F(Timer) = 16 MHz

2. Wir haben tick in every T(timer) = 1/F(systemClock) =
				                    = 1/F(Timer) = 
									= 1/16 000 000 =
								    = 0,000 000 0 625
   ABER unser Prescaler is auf 1024 gesettet,
   dh unser Timer [ 16 Millionen / 1024 (= 15 625) Takten pro Sekunde ] 
   langsamer als die Frequenz (F) von CPU ->
   -> dann ist unsere
	  Periode = T(timer) = 1/15 625 = 0,000064

3. 8 bit timer means
 ticks max = 2^8 - 1 = 255
 
4. So timer will overflow in every
 t(overflow) = ticks max * T(timer) =
			 = 255 * 0,000064 
			 = 0,01632s 
			 = (ung) 0,016s 


b) Was könnte man tun, um größere Zeiten zu verarbeiten?
 - 1) einen größeren Timer (mehr Bits) 
 - 2) Größen Prescaler benutzen (Prescaler verlangsamt den Timer)
 
*/

