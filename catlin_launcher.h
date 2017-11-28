#ifndef __catlin_launcher_h_
#define __catlin_launcher_h_

#define LEARNCHANGE 6
#define SLOWSPEED 163

void updateLauncher(void);
void setLauncher(int);
void launcherInit(void);
void learnCorrect(void);
void learnHigh(void);
void learnLow(void);
void autoAim(void);
void CatlinEEPROM_WRITE(int, int);
int CatlinEEPROM_READ(int);
void CatlinEEPROM_CLEAR();
double abs(double);
void zeroLauncher(void);
char launcher_ready_p(void);
void CatlinEEPROM_PRINT(void);

extern long int goalLauncherAngle;

#endif
