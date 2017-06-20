#include "main.h"

#define LAT_DEVI		0.03000
#define LON_DEVI		0.03000

const char *MSG[] = {	"Vehicle Got struck",
						"Vehicle exceeds the speed Limit",
						"Vehicle will arrive in 10 min.",
						" is on leave/absent." 
					};

const char *Name[] = { "",
						"Suhas",
						"Niveditha",
						"Salu"
					};
					
int8u EEMEM LAT_ADDR1[11];
int8u EEMEM LON_ADDR1[11];
int8u EEMEM LAT_ADDR2[11];
int8u EEMEM LON_ADDR2[11];


struct  {
	volatile int8u StoreLoc1:1;
	volatile int8u StoreLoc2:1;
	volatile int8u Found:1;
	volatile int8u trac;
	volatile int8u SpeedLimit:1;
	volatile int8u Struck:1;
	volatile int8u MeasSpeed:1;
	volatile int8u ScanTmr:1;
	volatile int8u OFTmr:1;
	volatile int8u AllClr:1;
	volatile int8u AllCame:1;
}Flags;

extern int8u lcdptr;
enum STATES state = IDEL;

int main(void)
{
	int8u rpm;
	init();
	
	while (TRUE) {
		
		sleep();
		
		if (Flags.StoreLoc1) {
			lcdclrr(1);
			lcdws("Stor'g First Loc");
			beep(1,100);
			StoreLoc(1);
			Flags.StoreLoc1 = 0;
			lcdclrr(1);
		}
	
		if (Flags.StoreLoc2) {
			lcdclrr(1);
			lcdws("Stor'g Secon Loc");
			beep(1,100);
			StoreLoc(2);
			Flags.StoreLoc2 = 0;
			lcdclrr(1);
		}
		
		if (Flags.MeasSpeed == TRUE) {
			PWMDC(rpm = ReadRPM());
			
			if (Flags.SpeedLimit == FALSE && rpm >= UPPER_SPEED_LIMIT) {
				lcdclrr(1);
				lcdws("Speed Limit Ex's");
				dlyms(500);
				beep(1,150);
				GSMEn();
				GSMsndmsg(COLPhNum, MSG[1]);
				Flags.SpeedLimit = TRUE;
			}
			
			if (Flags.Struck == FALSE && rpm <= LOWER_SPEED_LIMIT) {
				lcdclrr(1);
				lcdws("Vehicle Got St'k");
				beep(1,150);
				dlyms(500);
				GSMEn();
				GSMsndmsg(COLPhNum, MSG[0]);
				Flags.Struck = TRUE;
			}
		}
		
		StateMachine();	
	}
	return 0;
}

static int8u TrackLoc(int8u loc)
{
	int8u lat[11];
	int8u lon[11];

	int8u InVldCnt = 0;
	int8u pass[5];
	int8u i;

	for (i = 0; i < 10; i++)
		lat[i] = lon[i] = '0';

	lat[10] = lon[10] = '\0';

	//lcdclrr(1);
	//lcdws("Finding Location");

	ReadLoc(lat,lon);

	if (CompLoc(loc,lat,lon)) {
		lcdclrr(1);
		lcdws(" Location Found");
		beep(1,100);
		dlyms(1000);
		Flags.Found = 1;
		return loc;
	}
	return 0;
}

static void ReadLoc(int8u lat[], int8u lon[])
{
	GPSEn();
	dlyms(500);
	GPSgetloc(lat,lon);
}
static int8u CompLoc(int8u loc, int8u lat[], int8u lon[])
{
	double LatRef;
	double LonRef;
	double latD;
	double lonD;

	int8u latEE[11];
	int8u lonEE[11];

	int8u InVldCnt = 0;
	int8u pass[5];
	int8u i;

	for (i = 0; i < 10; i++)
		latEE[i] = lonEE[i] = '0';

	latEE[10] = lonEE[10] = '\0';

	ReadLatLon(loc, latEE,lonEE);		/* Read REF from EEPROM */	

	LatRef = atof(latEE);
	LonRef = atof(lonEE);

	latD = atof(lat);
	lonD = atof(lon);

	if ((latD >= (LatRef - LAT_DEVI)) && (latD <= (LatRef + LAT_DEVI))) 
		if ((lonD >= (LonRef - LON_DEVI)) && (lonD <= (LonRef + LON_DEVI))) 
			return 1;
	return 0;
}	
static void StoreLoc(int8u loc)
{
	int8u lat[11];
	int8u lon[11];
	int8u i;
	double latD;
	double lonD;
	

	for (i = 0; i < 10; i++)
		lat[i] = lon[i] = '0';

	lat[10] = lon[10] = '\0';

	GPSEn();
	dlyms(500);
	GPSgetloc(lat,lon);

	latD = atof(lat);
	lonD = atof(lon);

	WriteLatLon(loc, lat,lon);
}
static void init(void)
{
	buzinit();
	beep(2,100);
	
	GSM_GPS_DDR  |= _BV(RFID_GSMP_PIN);
	GSM_GPS_PORT &= ~_BV(RFID_GSMP_PIN);
		
	lcdinit();
	adcinit();
	ledinit();
	motorinit();			
	uartinit();
	EXTINTinit();
	GPSinit();
	GSMEn();
	
	GSMinit();
	tmr1init();
	PWMinit();
	varinit(); 
	disptitl();
	sei();
	StartTmr();
	beep(1,100);
	
}

static void varinit(void) 
{
	Flags.StoreLoc1 = 0;
	Flags.StoreLoc2 = 0;
	Flags.Found = 0;	
	Flags.SpeedLimit = FALSE;
	Flags.Struck = FALSE;
	Flags.MeasSpeed = FALSE;
	Flags.OFTmr = FALSE;
	Flags.ScanTmr = FALSE;
	Flags.AllClr = FALSE;
	Flags.AllCame = FALSE;
}

static void EXTINTinit(void)
{
	INTDDR 	&= ~_BV(INT0_PIN);
	INTPORT |= _BV(INT0_PIN);

	INTDDR 	&= ~_BV(INT1_PIN);
	INTPORT |= _BV(INT1_PIN);

	GICR |= _BV(INT0) | _BV(INT1);			//ENABLE EXTERNAL INTERRUPT
	MCUCR |= _BV(ISC01) | _BV(ISC11);		//FALLING EDGE INTERRUPT

}
static void disptitl(void)
{
	lcdclr();
	lcdws("School Chi'n Tra");
}
		
static void tmr1init(void)
{
	TCNT1H   = 0xD3;
	TCNT1L   = 0x00;
	TIMSK   |= _BV(TOIE1);			//ENABLE OVERFLOW INTERRUPT
	TCCR1A   = 0x00;					
	TCCR1B  |= _BV(CS10) | _BV(CS11); /* PRESCALAR BY 16 */
}

/* overflows at every 100msec */
ISR(TIMER1_OVF_vect) 
{ 
	static int8u i,j,k;

	TCNT1H = 0xD3;
	TCNT1L = 0x00;
	
	if (++i >= 50)
		i = 0;
	
	switch(i) {
		case 0: case 2: ledon(); break;
		case 1: case 3: ledoff(); break;
	} 
	
	if (Flags.ScanTmr == TRUE && ++j >= 100) {
		Flags.OFTmr = TRUE;
		Flags.ScanTmr = FALSE;
		j = 0;
	}
	
	PWMDC(ReadRPM());	/* Control Speed */
}
ISR(INT0_vect) 
{ 
	Flags.StoreLoc1 = 1;
	GICR |= _BV(INT0);
}
ISR(INT1_vect) 
{ 

	Flags.StoreLoc2 = 1;
	GICR |= _BV(INT1);
}
void WriteLatLon(int8u loc, int8u lat[], int8u lon[])
{
	if (loc == 1) {
		eeprom_update_block ((const void *)lat, (void *)LAT_ADDR1, 11);
		eeprom_update_block ((const void *)lon, (void *)LON_ADDR1, 11);
	} else if (loc == 2) {
		eeprom_update_block ((const void *)lat, (void *)LAT_ADDR2, 11);
		eeprom_update_block ((const void *)lon, (void *)LON_ADDR2, 11);
	}
}
void ReadLatLon(int8u loc, int8u lat[], int8u lon[])
{
	if (loc == 1) {
		eeprom_read_block ((void *)lat , (const void *)LAT_ADDR1, 11) ;
		eeprom_read_block ((void *)lon, (const void *)LON_ADDR1, 11) ;
	} else if (loc == 2) {
		eeprom_read_block ((void *)lat , (const void *)LAT_ADDR2, 11) ;
		eeprom_read_block ((void *)lon, (const void *)LON_ADDR2, 11) ;
	}
}

static void StateMachine(void)
{
	int i, StuNo;
	char msg[50];
	
	static Path;
	static int8u Att[4] = {0,0,0,0};
	static int8u getdown[4] = {0,0,0,0};
	static int8u Enter[4] = {0,0,0,0};	
		
	msg[0] = '\0';
	
	switch (state) {
		case IDEL:
				lcdclrr(1);
				lcdws(" About to Move");
				Path = TO_SCHOOL;
				beep(1,250);
				if ( readtag() == DRIVER_TAG) {
					lcdclrr(1);
					lcdws("Starting Vehicle");
					beep(1,250);
					StartVehic();
					Flags.MeasSpeed = TRUE;
					state = SEAR_LOC1;
				}
				break;
		case SEAR_LOC1:
				lcdclrr(1);
				lcdws("Track Location-1");
				beep(1,150);
				if (TrackLoc(1) == LOCATION1) 
					state = GOT_LOC1;
				break;
		case GOT_LOC1:
				lcdclrr(1);
				lcdws("Near Location-1");
				beep(1,250);
				dlyms(500);
				GSMEn();
				GSMsndmsg(P1PhNum, MSG[2]);
				GSMsndmsg(P2PhNum, MSG[2]);
				dlyms(5000);
				state = LOC1;
				break;
		case LOC1:
				lcdclrr(1);
				lcdws("Location-1 Reach");
				dlyms(500);
				StopVehic();
				Flags.MeasSpeed = FALSE;
				if (Path == TO_SCHOOL) {
					lcdclrr(1);
					lcdws(" Scan Students");
					beep(1,250);
					Flags.ScanTmr = TRUE;
					while (Flags.OFTmr == FALSE) {
						if ((StuNo = readtag()) == GOUTHAMI_TAG) {
							lcdclrr(1);
							lcdws("   WRONG BUS!");
							beep(1,500);
						} else if (StuNo == SUHAS_TAG || StuNo == NIVEDITHA_TAG || StuNo == SALU_TAG) {
							lcdclrr(1);
							lcdws(Name[StuNo]);
							lcdws(" REG");
							beep(1,500);
							Att[StuNo] = 1;
						}
					}
					Flags.OFTmr = FALSE;
					lcdclrr(1);
					lcdws(" Scan Complete");
					beep(1,250);
					for (i = 1; i <= 2; i++) {
						if (Att[i] == 0) {
							strcat(msg, Name[i]);
							strcat(msg, MSG[3]);
							GSMEn();
							GSMsndmsg(COLPhNum, msg);
							msg[0] = '\0';
						}
					}
					state = SEAR_LOC2;
				} else {
					state = FINAL_PARK;
					
				}
				
				lcdclrr(1);
				lcdws("Starting Vehicle");
				beep(1,250);
				StartVehic();
				Flags.MeasSpeed = TRUE;
				dlyms(5000);
				break;
		case SEAR_LOC2:
				lcdclrr(1);
				lcdws("Track Location-2");
				beep(1,150);
				if (TrackLoc(2) == LOCATION2)
					state = GOT_LOC2;
				break;
		case GOT_LOC2:
				lcdclrr(1);
				lcdws("Near Location-2");
				beep(1,150);
				GSMEn();
				GSMsndmsg(P3PhNum, MSG[2]);
				dlyms(5000);	
				state = LOC2;
				break;
		case LOC2:
				lcdclrr(1);
				lcdws("Location-2 Reach");
				dlyms(500);
				StopVehic();
				Flags.MeasSpeed = FALSE;
				if (Path == TO_SCHOOL) {
					lcdclrr(1);
					lcdws(" Scan Students");
					dlyms(500);
					beep(1,250);
					Flags.ScanTmr = TRUE;
					while (Flags.OFTmr == FALSE) {
						if ((StuNo = readtag()) == GOUTHAMI_TAG) {
							lcdclrr(1);
							lcdws("   WRONG BUS!");
							beep(1,500);
							} else if (StuNo == SUHAS_TAG || StuNo == NIVEDITHA_TAG || StuNo == SALU_TAG) {
							lcdclrr(1);
							lcdws(Name[StuNo]);
							lcdws(" REG");
							beep(1,500);
							Att[StuNo] = 1;
						}
					}
					Flags.OFTmr == FALSE;
					lcdclrr(1);
					lcdws(" Scan Complete");
					beep(1,250);
					for (i = 3; i <= 3; i++) {
						if (Att[i] == 0) {
							strcat(msg, Name[i]);
							strcat(msg, MSG[3]);
							GSMEn();
							GSMsndmsg(COLPhNum, msg);
							msg[0] = '\0';
						}
					}
					StartVehic();
					lcdclrr(1);
					lcdws("Starting Vehicle");
					beep(1,250);
					dlyms(5000);
					state = GOT_SCHOOL;
				} else {
					dlyms(5000);
					StartVehic();
					lcdclrr(1);
					lcdws("Starting Vehicle");
					beep(1,250);
					state = SEAR_LOC1;		
				}
				Flags.MeasSpeed = TRUE;
				break;
		case GOT_SCHOOL:
				lcdclrr(1);
				lcdws(" School Reached");
				beep(1,250);
				StopVehic();
				Flags.MeasSpeed = FALSE;
				lcdclrr(1);
				lcdws(" Scan Students");
				beep(1,250);
				
				while (Flags.AllClr == FALSE) {
					getdown[readtag()] = 1;
					
					if (getdown[1] == Att[1] && getdown[2] == Att[2] && getdown[3] == Att[3]) 
							Flags.AllClr = TRUE;
									
					beep(5,75);
				}
				
				lcdclrr(1);
				lcdws("Students Cleared");
				beep(1,150);
				
				while (readtag() != DRIVER_TAG);	 /* driver tag */
				
				lcdclrr(1);
				lcdws(" Vehicle Parked");
				beep(1,500);
				
				dlyms(15000);
				beep(1, 2500);
				state = VEHIC_PARKED;
				break;
		case VEHIC_PARKED:
				lcdclrr(1);
				lcdws(" Scan Students");
				beep(1,250);
				
				while (Flags.AllCame == FALSE) {
					Enter[readtag()] = 1;
					
					if (Enter[1] == Att[1] && Enter[2] == Att[2] && Enter[3] == Att[3])
					Flags.AllCame = TRUE;
					
					beep(2,100);
				}
				lcdclrr(1);
				lcdws("Starting Vehicle");
				beep(1,250);
				
				StartVehic();
				Flags.MeasSpeed = TRUE;
				Path = TO_HOME;
				state = SEAR_LOC2;
				break;
		case FINAL_PARK:
				StopVehic();
				lcdclrr(1);
				lcdws("Vehicle Parked");
				beep(1,1000);
				dlyms(1000);
				lcdclrr(1);
				lcdws(" Vehicle Halted");
				Flags.MeasSpeed = FALSE;
				state = HALT;
				break;
		case HALT:
				beep(2,75);
				dlyms(1000);
				break;
	}
}
