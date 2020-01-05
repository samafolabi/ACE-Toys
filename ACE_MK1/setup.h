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
extern Timer t;
extern DigitalOut err_led;
extern DigitalOut init_led;
extern DigitalOut ok_led; //red, blue, yellow
extern DigitalOut C;
extern DigitalOut D; //left2, right2
extern Mutex motor_lock;
extern Mutex stop_lock;
extern Mutex pc_lock;
extern volatile bool stop_thread;
extern Thread motor_thread;
extern Ticker green;

extern void l_print(string s);
extern string trim(string s);
extern string data_set(string lcd_data, string pc_data, string res_data = "");
extern void tick();

#endif