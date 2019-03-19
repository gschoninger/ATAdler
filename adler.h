/*========================================================================================================================================== //
|-----------.        .---.			                                                                              .---.        .-----------
|  ------    \  __  /     \			Federal Institute of Santa Catarina			2015/2                           /     \  __  /    ------
|    -----    \(  )/     \ \		Discipline: Microcontrolers					Professor: Mayara de Sousa		/ /     \(  )/    -----
|       ---   ` \/     \\\\\\		Students: Gabriel Schoninger					Class: 822				   //////   ' \/ `   ---
|         --- :    : \\ \ \\\\				  Johann Schmitdinger											  //// / // :    : ---
|           --'    `\  \   \ \\		Project: Hand Controller					File: adler.h				 // /   /  /`    '--
|             //..\\          \\																			//          //..\\
|        ====UU====UU====			                                                                               ====UU====UU====
|            '//||\\`				                                                                                   '//||\\`
|              ''``					                                                                                     ''``
/ ========================================================================================================================================== */

#ifndef ADLER_H_
#define ADLER_H_

// SYSTEM DEFINITIONS ====================================================================================================================== //
// ATmega DEFINITIONS ---------------------------------------------------------------------------------------------------------------------- //
#define F_CPU					16000000UL
#define USART_BR				9600
// Main loop ------------------------------------------------------------------------------------------------------------------------------- //
#define EWIG					1
#define SABABA					0
// Stages ---------------------------------------------------------------------------------------------------------------------------------- //
#define STAGE_IDLE				0
#define STAGE_UPDATE_OUTPUT		1
#define STAGE_UPDATE_INPUT		2
// Operation mode -------------------------------------------------------------------------------------------------------------------------- //
#define MODE_CONTROLLER			0
#define MODE_HAND				1
#define MODE_SUPER				2
// Hand poses ------------------------------------------------------------------------------------------------------------------------------ //
#define HAND_POSE_TOTAL			6
#define HAND_POSE_OPEN			0
#define HAND_POSE_CLOSED		1
#define HAND_POSE_LIKE			2
#define HAND_POSE_HANG_LOOSE	3
#define HAND_POSE_ROCK_N_ROLL	4
#define HAND_POSE_PEACE			5
#define HAND_POSE_FUCK_YOU		6

// BIT FUNCTIONS =========================================================================================================================== //
#define	set_bit(y,bit)	(y|=(1<<bit))
#define	clr_bit(y,bit)	(y&=~(1<<bit))
#define cpl_bit(y,bit) 	(y^=(1<<bit))
#define tst_bit(y,bit) 	(y&(1<<bit))

// PIN DEFINITIONS ========================================================================================================================= //
// ADC Pins -------------------------------------------------------------------------------------------------------------------------------- //
#define ADC_DDR		DDRC
#define ADC_PORT	PORTC
#define ADC_PIN		PINC
#define ADC_KEY		PC0
#define ADC_X		PC1
#define ADC_Y		PC2
#define ADC_Z		PC3
// Hand pins ------------------------------------------------------------------------------------------------------------------------------- //
#define HND_DDR		DDRB
#define HND_PORT	PORTB
#define HND_PIN		PINB
#define HND_0		PB0
#define HND_1		PB1
#define HND_2		PB2
#define HND_3		PB3
#define HND_4		PB4
#define HND_5		PB5
// Bluethooth pins ------------------------------------------------------------------------------------------------------------------------- //
#define BT_DDR		DDRD
#define BT_PORT		PORTD
#define BT_PIN		PIND
#define BT_R		PD0
#define BT_T		PD1

// VARIAVLES =============================================================================================================================== //
unsigned char hndCount[6];
unsigned char hndPose;
unsigned char pwmCount;
unsigned char adcMux;
unsigned char adcRaw[5];
unsigned char autoMode;
unsigned char stage;

// FUNCTIONS =============================================================================================================================== //
int alrInit(unsigned char mode);
int alrPwm(unsigned char hndNum, unsigned char pwm);
void alrTestHand();
int alrSetHandPose(unsigned char poseNum);
void alrUpdatePose();
void alrUpdatePwm();
void alrUpdateAdc();
void alrAutoModeEnable();
void alrAutoModeDisable();
void alrUsartTransmit(unsigned char tdata);
void alrUsartReceive();

#endif /* ADLER_H_ */