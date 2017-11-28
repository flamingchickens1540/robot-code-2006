#include "ifi_aliases.h"
#include "ifi_default.h"
#include "ifi_utilities.h"
#include "user_routines.h"
#include "serial_ports.h"
#include "interrupts.h"
#include "camera_autonomous_d.h"
#include "camera.h"
#include "tracking.h"

int ready_to_fire_p()
{
  int pan_error;
  pan_error=(int)PAN_SERVO - (int)PAN_CENTER_PWM_DEFAULT;
  if (pan_error < 0)
  {
    pan_error = (-1 * pan_error);
  }
 
  if (pan_error < PAN_CENTER_ERROR && light_centered_p() == YES)
  {
    return YES;
  }
  else
  {
    return NO;	  
  }
}

int calculateAdditionLeft(int pixelDistance)
{
  double adder=14 + 14*(1.0-(battery_voltage/13.8));
  return (int)((pixelDistance/(4.0)) + adder);
}

int calculateAdditionRight(int pixelDistance)
{
  double adder=22 + 22*((1.0-battery_voltage/13.8));
  return (int)((pixelDistance/(4.0)) + adder);
}

void camera_auto_rotate()
{
  int pan_error;
  int t_pan_error; //based on T packet from camera
  int new_packets_p;

  set_passerby_delay(3);
  new_packets_p = Camera_Handler();
  Servo_Track();

  if (new_packets_p == 0)
  {
    if(T_Packet_Data.mx != 0)
    {
      pan_error=(int)(PAN_SERVO - PAN_CENTER_PWM_DEFAULT);
      t_pan_error = (int)T_Packet_Data.mx - PAN_TARGET_PIXEL_DEFAULT;
      if (t_pan_error < 0)
      {
        t_pan_error = (-1 * t_pan_error);
      }
  
      //this turns left
      if (PAN_SERVO < PAN_CENTER_PWM_DEFAULT)
      {
        if (t_pan_error < PAN_ALLOWABLE_ERROR_DEFAULT)
        {
          if ((PAN_CENTER_PWM_DEFAULT - PAN_SERVO) > PAN_CENTER_ERROR)
          {
            LEFTMOTOR = (127 - calculateAdditionLeft(PAN_CENTER_PWM_DEFAULT - PAN_SERVO));
            RIGHTMOTOR = (127 - calculateAdditionLeft(PAN_CENTER_PWM_DEFAULT - PAN_SERVO));
          }
        }
        else if (PAN_SERVO < MIN_PAN)
        {
          LEFTMOTOR = (127 - calculateAdditionLeft(PAN_CENTER_PWM_DEFAULT - PAN_SERVO));
          RIGHTMOTOR = (127 - calculateAdditionLeft(PAN_CENTER_PWM_DEFAULT - PAN_SERVO));
        }
        else
        {
          LEFTMOTOR = (127);
          RIGHTMOTOR = (127);
        }
      }
      //this turns right
      else if (PAN_SERVO > PAN_CENTER_PWM_DEFAULT)
      {
        if (t_pan_error < PAN_ALLOWABLE_ERROR_DEFAULT)
        {
          if ((PAN_SERVO - PAN_CENTER_PWM_DEFAULT) > PAN_CENTER_ERROR)
          {
            LEFTMOTOR = (127 + calculateAdditionRight(PAN_SERVO - PAN_CENTER_PWM_DEFAULT));
            RIGHTMOTOR = (127 + calculateAdditionRight(PAN_SERVO - PAN_CENTER_PWM_DEFAULT));
          }
        }
        else if (PAN_SERVO > PAN_MAX_PWM_DEFAULT - MIN_PAN)
        {
          LEFTMOTOR = (127 + calculateAdditionRight(PAN_SERVO - PAN_CENTER_PWM_DEFAULT));
          RIGHTMOTOR = (127 + calculateAdditionRight(PAN_SERVO - PAN_CENTER_PWM_DEFAULT));
        }
        else
        {
          LEFTMOTOR = (127);
          RIGHTMOTOR = (127);
        }
      }
      else
      {
        LEFTMOTOR = (127);
        RIGHTMOTOR = (127);
      }
    }
    else
    {
      LEFTMOTOR = (127);
      RIGHTMOTOR = (127);
    }
  }
  else
  {
    LEFTMOTOR = (127);
    RIGHTMOTOR = (127);
  }
}
