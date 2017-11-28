/*******************************************************************************
* FILE NAME: catlin_aliases.h
*
* DESCRIPTION:
*  This file contains the catlin alias declarations.
*
* USAGE:
*	NO EDITING SHOULD HAPPEN TO THIS FILE WITHOUT THE
*	CONSENT OF THE SOFTWARE MANAGER OR MENTOR
*
*******************************************************************************/

#ifndef __catlin_aliases_h_
#define __catlin_aliases_h_

/* Constant Declarations */
#define RIGHTMOTOR			pwm01		/* Change this to your left motor's pwm */
#define LEFTMOTOR			pwm02		/* Change this to your right motor's pwm */
#define XAXIS				p4_x		/* Change this to your left joystick's x-axis */
#define YAXIS				p4_y		/* Change this to your right joystick's y-axis */

#define TOPTHRESHOLD		137		/* Change this to your top threshold (typically 10 above 127) */
#define BOTTOMTHRESHOLD		117		/* Change this to your bottom threshold (typically 10 below 127) */

#define	LAUNCHWINCH			pwm04		/* Change this to the Winch's Motor */

#define	LAUNCHER			pwm06		/* Change this to the Launcher's Motor */

#define	LOADER				pwm07		/* Change this to the Loader's Motor */
#define	LOADERFORWARD		255		/* Change this to the Loader's normal speed */
#define LOADERBACK			0		/* Change this to the Loader's reverse speed (for regurgitation) */

#define	ONDECKSERVO			pwm09		/* Change this to the On-Deck Servo's pwm */
#define	LAUNCHSERVO			pwm10		/* Change this to the Launcher Servo's pwm */

#define PAN_SERVO			pwm12		/* Camera Servo X-axis */
#define TILT_SERVO			pwm11		/* Camera Servo Y-axis */


/* These are basics that should always have values */
#ifndef FALSE
#define FALSE				0
#endif
#ifndef TRUE
#define TRUE				1
#endif
#define YES					1
#define NO					0
#define	MAX					255
#define	MIN					0
#define	NEUTRAL				127
#define	FULLFORWARD			255
#define	FULLBACK			0
#define STOPPED				127
#define ON					1
#define OFF					0
#define	LEDON				0
#define LEDOFF				1
#define	SWITCHOFF			1
#define	SWITCHON			0
#define	LAUNCHLOWSPEED		180
#define CALLSPSEC			38.17
#define	RIGHT				1
#define	LEFT				-1
#define SLIGHTTURNSPEED		32



/* CONSTANTS REPRESENTING DIGITAL INPUTS (Encoders, Optical Sensors, etc.) */
#define	LAUNCHENC1			rc_dig_in04		/* Interupt Encoder Input for the Launcher */
#define	RIGHTENC1			rc_dig_in01		/* Interupt Encoder Input for the Right Drive */
#define	LEFTENC1			rc_dig_in02		/* Interupt Encoder Input for the Left Drive */

#define LAUNCHBOTTOMLIMIT	rc_dig_in05		/* Launcher Bottom Limit Switch */
#define	LAUNCHTOPLIMIT		rc_dig_in06		/* Launcher Top Limit Switch */

#define LAUNCHENC2			rc_dig_in07		/* Launcher Encoder (non-interupt) */
#define RIGHTENC2			rc_dig_in08		/* Right Drive Encoder (non-interupt) */
#define LEFTENC2			rc_dig_in09		/* Left Drive Encoder (non-interupt) */

#define	BALLREADY			rc_dig_in10		/* Optical Sensor for 'Ball Ready' Position */
//#define	RACEWAYFULL			rc_dig_in11		/* Optical Sensor for when the Raceway is Full */

#define STARTPOSN1			rc_dig_in12		/* Autonomous Box, Start Posn Selector, Posn 1 */
#define STARTPOSN3			rc_dig_in13		/* Autonomous Box, Start Posn Selector, Posn 3 */
#define AUTOBIT1			rc_dig_in14		/* Autonomous Box, Autonomous Selector, bit 1 */
#define AUTOBIT2			rc_dig_in15		/* Autonomous Box, Autonomous Selector, bit 2 */
#define	AUTOBIT4			rc_dig_in16		/* Autonomous Box, Autonomous Selector, bit 4 */



/* CONSTANTS REPRESENTING OI DEVICES (LEDs, Buzzers, Joysticks, etc.) */
/*TOGGLES*/
#define	MODETOGGLE			p1_sw_aux2
#define	LAUNCHERTOGGLE		p1_sw_aux1
#define	LOADERTOGGLE		p1_sw_top
#define	LOWGOALTOGGLE		p1_sw_trig
#define	TRIM				p1_x
#define BIAS				p1_x
#define	MANUALELEVATION		p1_y
#define	DRIVEDIRECTION		p3_sw_aux2
#define CAMERARESEET		p3_sw_trig
/*Buttons*/
#define	AUTOROTATE			p2_sw_trig
#define	LEARNHIGH			p2_sw_top
#define	LEARNCORRECT		p2_sw_aux1
#define	LEARNLOW			p2_sw_aux2
#define FIRE1				p3_sw_aux1
#define	FIRE2				FALSE /*p4_sw_trig*/
#define	LOADERBARF			p3_sw_top
#define	CAMERARESET			p3_sw_trig

#define	TURBO				p4_sw_trig

#define	MORELEFT			p1_wheel
#define MORERIGHT			p1_aux
/*LEDs*/
#define	READYTOFIRELED		Pwm1_green
#define	BALLREADYLED		Pwm1_red
#define	CAMERALOCKLED		Pwm2_green
/*Other*/
#define	BUZZER				Pwm2_red


#endif
