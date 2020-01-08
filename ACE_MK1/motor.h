#ifndef MOTOR_H
#define MOTOR_H

#include "mbed.h"
#include "setup.h"

extern void transfer(string code, bool thread_stop = false);
extern void fwd();
extern void rev();
extern void left();
extern void right();
extern void stop();
extern void waits(float x);
extern void motor_loop(string* pattern);

#endif