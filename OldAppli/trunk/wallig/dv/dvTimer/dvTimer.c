/******************************************************************************/
/*                                                                            */
/*  TIME.C:  Time Functions for 1000Hz Clock Tick                             */
/*                                                                            */
/******************************************************************************/
/*  ported to arm-elf-gcc / WinARM by Martin Thomas, KL, .de                  */
/*  <eversmith@heizung-thomas.de>                                             */
/*  modifications Copyright Martin Thomas 2005                                */
/*                                                                            */
/*  Based on file that has been a part of the uVision/ARM development         */
/*  tools, Copyright KEIL ELEKTRONIK GmbH 2002-2004                           */
/******************************************************************************/

/*
  - mt: modified interrupt ISR handling
*/

#include "sys/AT91SAM7S64.h"                    /* AT91SAMT7S64 definitions */
#include "sys/Board.h"
#include "dvISR/interrupt_utils.h"
#include "libTime/libTimer.h"

#ifdef ERAM  /* Fast IRQ functions Run in RAM  - see board.h */
#define ATTR RAMFUNC
#else
#define ATTR
#endif

#define TCK  1000                           /* Timer Clock  */

#define PIV  ((MCK/TCK/16)-1)               /* Periodic Interval Value */


volatile unsigned long timeval;             /* Current Time Tick */

void disk_timerproc (void);
// mt void system_int (void) __irq __atr {        /* System Interrupt Handler */
//void  __attribute__ ((interrupt("IRQ"))) system_int (void) __atr {        /* System Interrupt Handler */
//void  INTFUNC ATTR system_int (void) {        /* System Interrupt Handler */

void  NACKEDFUNC ATTR system_int (void) {        /* System Interrupt Handler */
  volatile AT91S_PITC * pPIT = AT91C_BASE_PITC;

  ISR_ENTRY();

	if (pPIT->PITC_PISR & AT91C_PITC_PITS) 
	{  /* Check PIT Interrupt */
		vFlibTimer_Tick();
		timeval++;                              /* Increment Time Tick */
		if(!(timeval%10))
			disk_timerproc();
		*AT91C_AIC_EOICR = pPIT->PITC_PIVR;     /* Ack & End of Interrupt */
	} 
	else 
		*AT91C_AIC_EOICR = 0;                   /* End of Interrupt */
  
  ISR_EXIT();
}


void vFdvTimer_init(void) {                    /* Setup PIT with Interrupt */
  volatile AT91S_AIC * pAIC = AT91C_BASE_AIC;

  //*AT91C_PIOA_CODR = LED3; 

  *AT91C_PITC_PIMR = AT91C_PITC_PITIEN |    /* PIT Interrupt Enable */ 
                     AT91C_PITC_PITEN  |    /* PIT Enable */
                     PIV;                   /* Periodic Interval Value */ 

  /* Setup System Interrupt Mode and Vector with Priority 7 and Enable it */
  // mt pAIC->AIC_SMR[AT91C_ID_SYS] = AT91C_AIC_SRCTYPE_INT_EDGE_TRIGGERED | 7;
  pAIC->AIC_SMR[AT91C_ID_SYS] = AT91C_AIC_SRCTYPE_INT_POSITIVE_EDGE | 7;
  
  pAIC->AIC_SVR[AT91C_ID_SYS] = (unsigned long) system_int;
  pAIC->AIC_IECR = (1 << AT91C_ID_SYS);
  
  //*AT91C_PIOA_CODR = LED2; 
  
}

void dvTimerwait(unsigned long time)
{
	unsigned long tick;
	
	tick = timeval;
	
	/* Wait for specified Time */
	while ((unsigned long)(timeval - tick) < time);
}
