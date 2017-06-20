#ifndef MAIN_H
#define MAIN_H

#include "includes.h"

#define INTDDR					DDRD
#define INTPORT					PORTD
#define INT0_PIN				PD2
#define INT1_PIN				PD3


//DEFINE CONSTANT
#define UPPER_SPEED_LIMIT		60
#define LOWER_SPEED_LIMIT		30

enum { TO_SCHOOL, TO_HOME};
	
enum {LOCATION0, LOCATION1, LOCATION2};
	
enum {DUMMY=0, SUHAS_TAG, NIVEDITHA_TAG, SALU_TAG, GOUTHAMI_TAG, DRIVER_TAG};
	
enum STATES {
	IDEL, SEAR_LOC1, GOT_LOC1,LOC1,SEAR_LOC2, GOT_LOC2, LOC2, GOT_SCHOOL, VEHIC_PARKED, FINAL_PARK, HALT, 
	};
//DEFINE MACROS
#define StartVehic()		moton()
#define StopVehic()			motoff()

#define StartTmr()			TCCR0  	|= _BV(CS01)
#define StopTmr()			TCCR0  	&= ~_BV(CS01)

#define ReadRPM()			(0.097 * adcget(0))

#define GSMEn()				do {									\
								GSM_GPS_PORT |= _BV(RFID_GSMP_PIN);	\
								GSM_GPS_PORT &= ~_BV(GSM_GPS_PIN); 	\
								dlyms(500);							\
							} while(0)

#define GPSEn()				do {									\
								GSM_GPS_PORT |= _BV(RFID_GSMP_PIN);	\
								GSM_GPS_PORT |= _BV(GSM_GPS_PIN); 	\
								dlyms(500);							\
							} while(0)


//FUNCTION PROTOTYPES
static void		init		(void);
static void 	disptitl 	(void);
static void 	tmr1init	(void);
static void 	EXTINTinit	(void);
static void 	StoreLoc	(int8u loc);
static int8u 	TrackLoc	(int8u loc);
static void 	WriteLatLon	(int8u loc, int8u lat[], int8u lon[]);
static void 	ReadLatLon	(int8u loc, int8u lat[], int8u lon[]);
static void 	ReadLoc		(int8u lat[], int8u lon[]);
static int8u 	CompLoc		(int8u loc,int8u lat[], int8u lon[]);
static void		varinit		(void); 
static void		StateMachine(void);


#endif
