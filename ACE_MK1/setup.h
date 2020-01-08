#ifndef SETUP_H
#define SETUP_H

#include "mbed.h"
#include <string>
#include "motordriver.h"

#define WHITESPACE " \n\r\t\f\v"

extern DigitalOut A_FWD;
extern DigitalOut A_REV;
extern DigitalOut B_FWD;
extern DigitalOut B_REV; //left, right, rear
extern Serial pc;
extern Serial wifi;
extern Serial bt;
extern Timer wifi_timer;
extern Timer bt_timer;
extern DigitalOut err_led;
extern DigitalOut init_led;
extern DigitalOut ok_led; //red, blue, yellow
extern DigitalOut C;
extern DigitalOut D; //left2, right2
extern BusOut leds;
extern Mutex motor_lock;
extern Mutex stop_lock;
extern Mutex pc_lock;
extern volatile bool stop_thread;
extern Thread motor_thread;
extern Ticker green;

extern string ltrim(string s);
extern string rtrim(string s);
extern string trim(string s);
extern void tick();

#endif