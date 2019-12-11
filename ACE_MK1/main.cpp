#include "mbed.h"
#include <string>
#include "TextLCD.h"
#include "motordriver.h"

TextLCD lcd(D2, D3, D4, D5, D6, D7); // rs, e, d4-d7
//enable, fwd, rev
Motor A(D10, D8, D9, 1); //left
Motor B(D12, D14, D15, 1); //right
Serial pc(USBTX, USBRX);
Serial wifi(PE_8,PE_7);
Timer t;

string ip_addr = "";

void fwd();
void rev();
void left();
void right();
void stop();
void waits(float x) {wait_us(x*1000000);};

string wifi_send(string cmd, bool lon = false);
bool wifi_send_test(string cmd, string test, bool lon = false);
bool wifi_setup();

int main()
{
    wifi.baud(115200);
    pc.baud(115200);

    lcd.cls();
    lcd.printf("Initialising");
    stop();
    if (!wifi_setup()){pc.printf("Error");};

	while(1)
	{
        sleep();
	}
	
}

string wifi_send(string cmd, bool lon) {
    /*wifi.putc('A');
    wifi.putc('T');
    wifi.putc('\r');
    wifi.putc('\n');
    while(1){
        if (wifi.readable()) {
            pc.putc(wifi.getc());
        }
    }*/
    string res = "";
    for (int i = 0; i < cmd.length(); i++) {wifi.putc(cmd.at(i));}
    wifi.putc('\r');
    wifi.putc('\n');
    //bool running = true;
    float timeout = lon ? 15000 : 200;
    t.start();
    float time = t.read_ms();
    while(time + timeout > t.read_ms()){
        while (wifi.readable()) {
            res += wifi.getc();
        }
    }
    pc.printf("%s\n",res.c_str());
    return res;
}

bool wifi_send_test(string cmd, string test, bool lon) {
    return wifi_send(cmd, lon).find(test) != string::npos;
}

bool wifi_setup() {
    if (wifi_send_test("AT", "OK")) {
        lcd.cls();
        lcd.printf("Wi-Fi Working");
				Thread::wait(2000);
        wifi_send("AT+CWMODE=1");
        string addr = wifi_send("AT+CIFSR");
        if (addr.find("0.0.0.0") != string::npos) {
            lcd.cls();
            lcd.printf("Connecting...");
            bool all_ok = wifi_send_test("AT+CWJAP=\"Test-ACE\",\"test-pass\"", "OK", true);
            if (!all_ok){lcd.cls();lcd.printf("CNCTION Error");return false;}
            addr = wifi_send("AT+CIFSR");
        }
        addr = addr.substr(addr.find_first_of("\"")+1);
        ip_addr = addr.substr(0,addr.find_first_of("\""));
        lcd.cls();
        lcd.printf("Connected");
        Thread::wait(2000);
        wifi_send("AT+CIPMUX=1");
        bool all_ok = wifi_send_test("AT+CIPSERVER=1,80","OK");
        if (!all_ok){lcd.cls();lcd.printf("SERVER Error");return false;}
        lcd.cls();
        lcd.printf("%s", ip_addr.c_str());
        lcd.locate(0, 1);
        lcd.printf("Ready to CNCT");

    } else {
        lcd.cls();
        lcd.printf("MODULE Error");
        return false;
    }
    return true;
}

void fwd()
{
    A.speed(0.8);
    B.speed(-0.8);
}

void rev()
{
    A.speed(-0.8);
    B.speed(0.8);
}

void left()
{
    A.speed(-0.8);
    B.speed(-0.8);
}

void right()
{
    A.speed(0.8);
    B.speed(0.8);
}

void stop()
{
    A.stop(0.5);
    B.stop(0.5);
    waits(0.5);
}
