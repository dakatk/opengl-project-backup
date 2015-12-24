#ifndef VIEW_H__
#define VIEW_H__

float angle;
float vx, vz, vy;
float x, y, z;
float fdx, fdy, fdz, sdx, sdz;

extern void initCamera();
extern void direction(float delta_h, float delta_v);
extern void position(float delta_x, float delta_z, float s);

#endif
