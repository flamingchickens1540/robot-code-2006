//fire.c by Tom
#include "catlin_fire.h"
#include "ifi_utilities.h"
#include "user_routines.h"
#include <stdio.h>

#include "catlin_routines.h"
#include "ifi_aliases.h"
#include "ifi_default.h"
#include "catlin_aliases.h"
#include "interrupts.h"

int fireCounter;
int fireState;
char haveFired;
int lastFireAngle;
int lastFireCamera;
int fireCloseWait;

void fireInit(void){
	haveFired = FALSE;
	fireCounter = 0;
	fireState = 1;
	lastFireAngle = 0;
	lastFireCamera = 0;	
	fireCloseWait = 0;
}
void fireTest(void)
{
	
	//User_Byte1 = fireState;
	//User_Byte2 = DRIVEDIRECTION;
	//printf("%d \n", fireState);
	//waiting for a ball
	if(fireState == 1)	{
		ONDECKSERVO = CLOSE;//WRONG
		LAUNCHSERVO = CLOSE;
		if((FIRE2 == 1 || FIRE1 == 1) && LAUNCHER > 137){
			fireState = 3;
			haveFired = TRUE;
			fireTest();
		}else if(BALLREADY == SWITCHOFF && fireCloseWait < 24){
			fireCloseWait++;
		}else if(BALLREADY == SWITCHOFF && fireCloseWait >= 24){
			fireState = 2;
			fireTest();
			fireCloseWait = 0;
		}
	}else if(fireState == 2){//waiting to fire
		ONDECKSERVO = OPEN;//wrong
		LAUNCHSERVO = CLOSE;
		if((FIRE2 == 1 || FIRE1 == 1) && LAUNCHER > 137){
			fireState = 3;
			fireTest();
			haveFired = TRUE;
		}else if(BALLREADY == SWITCHON){
			fireState = 1;
			fireTest();
		}
	}else if(fireState == 3){//fire once
		ONDECKSERVO = OPEN;//wrong
		LAUNCHSERVO = OPENED;
		lastFireCamera = TILT_SERVO;
		lastFireAngle = clickRotationsLauncher;
		fireCounter++;
		if(fireCounter > TIMEBALLOUT && (FIRE2 == 0 && FIRE1 == 0)){
			fireState = 1;
			fireCounter = 0;
			fireTest();
		}else if(fireCounter > RAPIDFIREWAIT && (FIRE2 == 1 || FIRE1 == 1)){
			fireState = 4;
			fireCounter = 0;
			fireTest();
		}
	}else if(fireState == 4){//rapid fire
		ONDECKSERVO = CLOSE;//WRONG
		LAUNCHSERVO = OPENED;
		if(FIRE2 == 0 && FIRE1 == 0){
			fireState = 1;
			fireTest();
		}
	}
	
}

void fire(void){
	fireState = 3;
	fireTest();
	haveFired = TRUE;
}

void rapidFire(void){
	fireState = 4;
	ONDECKSERVO = CLOSE;//wrong
	LAUNCHSERVO = OPENED;
	haveFired = TRUE;
	
}
