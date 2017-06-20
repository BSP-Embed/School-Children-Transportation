#ifndef _APP_H
#define _APP_H

/* Main settings */

/*comment below line for release*/
#define TESTING
#define EEPROM_DISPLAY				0

/******** MAIN CONFIGURATION ENDS ****************/

/* LCD CONFIGURATION */
#define LCD_DDR           	   		DDRB 
#define LCD_DATA_PORT          		PORTB
#define LCD_ENABLE_PIN        		PB3 
#define LCD_REGISTER_SELECT_PIN     PB2
#define DISPLAY_INIT				1
#define LCD_NSCRL					3
#define FLOAT_T0_STRING				1
#define DECIMAL_PRECISION			100

/******** LCD CONFIGURATION ENDS ****************/

/* BUZZER: CONFIGURATION */
#define	BUZ_DDR						DDRC
#define	BUZ_PORT					PORTC
#define	BUZ_PIN						PC1
#define OFF_TIME					100
/******** BUZZER CONFIGURATION ENDS ****************/

/* LED: CONFIGURATION */

#define	LED_DDR			DDRC
#define	LED_PORT		PORTC
#define	LED_PIN			PC0

#define OFF_TIME		100
/******** LED CONFIGURATION ENDS ****************/

/* GSM: CONFIGURATION */
#define GSM_GPS_DDR				DDRC
#define GSM_GPS_PORT			PORTC
#define RFID_GSMP_PIN			PC6
#define GSM_GPS_PIN				PC7

#define MSG_START					""
#define MSG_END						". Automated SMS By: SCT."


/******** SMS CONFIGURATION ****************/
/*comment below line for release*/
//#define TESTING

#ifdef TESTING
#define COLPhNum	 "9980237552"
#define	P1PhNum		 "9980237552"
#define	P2PhNum		 "9980237552"
#define	P3PhNum		 "9980237552"
#else
#define COLPhNum	 "7337758894"
#define	P1PhNum		 "9686422243"
#define	P2PhNum		 "9886444726"
#define	P3PhNum		 "8050946540"
#endif
/******** SMS CONFIGURATION ENDS ****************/


/******** MOTOR CONFIGURATION   ****************/
#define MOT_PORT			PORTC
#define MOT_DDR				DDRC
#define MOT_ANODE			PC4
#define MOT_CATHODE			PC5
/******** MOTOR CONFIGURATION ENDS  ****************/

/******** PWM CONFIGURATION  *******************/
#define PWM_DDR				DDRD
#define PWM_PIN				PD7
/********  PWM CONFIGURATION ENDS **************/

/******* SERVO CONFIGURATION ********************/
#define SERVO_DDR			DDRD
#define SERVO_PORT			PORTD
#define SERVO1				PD5
/******** SERVO CONFIGURATION ENDS **************/

/********* RELAY CONFIGURATION ******************/
#define RELAY_PORT			PORTC
#define RELAY_DDR			DDRC
#define RELAY				PC7
/********* RELAY CONFIGURATION ENDS ************/

/*********  RFID CONFIGURATION   ***************/
//#define		DISPLAY_TAGNUMBER		0

/*********** RFID CONFIGURATION ENDS ***********/

#endif
