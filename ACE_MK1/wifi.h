#ifndef WIFI_H
#define WIFI_H

#include "mbed.h"
#include "setup.h"

#define ace_break "ACE-Break__"
#define ace_break_length 11

extern string ip_addr;

extern string wifi_send(string cmd, bool lon = false);
extern bool wifi_send_test(string cmd, string test, bool lon = false);
extern int wifi_setup();
extern string read_wifi();


#endif