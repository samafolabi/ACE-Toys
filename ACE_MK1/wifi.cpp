#include "wifi.h"

string ip_addr;

string wifi_send(string cmd, bool lon) {
    string res = "";
    for (int i = 0; i < cmd.length(); i++) {wifi.putc(cmd.at(i));}
    wifi.putc('\r');
    wifi.putc('\n');
    float timeout = lon ? 15000 : 500;
    t.start();
    float time = t.read_ms();
    while(time + timeout > t.read_ms()){
        if (wifi.readable()) {
            res+=wifi.getc();
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
    ThisThread::sleep_for(1000);
    if (wifi_send_test("AT", "OK")) {
        //lcd.cls();
        pc.printf("Wi-Fi Working");
		ThisThread::sleep_for(2000);
        wifi_send("AT+CWMODE=1");

        //lcd.cls();
        pc.printf("Connecting...");
        string res = ""; t.start();
        float time = t.read_ms();
        while(time + 3000 > t.read_ms()){
            while (wifi.readable()) {
                res += wifi.getc();
            }
        }
        t.stop();
        //pc.printf("%s", res.c_str());
        string addr = "";
        if (res.find("+CIFSR:") != string::npos) {
            addr = res;
        } else {
            addr = wifi_send("AT+CIFSR");
            if (addr.find("0.0.0.0") != string::npos) {
                bool all_ok = wifi_send_test("AT+CWJAP=\"PN8V6\",\"HJTSF3JYJ945YLRC\"", "OK", true);
                if (!all_ok){/*lcd.cls();*/
                pc.printf("CNCTION Error");
        ok_led = 0;
        err_led = 1;return false;
                }
                addr = wifi_send("AT+CIFSR");
            }
            //connection switch code
            /*if (addr.find("0.0.0.0") != string::npos) {
                string ssids[50];
                string ecns[50];
                string wifis = wifi_send("AT+CWLAP", true);
                string line = ""; char count = 0;
                while (wifis.find("(") != string::npos) {
                    wifis = wifis.substr(wifis.find("("));
                    ecns[count] = wifis.at(1);
                    wifis = wifis.substr(wifis.find("\"")+1);
                    ssids[count] = wifis.substr(0,wifis.find("\""));
                    count++;
                }
                //pc.printf("%s",ecns[0].c_str());
                //lcd.cls();
                lcd.locate(0, 1);
                pc.printf("Connect");
                lcd.locate(0, 0);
                char current = 0;
                while (ecns[current].at(0)-0x30==5) {current++;}
                
                        
                        l_print(ssids[current]);
                
                while (1) {
                    if (first) {
                        //pc.printf("reahced");
                        current++;
                        while (ecns[current].at(0)-0x30==5) {current = current == count? 0:current+1;};
                                //lcd.cls();
                                lcd.locate(0, 0);
                                l_print(ssids[current]);
                                lcd.locate(0, 1);
                                l_print("Connect");
                              
                        ThisThread::sleep_for(300);
                        while(!first);
                    }
                    if (second) {
                        if (ecns[current].at(0)-0x30==0) {
                            //lcd.cls(); pc.printf("Connecting...");
                            bool all_ok = wifi_send_test("AT+CWJAP=\""+ssids[current]+"\"", "OK", true);
                            if (!all_ok){//lcd.cls();pc.printf("CNCTION Error");return false;}
                            addr = wifi_send("AT+CIFSR");
                            break;
                        } else {
                            //lcd.cls();
                            pc.printf("Need BT for password");
                            ThisThread::sleep_for(1000);
                            //lcd.cls();
                            lcd.locate(0, 1);
                            pc.printf("Connect");
                            lcd.locate(0, 0);
                            pc.printf("%s", ssids[current].c_str());
                        }
                    }
                }
                
            }*/
        }
        
        addr = addr.substr(addr.find("\"")+1);
        ip_addr = addr.substr(0,addr.find("\""));
        //lcd.cls();
        pc.printf("Connected");
        ThisThread::sleep_for(2000);
        wifi_send("AT+CIPMUX=1");
        bool all_ok2 = wifi_send_test("AT+CIPSERVER=1,80","OK");
        if (!all_ok2){/*lcd.cls();*/pc.printf("SERVER Error");
        
        ok_led = 0;
        err_led = 1;
        return false;
        }
        //lcd.cls();
        pc.printf("%s", ip_addr.c_str());
        //lcd.locate(0, 1);
        pc.printf("Ready to CNCT");
        ok_led = 1;
        err_led = 0;
    } else {
        //lcd.cls();
        ok_led = 0;
        err_led = 1;
        pc.printf("MODULE Error");
        return false;
    }
    return true;
}

string read_wifi() {
    string res = "";
    t.start();
    float time = t.read_ms();
    while(time + 500 > t.read_ms()){
        while (wifi.readable()){
        res += wifi.getc();}
    }
    t.stop();
    return res;
}
