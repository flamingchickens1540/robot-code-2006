/*******************************************************************************
* FILE NAME: catlin_routines.c
*
* DESCRIPTION:
*  This file contains the catlin declarations.
*
* USAGE:
*	Any real editing should be done in this file!
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

/***** VARIABLE DECLARATION *******/

int leftInit;
int rightInit;
int oldLimitBottom;
int oldLimitTop;
char hasZero;
int waitTime;
int autoTimer;
char rotating;
char dirTurning;
int timer;
char direction;

void Catlin_Initialize(void) {
	direction = LEFT;
	timer = 0;
	waitTime = 0;
	autoTimer = 0;
	rotating = NO;

	leftInit = 0;
	rightInit = 0;
	oldLimitBottom = SWITCHOFF;
	oldLimitTop = SWITCHOFF;
	hasZero = 0;
	
	dirTurning = 0;
	
	PAN_SERVO = PAN_CENTER_PWM_DEFAULT;		// CAMERA
	TILT_SERVO = TILT_CENTER_PWM_DEFAULT;	// CAMERA

//	Launcher Initialization
	launcherInit();
	
//	Fire Initialization
	fireInit();

}

void Catlin_Common_Last(void) {
	
	if(light_centered_p()) CAMERALOCKLED = LEDON;
	else CAMERALOCKLED = LEDOFF;
	
	if(BALLREADY) BALLREADYLED = LEDON;
	else BALLREADYLED = LEDOFF;
	
	if(ready_to_fire_p() && launcher_ready_p()) READYTOFIRELED = LEDON;
	else READYTOFIRELED = LEDOFF;
	
	if(oldLimitBottom == SWITCHON){
		if(LAUNCHBOTTOMLIMIT == SWITCHOFF){
			clickRotationsLauncher = 0;
		}
	}
	oldLimitBottom = LAUNCHBOTTOMLIMIT;
	
	if(oldLimitTop == SWITCHON){
		if(LAUNCHTOPLIMIT == SWITCHOFF){
			clickRotationsLauncher = 142;
		}
	}
	oldLimitTop = LAUNCHTOPLIMIT;
	
}


void Catlin_Common_First(void) {

	//BUZZER = LEDOFF;

/////////////////////////////////////////// CAMERA
	// send diagnostic information to the terminal
	//Tracking_Info_Terminal();

	// This function is responsable for camera initialization 
	// and camera serial data interpretation. Once the camera
	// is initialized and starts sending tracking data, this 
	// function will continuously update the global T_Packet_Data 
	// structure with the received tracking information.
	Camera_Handler();
	Servo_Track();

	// This function reads data placed in the T_Packet_Data
	// structure by the Camera_Handler() function and if new
	// tracking data is available, attempts to keep the center
	// of the tracked object in the center of the camera's
	// image using two servos that drive a pan/tilt platform.
	// If the camera doesn't have the object within it's field 
	// of view, this function will execute a search algorithm 
	// in an attempt to find the object.

//////////////////////////////////////////////
}

void Catlin_Autonomous(void) {
	char mode = 0;
	if(AUTOBIT1 == SWITCHON) mode += 1;
	if(AUTOBIT2 == SWITCHON) mode += 2;
	if(AUTOBIT4 == SWITCHON) mode += 4;
	
	if(STARTPOSN1 == SWITCHON) {
		driveAndShootAuto(1);
		/*switch(mode) {
			case 0:
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				break;
		}*/
	}

	else if(STARTPOSN3 == SWITCHON) {
		driveAndShootAuto(3);
		/*switch(mode) {
			case 0:
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				break;
		}*/
	}

	else {
		driveAndShootAuto(2);
		/*switch(mode) {
			case 0:
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				break;
		}*/
	} 
}

void Catlin_Main_Routine(void) {
	
	//User_Byte1 = clickRotationsLauncher;
	//User_Byte2 = goalLauncherAngle % 255;
	//User_Byte3 = goalLauncherAngle / 255;

	if(CAMERARESET){
		Restart_Camera();
	}	
	
	if(hasZero == 0){
		//BUZZER = LEDOFF;
		//printf("CHECK 1 - HAS ZERO (124)\r");
		setLauncher(-5000);
		updateLauncher();
		if(LAUNCHBOTTOMLIMIT == SWITCHON){
			hasZero = 1;
			clickRotationsLauncher = 0;
			setLauncher(0);
		}
	}
	else {
		Catlin_Common_First();	// DO NOT REMOVE THIS LINE
		if(MODETOGGLE) {
			autoAim();
			updateLauncher();
		}
		else Catlin_Manual_Elevation();
	}
	
	if(LEARNHIGH)			learnHigh();
	else if(LEARNLOW)		learnLow();
	else if(LEARNCORRECT)	learnCorrect();
	
	
	


	if(FIRE1 == 0){
		Catlin_Basic_Drive();
	}else{
		LEFTMOTOR = 127;
		RIGHTMOTOR = 127;
	}
	
	if(MORELEFT > 200){
		Set_Catlin_Slight_Shift(LEFT);
	}
	if(MORERIGHT > 200){
		Set_Catlin_Slight_Shift(RIGHT);
	}
	Catlin_Slight_Shift();
	

		
	if(LAUNCHERTOGGLE) {
		LAUNCHER = FULLFORWARD;
	}
	else{
		LAUNCHER = NEUTRAL;
	}
	
	if(AUTOROTATE) rotating = YES;
		
	if(rotating && !ready_to_fire_p()) {
		if((XAXIS > 177) || (XAXIS < 77) || (YAXIS > 177) || (YAXIS < 77)) {
			rotating = NO;
				
		}else{
			camera_auto_rotate();
		}
	}
	else if(ready_to_fire_p()) rotating = NO;

	User_Byte1 = clickRotationsLauncher % 256;
	User_Byte2 = clickRotationsLauncher / 256;
	
	//User_Byte3 = TILT_SERVO;
	//User_Byte4 = LAUNCHBOTTOMLIMIT;

//	User_Byte1 = clickRotationsRight % 256;
//	User_Byte2 = clickRotationsRight / 256;
	
//	User_Byte3 = clickRotationsLeft % 256;
//	User_Byte4 = clickRotationsLeft / 256;

	User_Byte5 = ((goalLauncherAngle) % 256);
	User_Byte6 = ((goalLauncherAngle) / 256);
	
	if(LOADERTOGGLE && !LOADERBARF) LOADER = LOADERFORWARD;
	else if(LOADERBARF) LOADER = LOADERBACK;
	else LOADER = NEUTRAL;
	

	
	fireTest();
	//////////////////////////////////
	
	if(LOWGOALTOGGLE){
	 	zeroLauncher();
		updateLauncher();
	}	
	

	
	
	Catlin_Common_Last();
	
	if(p4_sw_top){
		if(waitTime > 15){		
			waitTime = 0;
//			User_Byte4 = 78;
			CatlinEEPROM_PRINT();
		}else{
			waitTime++;
		}
	}
	if(p4_sw_trig){
		if(waitTime > 15){
			waitTime = 0;
		}else{
			waitTime++;
		}
	}	

	//BUZZER = LEDOFF;
	
	User_Byte1 = hasZero;
}

void Catlin_Basic_Drive(void) {
	
	char dirTemp = 0;
	
	/* Begin Basic Single-Joystick Drive Code */
	/* NOTE: CONSTANTS MUST BE SET IN catlin_aliases.h FOR CODE TO WORK */
	
	if(TURBO) {
		leftInit = YAXIS;  
		rightInit = (255 - YAXIS);
	}
	else {
		leftInit = (.5 * YAXIS) + (.5 * (255 - (float) XAXIS));  
		rightInit = (255 - ((.5 * YAXIS) + (.5 * (float) XAXIS)));
	}

	if(leftInit>255) leftInit = 255;
	/*else pwm01 = pwm01Init; */
 	if(rightInit>255) rightInit = 255;
 	/*else pwm03 = pwm03Init; */

 	if((leftInit > TOPTHRESHOLD) || (leftInit < BOTTOMTHRESHOLD)) LEFTMOTOR = leftInit;
 	else LEFTMOTOR = 127;

	if((rightInit > TOPTHRESHOLD) || (rightInit < BOTTOMTHRESHOLD)) RIGHTMOTOR = rightInit;
	else RIGHTMOTOR = 127;
	/* End Basic Drive Code */
	
	if(DRIVEDIRECTION) {
		dirTemp = LEFTMOTOR;
		LEFTMOTOR = RIGHTMOTOR;
		RIGHTMOTOR = dirTemp;
	}
	else { /* SkipGleefully(); */ } 
}

void Catlin_Manual_Elevation(void) {
		if(MANUALELEVATION < 40) {
			if(LAUNCHBOTTOMLIMIT == SWITCHOFF) LAUNCHWINCH = 255;
			else LAUNCHWINCH = NEUTRAL;
			setLauncher(clickRotationsLauncher);
		}
		else if(MANUALELEVATION > 230) {
			if(LAUNCHTOPLIMIT == SWITCHOFF) LAUNCHWINCH = 0;
			else LAUNCHWINCH = NEUTRAL;
			setLauncher(clickRotationsLauncher);
		}
		else {
//			User_Byte1 = (MANUALELEVATION - 40) * .75;
//			User_Byte2 = (int)(TRIM/4)-(int)(63/4);
			setLauncher((MANUALELEVATION - 40)*(.75)- (int)(63/4) + ((TRIM)/4));
			updateLauncher();
		}
}

void Catlin_Slight_Shift(void) {
	
	if(timer == 0){
	
	}else if(direction == RIGHT) {
		LEFTMOTOR = RIGHTMOTOR = 127 + SLIGHTTURNSPEED;
		timer--;
	}else if(direction == LEFT){
		LEFTMOTOR = RIGHTMOTOR = 127 - SLIGHTTURNSPEED;
		timer--;
	}
	
}

void Set_Catlin_Slight_Shift(char shiftDirection){
	timer = 2;
	direction = shiftDirection;
}

void arcDrive(void){
	
	char tempY;
	if(YAXIS > 205) tempY = 205;
	else if (YAXIS < 50) tempY = 50;
	else tempY = YAXIS;
	
	camera_auto_rotate();	
	if(tempY >= 137) {
		LEFTMOTOR = LEFTMOTOR + (tempY/2) - (127/2); 
		RIGHTMOTOR = RIGHTMOTOR - (tempY/2) + (127/2);
	}
	if(tempY < 117){
		LEFTMOTOR = LEFTMOTOR + tempY/2 - 127; 
		RIGHTMOTOR = RIGHTMOTOR - tempY/2 + 127;
	}
}

