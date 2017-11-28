/*******************************************************************************
*
*	TITLE:		tracking.h 
*
*	VERSION:	0.1 (Beta)                           
*
*	DATE:		1-Jan-2006
*
*	AUTHOR:		R. Kevin Watson
*				kevinw@jpl.nasa.gov
*
*	COMMENTS:	This is the "streamlined" version of tracking.h
*
*				You are free to use this source code for any non-commercial
*				use. Please do not make copies of this source code, modified
*				or un-modified, publicly available on the internet or elsewhere
*				without permission. Thanks.
*
*				Copyright �2005-2006 R. Kevin Watson. All rights are reserved.
*
********************************************************************************
*
*	CHANGE LOG:
*
*	DATE         REV  DESCRIPTION
*	-----------  ---  ----------------------------------------------------------
*	01-Jan-2006  0.1  RKW - Original code.
*
*******************************************************************************/
#ifndef _TRACKING_H
#define _TRACKING_H

#include "catlin_aliases.h"

// By default, PWM output one is used for the pan servo.
// Change it to another value if you'd like to use PWM
// output one for another purpose.
//#define PAN_SERVO pwm07

// By default, PWM output two is used for the tilt servo.
// Change it to another value if you'd like to use PWM
// output two for another purpose.
//#define TILT_SERVO pwm08

// This value defines how many "slow loops" to wait before
// sending the tracking servo(s) to their next destination
// while in search mode. This provides a small delay for the 
// camera to lock onto the target between position changes.
#define SEARCH_DELAY_DEFAULT 0


// so we don't start searching when a robot passes in front
// of the camera, when the camera is still oriented correctly
#define PASSERBY_DELAY_DEFAULT 24
#define MINIMUM_TARGET_TIME 6
#define THETA_ADDITION .0327
#define TILT_TRACK_VALUE 180

// These values define how quickly the camera will attempt 
// to track the object. If these are set too high, the camera 
// will take longer to settle, too low and the camera will
// overshoot the target and oscillate.
#define PAN_GAIN_DEFAULT 5
#define TILT_GAIN_DEFAULT 6

// If your camera suddenly moves away from the target once
// it finds it, you'll need to change the sign on one or
// both of these values. Start with the tilt first.
#define PAN_ROTATION_SIGN_DEFAULT -1
#define TILT_ROTATION_SIGN_DEFAULT 1

// These two values define the image pixel that we're
// going to try to keep the tracked object on. By default
// the center of the image is used.
#define PAN_TARGET_PIXEL_DEFAULT 79
#define TILT_TARGET_PIXEL_DEFAULT 119

// These values define how much error, in pixels, is 
// allowable when trying to keep the center of the tracked
// object on the center pixel of the camera's imager. Too 
// high a value and your pointing accuracy will suffer, too
// low and your camera may oscillate because the servos 
// don't have enough pointing resolution to get the center 
// of the tracked object into the square/rectangle defined 
// by these values 
#define PAN_ALLOWABLE_ERROR_DEFAULT 4
#define TILT_ALLOWABLE_ERROR_DEFAULT 4
#define PAN_ALLOWABLE_ERROR_AUTO 6
#define TILT_ALLOWABLE_ERROR_AUTO 6

// These values define the lower and upper bound on the
// pan servo travel
#define PAN_MIN_PWM_DEFAULT 0
#define PAN_MAX_PWM_DEFAULT 255

// This value defines how far the pan servo will step
// each time while in search mode. This value was chosen
// to provide five evenly spaced stopping points (i.e., 0,
// 62, 124, 186 and 248) for the pan servo.
#define PAN_SEARCH_STEP_SIZE_DEFAULT 30

// These values define the PWM values of the pan and tilt
// servos when the camera is pointing directly ahead
// (i.e., when pan and tilt angles are zero).
#define PAN_CENTER_PWM_DEFAULT 115
#define TILT_CENTER_PWM_DEFAULT 173

// These values define the lower and upper bound on the
// tilt servo travel
#define TILT_MIN_PWM_DEFAULT 107
#define TILT_MAX_PWM_DEFAULT 240

// This value defines how far the tilt servo will step
// each time while in search mode. This value was chosen
// to provide three evenly spaced stopping points (i.e.,
// 94, 144 and 194) for the tilt servo.
#define TILT_SEARCH_STEP_SIZE_DEFAULT 15

// parameters for CMUcam2 with OV7620 camera module
#define IMAGE_WIDTH 159
#define IMAGE_HEIGHT 239

// function prototypes
static unsigned int passerby_delay_default = PASSERBY_DELAY_DEFAULT;
void Servo_Track(void);
void set_passerby_delay(unsigned int delay);
/*
void turret_track(void);
*/
#endif
