//fire code by Tom

#ifndef __catlin_fire_h_
#define __catlin_fire_h_

extern char haveFired;
extern int lastFireAngle;
extern int lastFireCamera;

#define OPENED	0		//servo value for oepn
#define CLOSE	255   //servo value for closed
#define TIMEBALLOUT 22 //time it takes the ball to drop out before we can put servo back to close
#define RAPIDFIREWAIT 30 //how many times the fireTest has to be called while trigger held down to rapid fire

void fireTest(void);//lets code test if it should fire
void fire(void);//lets just fire!!!!!!!
void fireInit(void);
void rapidFire(void);

#endif 
