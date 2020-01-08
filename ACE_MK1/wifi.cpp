#include "wifi.h"

string ip_addr;

string wifi_send(string cmd, bool lon) {
    cmd = trim(cmd);
    string res = "";
    for (int i = 0; i < cmd.length(); i++) {wifi.putc(cmd.at(i));}
    wifi.putc('\r');
    wifi.putc('\n');
    float timeout = lon ? 15000 : 750;
    wifi_timer.start();
    float time = wifi_timer.read_ms();
    while(time + timeout > wifi_timer.read_ms()){
        if (wifi.readable()) {
            res+=wifi.getc();
        }
    }
    wifi_timer.stop();
    //pc.printf("%s\n",res.c_str());
    return res;
}

bool wifi_send_test(string cmd, string test, bool lon) {
    return wifi_send(cmd, lon).find(test) != string::npos;
}

int wifi_setup() {
    ThisThread::sleep_for(1000);
    if (wifi_send_test("AT", "OK")) {
        //lcd.cls();
        //pc.printf("Wi-Fi Working");
		ThisThread::sleep_for(2000);
        wifi_send("AT+CWMODE=1");

        //lcd.cls();
        //pc.printf("Connecting...");
        string res = ""; wifi_timer.start();
        float time = wifi_timer.read_ms();
        while(time + 3000 > wifi_timer.read_ms()){
            while (wifi.readable()) {
                res += wifi.getc();
            }
        }
        wifi_timer.stop();
        //pc.printf("%s", res.c_str());
        string addr = "";
        if (res.find("+CIFSR:") != string::npos) {
            addr = res;
        } else {
            addr = wifi_send("AT+CIFSR");
            if (addr.find("0.0.0.0") != string::npos) {
                //bool all_ok = wifi_send_test("AT+CWJAP=\"PN8V6\",\"HJTSF3JYJ945YLRC\"", "OK", true);
                //if (!all_ok){/*lcd.cls();*/
                //pc.printf("CNCTION Error");
                return 1;
                //}
                //addr = wifi_send("AT+CIFSR");
            }
        }
        
        addr = addr.substr(addr.find("\"")+1);
        ip_addr = addr.substr(0,addr.find("\""));
        //lcd.cls();
        //pc.printf("Connected");
        ThisThread::sleep_for(2000);
        wifi_send("AT+CIPMUX=1");
        bool all_ok2 = wifi_send_test("AT+CIPSERVER=1,80","OK");
        if (!all_ok2){/*lcd.cls();*///pc.printf("SERVER Error");
        
        return 2;
        }
        //lcd.cls();
        //pc.printf("%s", ip_addr.c_str());
        //lcd.locate(0, 1);
        //pc.printf("Ready to CNCT");
    } else {
        //lcd.cls();
        //pc.printf("MODULE Error");
        return -1;
    }
    return 0;
}

string read_wifi() {
    string res = "";
    wifi_timer.start();
    float time = wifi_timer.read_ms();
    while(time + 500 > wifi_timer.read_ms()){
        while (wifi.readable()){
        res += wifi.getc();}
    }
    wifi_timer.stop();
    return res;
}
