#include "setup.h"

DigitalOut A_FWD(D9), A_REV(D10), B_FWD(D14), B_REV(D15);
Serial pc(USBTX, USBRX);
Serial wifi(PE_8,PE_7);
Timer t;
DigitalOut err_led(LED3), init_led(LED2), ok_led(LED1);
DigitalOut C(D12), D(D13); //left2, right2
Mutex motor_lock, stop_lock, pc_lock;
volatile bool stop_thread = true;
Thread motor_thread;
Ticker green;


void l_print(string s) {
    //if (s.length() >= 16) {s = s.substr(0,15) + "\0";}
    /*lcd*/pc.printf("%s", s.c_str());
}

void tick() {
    init_led = !init_led;
}

string data_set(string lcd_data, string pc_data, string res_data) {
    res_data = res_data == "" ? pc_data : res_data;
    //pc.printf("%s\n", pc_data.c_str());
    //lcd.cls();
    l_print(lcd_data);
    return res_data;
}

string trim(string s)
{
	size_t end = s.find_last_not_of(WHITESPACE);
	return (end == string::npos) ? "" : s.substr(0, end + 1);
}


