#ifndef CAMERA_AUTONOMOUS_D
#define CAMERA_AUTONOMOUS_D

#define PAN_CENTER_ERROR 7
#define MIN_PAN 5

int calculateAdditionLeft(int pixelDistance);
int calculateAdditionRight(int pixelDistance);
int ready_to_fire_p(void);
void camera_auto_rotate(void);

#endif
