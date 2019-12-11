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
const string ace_break = "ACE-Break__";
const string WHITESPACE = " \n\r\t\f\v";

void fwd();
void rev();
void left();
void right();
void stop();
void waits(float x) {wait_us(x*1000000);};

string trim(string s);
string wifi_send(string cmd, bool lon = false);
bool wifi_send_test(string cmd, string test, bool lon = false);
bool wifi_setup();
string data_set(string lcd_data, string pc_data, string res_data = "");

int main()
{
    wifi.baud(115200);
    pc.baud(115200);

    lcd.cls();
    lcd.printf("Initialising");
    stop();
    
    bool active = true;
    if (!wifi_setup()){pc.printf("Error"); active = false;};

	while(1)
	{
        if (active)
        {
            if (wifi.readable()) {
                string res = "";
                t.start();
                float time = t.read_ms();
                while(time + 500 > t.read_ms()){
                    while (wifi.readable()) {
                        res += wifi.getc();
                    }
                }
                t.stop();
                size_t check = res.find("POST / HTTP/1.1");
                if (check == string::npos)
                    {pc.printf("%s\n---\n", res.c_str());}
                if (check != string::npos) {
                    size_t d = res.find(ace_break) + ace_break.length() + 1; //new line as well
                    string data = trim(res.substr(d));
                    pc.printf("%s\n", data.c_str());

                    string response = "HTTP/1.1 200 OK\nAccess-Control-Allow-Origin : *\nContent-Type: text/plain\nContent-Length: ";

                    string res_data = "";
                    if (data == "Test") {
                        res_data = data_set("Connected to\nHost", "Connected to Host", "Connected to Buggy");
                    } else if (data == "FWD") {
                        stop();
                        fwd();
                        res_data = data_set("Moving\nForward", "Moving Forward");
                    } else if (data == "REV") {
                        stop();
                        rev();
                        res_data = data_set("Moving\nBackward", "Moving Backward");
                    } else if (data == "LFT") {
                        stop();
                        left();
                        res_data = data_set("Moving\nLeft", "Moving Left");
                    } else if (data == "RGT") {
                        stop();
                        right();
                        res_data = data_set("Moving\nRight", "Moving Right");
                    } else if (data == "STP") {
                        stop();
                        res_data = data_set("Stopping", "Stopping");
                    } else if (data.substr(0,4) == "LCD_") {
                        res_data = data_set(data.substr(4), "Writing to LCD", "Written to LCD");
                    } else {
                        res_data = data_set("DATA Error", "Error");
                    }

                    response += to_string(res_data.length());
                    response += "\n\n";
                    response += res_data;

                    wifi_send("AT+CIPSEND=0,"+to_string(response.length()));
                    wifi.printf("%s",response.c_str());
                }
            }
        }else {sleep();}
	}
	
}

string data_set(string lcd_data, string pc_data, string res_data) {
    res_data = res_data == "" ? pc_data : res_data;
    pc.printf("%s\n", pc_data.c_str());
    lcd.cls();
    lcd.printf("%s", lcd_data.c_str());
    return res_data;
}

string trim(string s)
{
	size_t end = s.find_last_not_of(WHITESPACE);
	return (end == string::npos) ? "" : s.substr(0, end + 1);
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
    t.stop();
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
    Thread::wait(500);
}
