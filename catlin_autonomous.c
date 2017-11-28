/*******************************************************************************
* FILE NAME: catlin_autonomous.c
*
* DESCRIPTION:
*  This file contains the catlin autonomous modes.
*
*******************************************************************************/
#include "ifi_aliases.h"
#include "ifi_default.h"
#include "ifi_utilities.h"
#include "user_routines.h"
#include <stdio.h>
#include "interrupts.h"
#include "catlin_routines.h"
#include "eeprom.h"
#include "catlin_launcher.h"
#include "catlin_fire.h"
#include "catlin_aliases.h"
#include "camera_autonomous_d.h"
#include "tracking.h"
#include "camera.h"
#include "catlin_autonomous.h"


void driveAndShootAuto(char posn) {
	static int firstTime = 0;
	static int p = 0;
	static int fireTimer = 0;
	int distance;
	
	if(firstTime == 0){
		firstTime = 1;
		clickRotationsLeft = 0;
		setLauncher(130);
	}
	if(posn == 3){
		distance = -2200;
	}else if(posn == 2){
		distance = -2400;
	}else{
		distance = -2600;
	}
	//printf("HERE\r");
	
	
	LAUNCHER = 255;
	updateLauncher();
	
	Camera_Handler();
	Servo_Track();
	
	User_Byte1 = clickRotationsLeft % 255;
	User_Byte2 = clickRotationsLeft / 255;
	User_Byte3 = 87;
	
	if(clickRotationsLeft > distance){
		LEFTMOTOR = 255 - 207;
		RIGHTMOTOR = 202;
		ONDECKSERVO = OPEN;//wrong
		LAUNCHSERVO = CLOSE;
	}else if(p < 10){
		LEFTMOTOR = 255 - 197;
		RIGHTMOTOR = 127;
		p++;
		ONDECKSERVO = OPEN;//wrong
		LAUNCHSERVO = CLOSE;
	}else{
		LEFTMOTOR = NEUTRAL;
		RIGHTMOTOR = NEUTRAL;
		
		if(can_see_light_p() == YES){
			autoAim();
			updateLauncher();
			fireTimer++;
			//if(fireTimer > 75){
			//	rapidFire();
			//}else
			if(ready_to_fire_p() == YES && launcher_ready_p() == YES){
				rapidFire();
			}else{
				camera_auto_rotate();
			}
			
		}
	}
	
	Catlin_Common_Last();
}
