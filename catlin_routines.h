/*******************************************************************************
* FILE NAME: catlin_routines.h
*
* DESCRIPTION: 
*  This file contains declarations of functions used in Catlin's portion of
*  software.
*
*******************************************************************************/

#ifndef __catlin_routines_h_
#define __catlin_routines_h_


/* Function Declarations */
void Catlin_Initialize(void);
void Catlin_Autonomous(void);
void Catlin_Main_Routine(void);
void Catlin_Common_First(void);
void Catlin_Common_Last(void);
void Catlin_Basic_Drive(void);
void Catlin_Manual_Elevation(void);
void Catlin_Slight_Shift(void);
void Set_Catlin_Slight_Shift(char);
void arcDrive(void);

#endif
