#ifndef MOTOR_H
#define MOTOR_H

#include "mbed.h"
#include "setup.h"

extern void fwd();
extern void rev();
extern void left();
extern void right();
extern void stop(bool stop_from_thread = false);
extern void waits(float x);
extern void motor_loop(string* pattern);

#endif