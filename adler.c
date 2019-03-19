/*========================================================================================================================================== //
|-----------.        .---.			                                                                              .---.        .-----------
|  ------    \  __  /     \			Federal Institute of Santa Catarina			2015/2                           /     \  __  /    ------
|    -----    \(  )/     \ \		Discipline: Microcontrolers					Professor: Mayara de Sousa		/ /     \(  )/    -----
|       ---   ` \/     \\\\\\		Students: Gabriel Schoninger					Class: 822				   //////   ' \/ `   ---
|         --- :    : \\ \ \\\\				  Johann Schmitdinger											  //// / // :    : ---
|           --'    `\  \   \ \\		Project: Hand Controller					File: adler.c				 // /   /  /`    '--
|             //..\\          \\																			//          //..\\
|        ====UU====UU====			                                                                               ====UU====UU====
|            '//||\\`				                                                                                   '//||\\`
|              ''``					                                                                                     ''``
/ ========================================================================================================================================== */

// LIBRARIES =============================================================================================================================== //
#include "adler.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// FUNCTIONS =============================================================================================================================== //
int alrInit(unsigned char mode)
{
	int i;
	int baudRate;
	// USART I/O configuration
	clr_bit(BT_DDR, BT_R);			// Configures RX pin as input
	set_bit(BT_PORT, BT_R);			// Actives pull-up for RX pin
	set_bit(BT_DDR, BT_T);			// Configures TX pin as output
	clr_bit(BT_PORT, BT_T);			// Clears TX pin
	// USART initialization
	set_bit(UCSR0A, RXC0);			// Sets receive complete flag
	set_bit(UCSR0A, TXC0);			// Sets transmit complete flag
	set_bit(UCSR0A, UDRE0);			// Sets empty transmit buffer flag
	clr_bit(UCSR0A, FE0);			// Clears frame error
	clr_bit(UCSR0A, DOR0);			// Clears data OverRun
	clr_bit(UCSR0A, UPE0);			// Clears parity error
	set_bit(UCSR0A, U2X0);			// Disables double speed
	set_bit(UCSR0A, MPCM0);			// Disables multi-communication
	set_bit(UCSR0B, RXEN0);			// Enables receiver
	set_bit(UCSR0B, TXEN0);			// Enables transmitter
	clr_bit(UCSR0C, UMSEL00);		// Configures asynchronous mode
	clr_bit(UCSR0C, UMSEL01);		//
	clr_bit(UCSR0C, UPM00);			// Disables parity check
	clr_bit(UCSR0C, UPM01);			//
	set_bit(UCSR0C, USBS0);			// Configures 2 stop bits
	set_bit(UCSR0C, UCSZ00);		// Configures 8 data bits
	set_bit(UCSR0C, UCSZ01);		//
	clr_bit(UCSR0C, UCSZ02);		//
	clr_bit(UCSR0C, UCPOL0);		// Disables clock polarity
	baudRate = F_CPU / 16 / USART_BR;		// Calculates baud rate
	UBRR0H = (unsigned char)(baudRate>>8);	// Configures USART baud rate
	UBRR0L = (unsigned char)baudRate;		//
	// Operation specific configuration
	switch(mode){
	case MODE_CONTROLLER:
		// ADC I/O configuration
		ADC_DDR = 0x00;				// Configures ADC pins as input
		// ADC Timer configuration
		clr_bit(TCCR0A, WGM00);		// Configures timer 0 normal mode
		clr_bit(TCCR0A, WGM01);		//
		clr_bit(TCCR0A, WGM02);		//
		set_bit(TCCR0B, CS00);		// Configures 1024 prescaler
		clr_bit(TCCR0B, CS01);		//
		set_bit(TCCR0B, CS02);		//
		set_bit(TIFR0, TOV0);		// Clears interrupt flag
		TCNT0 = 0;					// Clears counter
		// ADC configuration
		set_bit(ADMUX, REFS0);		// Configures AVcc reference
		clr_bit(ADMUX, REFS1);		//
		set_bit(ADMUX, ADLAR);		// Sets output to left adjusted
		clr_bit(ADMUX, MUX0);		// Connects ADC0 pin to ADC
		clr_bit(ADMUX, MUX1);		//
		clr_bit(ADMUX, MUX2);		//
		clr_bit(ADMUX, MUX3);		//
		set_bit(ADCSRA, ADATE);		// Selects auto mode
		set_bit(ADCSRA, ADIF);		// Clears ADC interrupt request
		set_bit(ADCSRA, ADPS0);		// Sets ADC prescaler to 128
		set_bit(ADCSRA, ADPS1);		//
		set_bit(ADCSRA, ADPS2);		//
		clr_bit(ADCSRB, ADTS0);		// Sets trigger source to timer 0 overflow
		clr_bit(ADCSRB, ADTS1);		//
		set_bit(ADCSRB, ADTS2);		//
		set_bit(ADCSRA, ADIE);		// Enables ADC interrupt
		set_bit(ADCSRA, ADIF);		// Clears interrupt request
		set_bit(ADCSRA, ADEN);		// Enables ADC
		// Variables definitions
		adcMux = 0;					// Starts at first ADC pin
		for(i=0; i<5; i++)			// Clears measured values
			adcRaw[i] = 0;			//
		break;
	case MODE_HAND:
		// Hand I/O configuration
		HND_DDR = 0xFF;				// Configures servo pins as output
		HND_PORT = 0x00;			// Clears servo pins
		// Servo timer configuration
		clr_bit(TCCR2A, WGM20);		// Configures timer 0 on CTC mode
		set_bit(TCCR2A, WGM21);		//
		clr_bit(TCCR2A, WGM22);		//
		clr_bit(TCCR2B, CS20);		// Configures 256 prescaler
		set_bit(TCCR2B, CS21);		//
		set_bit(TCCR2B, CS22);		//
		TCNT2 = 0;					// Clears counter register
		OCR2A = 5;					// Sets compare value
		set_bit(TIMSK2, OCIE2A);	// Enables interrupt request on compare
		set_bit(TIFR2, OCF2A);		// Clears interrupt flag
		// Variables definitions
		autoMode = 0;				// Disables auto mode
		break;
	case MODE_SUPER:
		// General I/O configuration
		ADC_DDR = 0x00;				// Configures ADC pins as input
		HND_DDR = 0xFF;				// Configures servo pins as output
		HND_PORT = 0x00;			// Clears servo pins
		// ADC Timer configuration
		clr_bit(TCCR0A, WGM00);		// Configures timer 0 normal mode
		clr_bit(TCCR0A, WGM01);		//
		clr_bit(TCCR0A, WGM02);		//
		set_bit(TCCR0B, CS00);		// Configures 1024 prescaler
		clr_bit(TCCR0B, CS01);		//
		set_bit(TCCR0B, CS02);		//
		set_bit(TIFR0, TOV0);		// Clears interrupt flag
		TCNT0 = 0;					// Clears counter
		// ADC configuration
		set_bit(ADMUX, REFS0);		// Configures AVcc reference
		clr_bit(ADMUX, REFS1);		//
		set_bit(ADMUX, ADLAR);		// Sets output to left adjusted
		clr_bit(ADMUX, MUX0);		// Connects ADC0 pin to ADC
		clr_bit(ADMUX, MUX1);		//
		clr_bit(ADMUX, MUX2);		//
		clr_bit(ADMUX, MUX3);		//
		set_bit(ADCSRA, ADATE);		// Selects auto mode
		set_bit(ADCSRA, ADIF);		// Clears ADC interrupt request
		set_bit(ADCSRA, ADPS0);		// Sets ADC prescaler to 128
		set_bit(ADCSRA, ADPS1);		//
		set_bit(ADCSRA, ADPS2);		//
		clr_bit(ADCSRB, ADTS0);		// Sets trigger source to timer 0 overflow
		clr_bit(ADCSRB, ADTS1);		//
		set_bit(ADCSRB, ADTS2);		//
		set_bit(ADCSRA, ADIE);		// Enables ADC interrupt
		set_bit(ADCSRA, ADIF);		// Clears interrupt request
		set_bit(ADCSRA, ADEN);		// Enables ADC
		// Servo timer configuration
		clr_bit(TCCR2A, WGM20);		// Configures timer 0 on CTC mode
		set_bit(TCCR2A, WGM21);		//
		clr_bit(TCCR2A, WGM22);		//
		clr_bit(TCCR2B, CS20);		// Configures 256 prescaler
		set_bit(TCCR2B, CS21);		//
		set_bit(TCCR2B, CS22);		//
		TCNT2 = 0;					// Clears counter register
		OCR2A = 5;					// Sets compare value
		set_bit(TIMSK2, OCIE2A);	// Enables interrupt request on compare
		set_bit(TIFR2, OCF2A);		// Clears interrupt flag
		// Variables definitions
		adcMux = 0;					// Starts at first ADC pin
		for(i=0; i<5; i++)			// Clears measured values
		adcRaw[i] = 0;				//
		autoMode = 0;				// Disables auto mode
		break;
	default:
		return 0;
		break;
	}
	sei();
	// Variables definitions
	stage = STAGE_IDLE;
	pwmCount = 0;
	for(i=0; i<6; i++)
		hndCount[i] = 0;
	
	return 1;
}
int alrPwm(unsigned char hndNum, unsigned char pwm)
{
	if((hndNum < 0) | (hndNum > 5) | (pwm<0) | (pwm>255))
	return 0;
	
	hndCount[hndNum] = pwm*0.16;
	
	return 1;
}
void alrTestHand()
{
	alrPwm(1, 200);
	alrPwm(2, 200);
	alrPwm(3, 200);
	alrPwm(4, 200);
	alrPwm(5, 200);
	_delay_ms(2000);
	alrPwm(1, 0);
	alrPwm(2, 0);
	alrPwm(3, 0);
	alrPwm(4, 0);
	alrPwm(5, 0);
	_delay_ms(2000);
	alrPwm(1, 200);
	_delay_ms(1000);
	alrPwm(2, 200);
	_delay_ms(1000);
	alrPwm(3, 200);
	_delay_ms(1000);
	alrPwm(4, 200);
	_delay_ms(1000);
	alrPwm(5, 200);
	_delay_ms(1000);
	alrPwm(5, 0);
	_delay_ms(1000);
	alrPwm(4, 0);
	_delay_ms(1000);
	alrPwm(3, 0);
	_delay_ms(1000);
	alrPwm(2, 0);
	_delay_ms(1000);
	alrPwm(1, 0);
	_delay_ms(1000);
	return;
}
int alrSetHandPose(unsigned char poseNum)
{
	if((poseNum<0) || (poseNum>HAND_POSE_TOTAL))
	return 0;
	else
	hndPose = poseNum;
	return 1;
}
void alrUpdatePose()
{
	if(autoMode){
		alrPwm(0, adcRaw[0]);
		alrPwm(1, adcRaw[1]);
		alrPwm(2, adcRaw[2]);
		alrPwm(3, adcRaw[3]);
		alrPwm(4, adcRaw[4]);
		alrPwm(5, adcRaw[5]);
		} else{
		switch(hndPose){
			case HAND_POSE_OPEN:
			alrPwm(1, 0);
			alrPwm(2, 0);
			alrPwm(3, 0);
			alrPwm(4, 0);
			alrPwm(5, 0);
			break;
			case HAND_POSE_CLOSED:
			alrPwm(1, 255);
			alrPwm(2, 255);
			alrPwm(3, 255);
			alrPwm(4, 255);
			alrPwm(5, 255);
			break;
			case HAND_POSE_LIKE:
			alrPwm(1, 0);
			alrPwm(2, 255);
			alrPwm(3, 255);
			alrPwm(4, 255);
			alrPwm(5, 255);
			break;
			case HAND_POSE_HANG_LOOSE:
			alrPwm(1, 0);
			alrPwm(2, 255);
			alrPwm(3, 255);
			alrPwm(4, 255);
			alrPwm(5, 0);
			break;
			case HAND_POSE_ROCK_N_ROLL:
			alrPwm(1, 255);
			alrPwm(2, 0);
			alrPwm(3, 255);
			alrPwm(4, 255);
			alrPwm(5, 0);
			break;
			case HAND_POSE_PEACE:
			alrPwm(1, 255);
			alrPwm(2, 0);
			alrPwm(3, 0);
			alrPwm(4, 255);
			alrPwm(5, 255);
			break;
			case HAND_POSE_FUCK_YOU:
			alrPwm(1, 255);
			alrPwm(2, 255);
			alrPwm(3, 0);
			alrPwm(4, 255);
			alrPwm(5, 255);
			break;
			default:
			break;
		}
	}
	return;
}
void alrUpdatePwm()
{
	if(pwmCount < 255)
	pwmCount++;
	else
	pwmCount = 0;
	
	if(pwmCount == 0){
		set_bit(HND_PORT, HND_0);
		set_bit(HND_PORT, HND_1);
		set_bit(HND_PORT, HND_2);
		set_bit(HND_PORT, HND_3);
		set_bit(HND_PORT, HND_4);
		set_bit(HND_PORT, HND_5);
	}
	if(pwmCount == hndCount[0])
	clr_bit(HND_PORT, HND_0);
	if(pwmCount == hndCount[1])
	clr_bit(HND_PORT, HND_1);
	if(pwmCount == hndCount[2])
	clr_bit(HND_PORT, HND_2);
	if(pwmCount == hndCount[3])
	clr_bit(HND_PORT, HND_3);
	if(pwmCount == hndCount[4])
	clr_bit(HND_PORT, HND_4);
	if(pwmCount == hndCount[5])
	clr_bit(HND_PORT, HND_5);
	
	return;
}
void alrUpdateAdc()
{
	if(adcMux<5)
	adcMux++;
	else
	adcMux = 0;
	switch(adcMux){
		case 0:
		clr_bit(ADMUX, MUX0);
		clr_bit(ADMUX, MUX1);
		clr_bit(ADMUX, MUX2);
		break;
		case 1:
		set_bit(ADMUX, MUX0);
		clr_bit(ADMUX, MUX1);
		clr_bit(ADMUX, MUX2);
		break;
		case 2:
		clr_bit(ADMUX, MUX0);
		set_bit(ADMUX, MUX1);
		clr_bit(ADMUX, MUX2);
		break;
		case 3:
		set_bit(ADMUX, MUX0);
		set_bit(ADMUX, MUX1);
		clr_bit(ADMUX, MUX2);
		break;
		case 4:
		clr_bit(ADMUX, MUX0);
		clr_bit(ADMUX, MUX1);
		set_bit(ADMUX, MUX2);
		break;
		case 5:
		set_bit(ADMUX, MUX0);
		clr_bit(ADMUX, MUX1);
		set_bit(ADMUX, MUX2);
		break;
	}
	return;
}
void alrAutoModeEnable()
{
	autoMode = 1;
	return;
}
void alrAutoModeDisable()
{
	autoMode = 0;
	return;
}
void alrUsartTransmit(unsigned char tdata)
{
	while(!(UCSR0A & (1<<UDRE0)))
		;
	UDR0 = tdata;
	return;
}
void alrUsartReceive()
{
	return;
}

// INTERRUPTIONS =========================================================================================================================== //
ISR(TIMER2_COMPA_vect)
{
	stage = STAGE_UPDATE_OUTPUT;
	return;
}
ISR(ADC_vect)
{
	set_bit(TIFR0, TOV0);
	adcRaw[adcMux] = ADCH;
	stage = STAGE_UPDATE_INPUT;
	return;
}
