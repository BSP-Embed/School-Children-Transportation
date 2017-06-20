#ifndef _MOTOR_H
#define _MOTOR_H

#include "includes.h"

#define moton()			MOT_PORT |= _BV(MOT_ANODE)
#define motoff()		MOT_PORT &= ~_BV(MOT_ANODE)

void	motorinit	(void);
void	Forward		(void);
void	Backward	(void);
void	Stop		(void);

#endif
