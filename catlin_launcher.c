//TOM
#include "interrupts.h"
#include "catlin_launcher.h"
#include "ifi_aliases.h"
#include "ifi_default.h"
#include "eeprom.h"
#include <stdio.h>
#include "catlin_aliases.h"
#include "catlin_fire.h"

long int goalLauncherAngle;

void launcherInit(void){
	int i;
	goalLauncherAngle = 0;
	
//ask tom to uncomment if weird stuff happens w/ table
/*
CatlinEEPROM_WRITE(148,116); 
CatlinEEPROM_WRITE(149,116); 
CatlinEEPROM_WRITE(150,116); 
CatlinEEPROM_WRITE(151,117); 
CatlinEEPROM_WRITE(152,118); 
CatlinEEPROM_WRITE(153,118); 
CatlinEEPROM_WRITE(154,118); 
CatlinEEPROM_WRITE(155,118); 
CatlinEEPROM_WRITE(156,118); 
CatlinEEPROM_WRITE(157,117); 
CatlinEEPROM_WRITE(158,116); 
CatlinEEPROM_WRITE(159,115); 
CatlinEEPROM_WRITE(160,114); 
CatlinEEPROM_WRITE(161,115); 
CatlinEEPROM_WRITE(162,115); 
CatlinEEPROM_WRITE(163,116); 
CatlinEEPROM_WRITE(164,116); 
CatlinEEPROM_WRITE(165,117); 
CatlinEEPROM_WRITE(166,117); 
CatlinEEPROM_WRITE(167,117); 
CatlinEEPROM_WRITE(168,115); 
CatlinEEPROM_WRITE(169,123); 
CatlinEEPROM_WRITE(170,132); 
CatlinEEPROM_WRITE(171,133); 
CatlinEEPROM_WRITE(172,133); 
CatlinEEPROM_WRITE(173,134); 
CatlinEEPROM_WRITE(174,134); 
CatlinEEPROM_WRITE(175,134); 
CatlinEEPROM_WRITE(176,134); 
CatlinEEPROM_WRITE(177,134); 
CatlinEEPROM_WRITE(178,134); 
CatlinEEPROM_WRITE(179,133); 
CatlinEEPROM_WRITE(180,133); 
Putdata(&txdata);
CatlinEEPROM_WRITE(181,133); 
CatlinEEPROM_WRITE(182,132); 
CatlinEEPROM_WRITE(183,132); 
CatlinEEPROM_WRITE(184,129); 
CatlinEEPROM_WRITE(185,122); 
CatlinEEPROM_WRITE(186,117); 
CatlinEEPROM_WRITE(187,120); 
CatlinEEPROM_WRITE(188,122); 
CatlinEEPROM_WRITE(189,124); 
CatlinEEPROM_WRITE(190,135); 
CatlinEEPROM_WRITE(191,130); 
CatlinEEPROM_WRITE(192,132); 
CatlinEEPROM_WRITE(193,133); 
CatlinEEPROM_WRITE(194,134); 
CatlinEEPROM_WRITE(195,135); 
CatlinEEPROM_WRITE(196,133); 
CatlinEEPROM_WRITE(197,133); 
CatlinEEPROM_WRITE(198,133); 
CatlinEEPROM_WRITE(199,133); 
CatlinEEPROM_WRITE(200,133); 
CatlinEEPROM_WRITE(201,133); 
CatlinEEPROM_WRITE(202,133); 
CatlinEEPROM_WRITE(203,133); 
CatlinEEPROM_WRITE(204,133); 
CatlinEEPROM_WRITE(205,132); 
CatlinEEPROM_WRITE(206,131); 
CatlinEEPROM_WRITE(207,130); 
CatlinEEPROM_WRITE(208,126); 
CatlinEEPROM_WRITE(209,124); 
CatlinEEPROM_WRITE(210,124); 
CatlinEEPROM_WRITE(211,123); 
CatlinEEPROM_WRITE(212,122); 
CatlinEEPROM_WRITE(213,122); 
*/

	
}

void updateLauncher(void){	
	int currentLauncherAngle = clickRotationsLauncher;
	//User_Byte3 = (goalLauncherAngle) % 256;
	//User_Byte4 = (goalLauncherAngle) / 256;	
		if(currentLauncherAngle > goalLauncherAngle - 2 && currentLauncherAngle < goalLauncherAngle + 2){
			LAUNCHWINCH = NEUTRAL;
		}else if(currentLauncherAngle < goalLauncherAngle - 20){
			LAUNCHWINCH = 55;//fast
		}else if(currentLauncherAngle > goalLauncherAngle + 20){
			LAUNCHWINCH = 200;//fast
		}else if(currentLauncherAngle <= goalLauncherAngle - 2){
			LAUNCHWINCH = 99;//slow
		}else if(currentLauncherAngle >= goalLauncherAngle + 2){
			LAUNCHWINCH = 155;//slow
		}	
	if(LAUNCHBOTTOMLIMIT == SWITCHON){
		if(LAUNCHWINCH > 127){
			LAUNCHWINCH = 127;
		}
	}
	if(LAUNCHTOPLIMIT == SWITCHON){
		if(LAUNCHWINCH < 127){
			LAUNCHWINCH = 127;
		}
	}
}


//clears eeprom
void CatlinEEPROM_CLEAR(){
	int i;
	for(i = 107; i < 270; i++){
		CatlinEEPROM_WRITE(i,0);
	}
}
//each reading is two bytes
int CatlinEEPROM_READ(int i){
		return EEPROM_Read(i - 107);
}

//writes the 2 byte data d to location i in EEPROM
void CatlinEEPROM_WRITE(int i, int d){
	EEPROM_Write((i - 107), d);	
	EEPROM_Write_Handler();
	
}
void CatlinEEPROM_PRINT(void){
	
	int i;
	//printf("PRINTINGINGINGINGNING");
	for(i = 107; i <270; i++){
		int n;
		for(n = 0; n < 200; n++){
			int l;
			l = n*2+5; 
			l = l*9;			

		}
		printf("CatlinEEPROM_WRITE(%d,", i);
		printf("%d); \r",CatlinEEPROM_READ(i));
		if(i == 180){
			Putdata(&txdata);
		}
		
	}
}


//given distance d from the target it reads the lookup table
//and will auto aimt he vertical angle
void autoAim(){
	double d = TILT_SERVO;//distance
	setLauncher(CatlinEEPROM_READ(d)-(int)(63/4) + (TRIM)/4);	
}

//called after a correct shot
void learnLow(){
User_Byte1 = 55;//change
	haveFired = TRUE;//change
	if(haveFired == TRUE){
		int cameraServo = TILT_SERVO;//servo values
		double placement = clickRotationsLauncher;//angle
		double oldValue;
		double newValueOne;
		double newValueTwo;
		
		int i;
		placement = placement + LEARNCHANGE;
		
		for(i = -6; i < 7; i++){
								//where you are writing too   //read difference between actual and theory and scale that then add that change to the orignal
			oldValue = CatlinEEPROM_READ(cameraServo+i);
			newValueOne = .75 - ((double)abs(i)/10.0);		
			newValueTwo = newValueOne*(placement-oldValue);
			printf("Writing too: %d \r", (int)(cameraServo+i));
			printf("Writing: %d \r",(int)(oldValue + newValueTwo));
			printf("New value one: %d \r", (int) (newValueOne*10));
			printf("New value two: %d \r", (int) (placement-oldValue));
			printf("Actual Value: %d \r\r", CatlinEEPROM_READ(cameraServo+i));
			CatlinEEPROM_WRITE(cameraServo+i, oldValue + newValueTwo);			
		}
		haveFired = FALSE;
	}


}

//called after a correct shot
void learnHigh(){
User_Byte1 = 55;//change
	haveFired = TRUE;//change
	if(haveFired == TRUE){
		int cameraServo = TILT_SERVO;//servo values
		double placement = clickRotationsLauncher;//angle
		double oldValue;
		double newValueOne;
		double newValueTwo;
		
		int i;
		
		placement = placement - LEARNCHANGE;
		
		for(i = -6; i < 7; i++){
								//where you are writing too   //read difference between actual and theory and scale that then add that change to the orignal
			oldValue = CatlinEEPROM_READ(cameraServo+i);
			newValueOne = 1 - ((double)abs(i)/10.0);		
			newValueTwo = newValueOne*(placement-oldValue);
			printf("Writing too: %d \r", (int)(cameraServo+i));
			printf("Writing: %d \r",(int)(oldValue + newValueTwo));
			printf("New value one: %d \r", (int) (newValueOne*10));
			printf("New value two: %d \r", (int) (placement-oldValue));
			printf("Actual Value: %d \r\r", CatlinEEPROM_READ(cameraServo+i));
			CatlinEEPROM_WRITE(cameraServo+i, oldValue + newValueTwo);			
		}
		haveFired = FALSE;
	}

}

//called after a correct shot
void learnCorrect(){
	User_Byte1 = 55;//change
	haveFired = TRUE;//change
	if(haveFired == TRUE){
		int cameraServo = TILT_SERVO;//servo values
		double placement = clickRotationsLauncher;//angle
		double oldValue;
		double newValueOne;
		double newValueTwo;
		
		int i;
		
		for(i = -6; i < 7; i++){
								//where you are writing too   //read difference between actual and theory and scale that then add that change to the orignal
			oldValue = CatlinEEPROM_READ(cameraServo+i);
			newValueOne = .75 - ((double)abs(i)/10.0);		
			newValueTwo = newValueOne*(placement-oldValue);
			printf("Writing too: %d \r", (int)(cameraServo+i));
			printf("Writing: %d \r",(int)(oldValue + newValueTwo));
			printf("New value one: %d \r", (int) (newValueOne*10));
			printf("New value two: %d \r", (int) (placement-oldValue));
			printf("Actual Value: %d \r\r", CatlinEEPROM_READ(cameraServo+i));
			CatlinEEPROM_WRITE(cameraServo+i, oldValue + newValueTwo);			
		}
		haveFired = FALSE;
	}

}

double abs(double d){
	if(d < 0){
		return 0-d;
	}else{
		return d;
	}
}
void setLauncher(int desiredAngle){
	
	if(LAUNCHTOPLIMIT == SWITCHON){
		if(desiredAngle >= clickRotationsLauncher){
			goalLauncherAngle = clickRotationsLauncher;			
		}else{
			goalLauncherAngle = desiredAngle;
		}
	}else if(LAUNCHBOTTOMLIMIT == SWITCHON){
		if(desiredAngle <= clickRotationsLauncher){
			goalLauncherAngle = clickRotationsLauncher;				
		}else{
			goalLauncherAngle = desiredAngle;
		}
	}else{
		goalLauncherAngle = desiredAngle;
	}
}

void zeroLauncher(){
	goalLauncherAngle = 0;
	if(LAUNCHERTOGGLE){
		LAUNCHER = SLOWSPEED;
	}
}
char launcher_ready_p(void){
	if(clickRotationsLauncher > goalLauncherAngle - 2 && clickRotationsLauncher < goalLauncherAngle + 2){
		return YES;
	}else{
		return NO;
	}
}
