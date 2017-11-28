/*******************************************************************************
*
*	TITLE:		tracking.c 
*
*	VERSION:	0.1 (Beta)                           
*
*	DATE:		1-Jan-2006
*
*	AUTHOR:		R. Kevin Watson
*				kevinw@jpl.nasa.gov
*
*	COMMENTS:	This is the "streamlined" version of tracking.c
*
*				You are free to use this source code for any non-commercial
*				use. Please do not make copies of this source code, modified
*				or un-modified, publicly available on the internet or elsewhere
*				without permission. Thanks.
*
*				Copyright ©2005-2006 R. Kevin Watson. All rights are reserved.
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
#include <stdio.h>
#include <math.h>
#include "ifi_default.h"
#include "ifi_aliases.h"
#include "camera.h"
#include "tracking.h"

/*******************************************************************************
*
*	FUNCTION:		Servo_Track()
*
*	PURPOSE:		This function reads data placed in the T_Packet_Data
*					structure by the Camera_Handler() function and if new
*					tracking data is available, attempts to keep the center
*					of the tracked object in the center of the camera's
*					image using two servos that drive a pan/tilt platform.
*					If the camera doesn't have the object within it's field 
*					of view, this function will execute a search algorithm 
*					in an attempt to find the object.		
*
*	CALLED FROM:	user_routines.c/Process_Data_From_Master_uP()
*
*	PARAMETERS:		None.
*
*	RETURNS:		Nothing.
*
*	COMMENTS:		This version of the tracking code uses a proportional
*					feedback controller to track the object.
*
*******************************************************************************/
void Servo_Track(void)
{
	static unsigned int old_camera_t_packets = 0;
	static unsigned char new_search = 1;
	static unsigned char loop_count = 0;
	static unsigned char passerby_delay = 0;
	static unsigned float theta = 0;
    static unsigned int time_targeted = 0;
  
    float sin3t = 0;
    float sin4t = 0;

	int temp_pan_servo;
	int temp_tilt_servo;
	int servo_step;
	int pan_error;
	int tilt_error;
/*
    if (get_current_camera_num() != CAMERA_ONE)
      set_current_camera(CAMERA_ONE);
*/
	// Has a new camera t-packet arrived since we last checked?
	if(camera_t_packets != old_camera_t_packets)
	{
		old_camera_t_packets = camera_t_packets;

		// Does the camera have a tracking solution? If so,
		// do we need to move the servos to keep the center
		// of the tracked object centered within the image?
		// If not, we need to drop down below to start or
		// continue a search
		if(T_Packet_Data.my != 0)
		{
			// if we're tracking, reset the search
			// algorithm so that a new search pattern
			// will start should we lose tracking lock
            new_search = 1;
            passerby_delay = 0;
            time_targeted++;

			////////////////////////////////
			//                            //
			//	x-axis/pan tracking code  //
			//                            //
			////////////////////////////////


			// save the current pan servo PWM value into a local
			// integer variable so that we can detect and correct 
			// underflow and overflow conditions before we update 
			// the pan servo PWM value with a new value
			temp_pan_servo = (int)PAN_SERVO;

			// calculate how many image pixels we're away from the
			// vertical center line.
			pan_error = (int)T_Packet_Data.mx - PAN_TARGET_PIXEL_DEFAULT;

			// Are we too far to the left or right of the vertical 
			// center line? If so, calculate how far we should step
			// the pan servo to reduce the error.
			if(pan_error > PAN_ALLOWABLE_ERROR_DEFAULT)
			{
				// calculate how far we need to step the pan servo
				servo_step = pan_error / PAN_GAIN_DEFAULT;

				// Due to rounding error in the division calculation above,
				// the step may be calculated as zero, which will make it
				// impossible to converge on the target when x_error is
				// smaller than X_GAIN. To get around this problem, we just 
				// test for the zero case and set the step size to one. 
				if(servo_step == 0)
				{
					servo_step = 1;
				}
			}
			else if(pan_error < -1 * PAN_ALLOWABLE_ERROR_DEFAULT)
			{
				// calculate how far we need to step the pan servo
				servo_step = pan_error / PAN_GAIN_DEFAULT;

				// Due to rounding error in the division calculation above,
				// the step may be calculated as zero, which will make it
				// impossible to converge on the target when x_error is
				// smaller than X_GAIN. To get around this problem, we just 
				// test for the zero case and set the step size to one. 
				if(servo_step == 0)
				{
					servo_step = -1;
				}
			}
			else
			{
				// if we've fallen through to here, it means that we're
				// neither too far to the left or too far to the right
				// of the vertical center line of the image and don't 
				// need to move the servo
				servo_step = 0;
			}

			// add the step to the current servo position, taking into
			// account the direction set by the user in tracking.h
			temp_pan_servo += PAN_ROTATION_SIGN_DEFAULT * servo_step;

			// check the pan servo PWM value for under/overflow
			if(temp_pan_servo < PAN_MIN_PWM_DEFAULT)
			{
				temp_pan_servo = PAN_MIN_PWM_DEFAULT;
			}
			else if(temp_pan_servo > PAN_MAX_PWM_DEFAULT)
			{
				temp_pan_servo = PAN_MAX_PWM_DEFAULT;
			}

			// update pan servo PWM value
			PAN_SERVO = (unsigned char)temp_pan_servo;


			/////////////////////////////////
			//                             //
			//	y-axis/tilt tracking code  //
			//                             //
			/////////////////////////////////

			// save the current tilt servo PWM value into a local
			// integer variable so that we can detect and correct 
			// underflow and overflow conditions before we update 
			// the tilt servo PWM value with a new value
			temp_tilt_servo = (int)TILT_SERVO;

			// calculate how many image pixels we're away from the
			// horizontal center line.
			tilt_error = (int)T_Packet_Data.my - TILT_TARGET_PIXEL_DEFAULT;

			// Are we too far above or below the horizontal center line?
			// If so, calculate how far we should step the tilt servo to 
			// reduce the error.
			if(tilt_error > TILT_ALLOWABLE_ERROR_DEFAULT)
			{
				// calculate how far we need to step the tilt servo
				servo_step = tilt_error / TILT_GAIN_DEFAULT;

				// Due to rounding error in the division calculation above,
				// the step may be calculated as zero, which will make it
				// impossible to converge on the target when y_error is
				// smaller than Y_GAIN. To get around this problem, we just 
				// test for the zero case and set the step size to one. 
				if(servo_step == 0)
				{
					servo_step = 1;
				}
			}
			else if (tilt_error < -1 * TILT_ALLOWABLE_ERROR_DEFAULT)
			{
				// calculate how far we need to step the tilt servo
				servo_step = tilt_error / TILT_GAIN_DEFAULT;

				// Due to rounding error in the division calculation above,
				// the step may be calculated as zero, which will make it
				// impossible to converge on the target when x_error is
				// smaller than X_GAIN. To get around this problem, we just 
				// test for the zero case and set the step size to one. 
				if(servo_step == 0)
				{
					servo_step = -1;
				}
			}
			else
			{
				// if we've fallen through to here, it means that we're
				// neither too far below or to far above the horizontal
				// center line of the image and don't need to move the
				// servo
				servo_step = 0;
			}

			// add the step to the current servo position, taking into
			// account the direction set by the user in tracking.h
			temp_tilt_servo += TILT_ROTATION_SIGN_DEFAULT * servo_step;

			// check the tilt PWM value for under/overflow
			if(temp_tilt_servo < TILT_MIN_PWM_DEFAULT)
			{
				temp_tilt_servo = TILT_MIN_PWM_DEFAULT;
			}
			else if(temp_tilt_servo > TILT_MAX_PWM_DEFAULT)
			{
				temp_tilt_servo = TILT_MAX_PWM_DEFAULT;
			}

			// update tilt servo PWM value
			TILT_SERVO = (unsigned char)temp_tilt_servo;
		}
		else
		{
			///////////////////
			//               //
			//  search code  //
			//               //
			///////////////////

			// To provide a delay for the camera to lock onto the
			// target between position changes, we only step the camera
			// to a new position every SEARCH_DELAY times while we're 
			// in search mode. SEARCH_DELAY is #define'd in tracking.h
			loop_count++;

			//to prevent the camera from searching when a robot has
			//momentarily passed in front of the camera (while the
			//camera is still oriented correctly), put a delay
			//before we start a new search
            if (time_targeted < MINIMUM_TARGET_TIME)
                passerby_delay = passerby_delay_default;
            if ((new_search == 1) && (passerby_delay < passerby_delay_default))
			{
                passerby_delay++;
                /*
                theta = theta + THETA_ADDITION;
                temp_pan_servo = (int)PAN_SERVO;
                temp_tilt_servo = (int)TILT_SERVO;
                temp_pan_servo = temp_pan_servo + cos(6*theta);
                temp_tilt_servo = temp_tilt_servo + sin(6*theta);
                PAN_SERVO = (unsigned char)temp_pan_servo;
                TILT_SERVO = (unsigned char)temp_tilt_servo;
                */
			}
			else if(loop_count > SEARCH_DELAY_DEFAULT)
			{
				// reset the loop counter
				loop_count = 0;
				passerby_delay = 0;
                time_targeted = 0;

				// If we're starting a new search, initialize the pan
				// and tilt servos to the search starting point.
				// Otherwise, just continue the search pattern from
				// where we left off. The variable new_search is reset
				// to one each time the tracking code (above) executes.
				if(new_search == 1)
				{
					new_search = 0;
					temp_pan_servo = (int)PAN_SERVO;
                    temp_tilt_servo = (int)TILT_SERVO;
                    theta = asin((temp_pan_servo - PAN_CENTER_PWM_DEFAULT)/(PAN_CENTER_PWM_DEFAULT - PAN_MIN_PWM_DEFAULT))/4.0f;
                    //if (temp_pan_servo < 127)
                      //theta = theta + 3.14159f;
				}
				else
				{
					// calculate new servo position(s) based upon our
					// current servo position(s)
					temp_pan_servo = (int)PAN_SERVO;
					temp_tilt_servo = (int)TILT_SERVO;
                    
                    sin3t = sin(3*theta);
                    sin4t = sin(4*theta);

                    temp_tilt_servo = TILT_TRACK_VALUE;
                    temp_pan_servo = (int)((PAN_CENTER_PWM_DEFAULT - PAN_MIN_PWM_DEFAULT)*sin4t+PAN_CENTER_PWM_DEFAULT);
                    /*
                    if (sin3t < 0)
                    {
                      temp_pan_servo = (int)((PAN_CENTER_PWM_DEFAULT - PAN_MIN_PWM_DEFAULT)*sin3t+PAN_CENTER_PWM_DEFAULT);
                    }
                    else
                    {
                      temp_pan_servo = (int)((PAN_MAX_PWM_DEFAULT - PAN_CENTER_PWM_DEFAULT)*sin3t+PAN_CENTER_PWM_DEFAULT);
                    }

                    if (sin4t < 0)
                    {
                      temp_tilt_servo = (int)((TILT_CENTER_PWM_DEFAULT - TILT_MIN_PWM_DEFAULT)*sin4t+TILT_CENTER_PWM_DEFAULT);
                    }
                    else
                    {
                      temp_tilt_servo = (int)((TILT_MAX_PWM_DEFAULT - TILT_CENTER_PWM_DEFAULT)*sin4t+TILT_CENTER_PWM_DEFAULT);
                    }
                    */

                    //temp_pan_servo = (int)((((PAN_MAX_PWM_DEFAULT - PAN_MIN_PWM_DEFAULT)/2)*(sinv))+PAN_CENTER_PWM_DEFAULT);
                    //temp_tilt_servo = (int)((((TILT_MAX_PWM_DEFAULT - TILT_MIN_PWM_DEFAULT)/2)*(sin(4*theta)))+TILT_CENTER_PWM_DEFAULT);
					//printf("%d %d\r\n", temp_pan_servo, temp_tilt_servo);
					theta = theta + THETA_ADDITION;
				}
				// update the pan and tilt servo PWM value
				PAN_SERVO = (unsigned char)temp_pan_servo;
				TILT_SERVO = (unsigned char)temp_tilt_servo;
			}
		}
	}
	//printf("pan: %d tilt: %d\r", PAN_SERVO, TILT_SERVO);
}

void set_passerby_delay(unsigned int delay)
{
  passerby_delay_default=delay;
}

/*
void turret_track(void)
{
  static unsigned int old_camera_2_t_packets = 0;
  int pan_error;
  int tilt_error;

  if (get_current_camera_num() != CAMERA_TWO)
    set_current_camera(CAMERA_TWO);

  if(camera_t_packets != old_camera_2_t_packets)
  {
    old_camera_2_t_packets = camera_t_packets;

    if(T_Packet_Data.my != 0)
    {
      //the camera is on its side, so we have greater vertical accuracy
      //so tilt and pan are switched
      tilt_error = (int)T_Packet_Data.mx - PAN_TARGET_PIXEL_DEFAULT;
      pan_error = (int)T_Packet_Data.my - TILT_TARGET_PIXEL_DEFAULT;
      if (tilt_error > (3 * PAN_ALLOWABLE_ERROR_DEFAULT))
      {
        //turret_tilt_pwm = 180; //tell the turret to tilt up
		//printf("tilt turret up\r\n");
      }
      else if (tilt_error < (-1 * 3 * PAN_ALLOWABLE_ERROR_DEFAULT))
      {
        //turret_tilt_pwm = 74; //tell the turret to tilt down
		//printf("tilt turret down\r\n");
      }
      else
      {
        //turret_tilt_pwm = 0; //tell the turret to stop tilting
		//printf("stop tilting turret\r\n");
      }
      if (pan_error > (3 * TILT_ALLOWABLE_ERROR_DEFAULT))
      {
        //turret_pan_pwm = 180; //tell the turret to pan right
		//printf("pan turret right\r\n");
      }
      else if (pan_error < (-1 * 3 * TILT_ALLOWABLE_ERROR_DEFAULT))
      {
        //turret_pan_pwm = 74; //tell the turret to pan left
		//printf("pan turret left\r\n");
      }
      else
      {
        //turret_pan_pwm = 0; //tell the turret to stop panning
		//printf("stop panning turret\r\n");
      }
    }
  } 
}
*/
