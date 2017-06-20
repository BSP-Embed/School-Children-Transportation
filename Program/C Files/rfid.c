
#include "rfid.h"

struct  {
	volatile int8u RFIDtag:1;
	
}Flag;

const  char *tag[] = { "",
						"79008425449C",
						"79008425429A",
						"790084253AE2",
						"790084408934",
						"79008425439B",
					};

int8u sbuf[25];

int8u readtag(void)
{
	int8u i, j;
	j = 0;
	Flag.RFIDtag = 0;
	sbuf[0] = '\0';
	
	EnUARTInt();
	RFIDEn();
	
	while (++j < 5 && !Flag.RFIDtag)
		dlyms(1000);
	
	#if defined DISPLAY_TAGNUMBER >= 1
		int8u tag_count ;
		lcdclrr(1);
		lcdws(sbuf);
		for(;;);
	#endif
	
	beep(1,100);
	DisUARTInt();

	for (i = 1; i <= 5; i++)
		if(veritag(sbuf,tag[i]))
			return i;

	return 0;
	
}
static int8u veritag(char *tag_temp, char *tag)
{
	int8u i;
	

	for( i = 0; i < 12 ;i++)
	if(*tag_temp++ != *tag++)
	return 0;
	
	return 1;
}

ISR(USART_RXC_vect)
{
	static int8u i,cnt;

	if ((sbuf[i++] = UDR) && ++cnt == 12) {
		Flag.RFIDtag = 1;
		sbuf[i] = '\0';
		i = 0;
		cnt = 0;
		DisUARTInt();
	}
}
void EnUARTInt(void)
{
	int8u tmp;
	tmp = UDR;
	tmp = UDR;
	tmp = UDR;
	UCSRB |= (1 << 7);	/* Enable receive complete INT */
}
void DisUARTInt(void)
{
	int8u tmp;
	tmp = UDR;
	tmp = UDR;
	tmp = UDR;
	UCSRB &= ~(1 << 7);	/* Disable receive complete INT */
		
}