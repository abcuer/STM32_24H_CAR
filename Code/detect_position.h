#ifndef __DETECT_POSITION_H
#define __DETECT_POSITION_H

float getAverageYaw(int num_samples, int delay_ms);
void initialize(void);
void Yaw_detect(void);
void SoundLight(void);
void UpdateSoundLight(void);
void normalize_angle(float *angle);
void Trace(void);

#endif
