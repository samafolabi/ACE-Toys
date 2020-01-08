#include "setup.h"

DigitalOut A_FWD(D9), A_REV(D10), B_FWD(D14), B_REV(D15);
Serial pc(USBTX, USBRX);
Serial wifi(PE_8,PE_7);
Serial bt(PC_6, PC_7);
Timer wifi_timer, bt_timer;
DigitalOut err_led(LED3), init_led(LED2), ok_led(LED1);
BusOut leds(D5,D6,D7);
DigitalOut C(D12), D(D13); //left2, right2
Mutex motor_lock;
Ticker green;

void tick() {
    init_led = !init_led;
}

string ltrim(string s)
{
	size_t start = s.find_first_not_of(WHITESPACE);
	return (start == std::string::npos) ? "" : s.substr(start);
}

string rtrim(string s)
{
	size_t end = s.find_last_not_of(WHITESPACE);
	return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

string trim(string s)
{
	return rtrim(ltrim(s));
}


