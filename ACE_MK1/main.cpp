#include "mbed.h"
#include "setup.h"
#include "motor.h"
#include "wifi.h"
#include "bluetooth.h"

string data_decipher(string data);
string read_bt();
//AnalogIn l_sensor(A0), r_sensor(A1);
string last_movement = "";
bool wifi_error = false, needs_connecting = true;
string check_wifi();
string list_wifi();
string connect_wifi(string wifi_data);
string patt;
void loopy();
void leddy();
volatile bool mthread_running = false, lthread_running = false;
Thread m_thread, l_thread;
Mutex m_lock;
string motor_pattern = "", led_pattern = "";

#define MOTOR 1
#define LED 2

int main()
{
    wifi.baud(115200);
    pc.baud(115200);
    bt.baud(9600);

    //pc.puts("Initialising");
    stop();

    m_thread.start(loopy);
    l_thread.start(leddy);

	while(1)
	{
        if(bt.readable()) {
            string data = read_bt();
            //pc.printf("%s\n", data.c_str());
            string res = data_decipher(data);
            bt.printf("%s\n",res.c_str());
        }
        if (!wifi_error && !needs_connecting && wifi.readable()) {
            string res = read_wifi();
            
            size_t check = res.find("POST / HTTP/1.1");
            if (check == string::npos)
                {
                    //pc.printf("%s\n---\n", res.c_str());
                }
            if (check != string::npos) {
                size_t d = res.find(ace_break) + ace_break_length + 1; //new line as well
                string data = trim(res.substr(d));
                //pc.printf("%s\n", data.c_str());

                string response = "HTTP/1.1 200 OK\nAccess-Control-Allow-Origin : *\nContent-Type: text/plain\nContent-Length: ";

                string res_data = data_decipher(data);
                //pc.printf("%s\n\n-----\n\n", data.c_str());
                //pc.printf("%s\n%d", res.c_str(),d);

                response += to_string(res_data.length());
                response += "\n\n";
                response += res_data;

                pc.printf("Response: %s\n", res_data.c_str());

                wifi_send("AT+CIPSEND=0,"+to_string(response.length()));
                wifi.printf("%s",response.c_str());
            }
        }
	}
	
}

string list_wifi() {
    green.attach(&tick, 0.2);
    short size = 20;
    string ssids[size];
    char ecns[size];
    string json = "JSON:{\"wifis\": [";
    string wifis = wifi_send("AT+CWLAP", true);
    string line = ""; char count = 0;
    while ((wifis.find("(") != string::npos) && count < size) {
        wifis = wifis.substr(wifis.find("("));
        if (wifis.at(1)-0x30 == 5) continue;
        if (count > 0) json += ",";
        json += "\"";
        json += wifis.at(1);
        wifis = wifis.substr(wifis.find("\"")+1);
        json += wifis.substr(0,wifis.find("\""));
        json += "\"";
        count++;
    }
    json += "]}";
    green.detach();
    init_led = 1;
    return json;
}

string check_wifi() {
    green.attach(&tick, 0.2);
    int wifi_active = wifi_setup();
    green.detach();
    init_led = 0;
    string return_data = "";
    switch(wifi_active){
        case 0:
            ok_led = 1;
            //pc.printf("Wi-Fi is ready\n");
            needs_connecting = false;
            return_data = "IP_GIVEN_";
            return_data += ip_addr;
            break;
        case 1:
            init_led = 1;
            //pc.printf("Can't connect to wi-fi\n");
            return_data = "ConnectionNeeded_Check";
            break;
        case 2:
        case -1:
        default:
            err_led = 1;
            //pc.printf("ERRORRRRR!\n");
            wifi_error = true;
            return_data = "Error_Check";
            break;
    }
    return return_data;
}

string connect_wifi(string wifi_data) {
    green.attach(&tick, 0.2);
    string cmd = "AT+CWJAP=\"";//PN8V6\",\"";
    cmd += wifi_data.substr(1, wifi_data.find(";")-1);
    cmd += "\",\"";
    cmd += wifi_data.at(0)-0x30 > 0 ? wifi_data.substr(wifi_data.find(";")+1) : "";
    cmd += "\"";
    //pc.printf("%s\n", cmd.c_str());
    bool all_ok = wifi_send_test(cmd, "OK", true);
    green.detach();
    if (!all_ok) {
        init_led = 1;
        return "Error_Connecting";
    } else {
        string addr = wifi_send("AT+CIFSR");
        addr = addr.substr(addr.find("\"")+1);
        ip_addr = addr.substr(0,addr.find("\""));
        wifi_send("AT+CIPMUX=1");
        bool all_ok2 = wifi_send_test("AT+CIPSERVER=1,80","OK");
        if (!all_ok2){//pc.printf("SERVER Error");
            err_led = 1;
            return "Error_Connecting";
        }
        ok_led = 1;
        //pc.printf("Wi-Fi is ready\n");
        needs_connecting = false;
        string retu = "IP_GIVEN_";
        retu += ip_addr;
        return retu;
    }
}

string data_decipher(string data) {
    string res_data = "";
    if (data == "Test") {
        res_data = "Connected to Wi-Fi";
    } else if (data == "Connect?") {
        res_data = "Connected to BT";
    } else if (data == "CHECK_WIFI") {
        res_data = check_wifi();
    } else if (data == "CONNECT_LIST") {
        res_data = list_wifi();
    } else if (data.substr(0,13) == "WIFI_CONNECT_") {
        res_data = connect_wifi(data.substr(13));
    } else if (data == "FWD") {
        if (last_movement != data) {
            stop();
            fwd();
            last_movement = data;
        }
        res_data = "Moving Forward";
    } else if (data == "REV") {
        if (last_movement != data) {
            stop();
            rev();
            last_movement = data;
        }
        res_data = "Moving Backward";
    } else if (data == "LFT") {
        if (last_movement != data) {
            stop();
            left();
            last_movement = data;
        }
        res_data = "Moving Left";
    } else if (data == "RGT") {
        if (last_movement != data) {
            stop();
            right();
            last_movement = data;
        }
        res_data = "Moving Right";
    } else if (data == "STP") {
        if (mthread_running) {
            mthread_running = false;
        }
        stop();
        res_data = "Stopping";
        last_movement = "";
    } else if (data.substr(0,5) == "LOOP_") {
        if (mthread_running) {
            mthread_running = false;
        }
        stop();
        motor_pattern = data.substr(5);
        m_thread.flags_set(MOTOR);
        res_data = "Performing Motor Loop";
    } else if (data.substr(0,4) == "LED_") {
        if (lthread_running) {
            lthread_running = false;
        }
        led_pattern = data.substr(4);
        l_thread.flags_set(LED);
        res_data = "Performing LED Loop";
    } else {
        stop();
        res_data = "Error";
        ok_led = 0;
        err_led = 1;
    }
    //pc.printf("%s\n", res_data.c_str());
    return res_data;
}

void loopy () {while(1){
    ThisThread::flags_wait_all(MOTOR);
    mthread_running = true;
    string pattern = motor_pattern;
    char num = motor_pattern.at(0) - 0x30;
    //pc.printf("%d\n", num);
    string cde[num];
    char sec[num];
    pattern = pattern.substr(2);
    //pc.printf("%s\n", pattern.c_str());
    for (int i = 0; i < num; i++) {
        cde[i] = pattern.substr(0,3);
        sec[i] = pattern.at(3) - 0x30;
        if(i < num-1) pattern = pattern.substr(5);
        //pc.printf("%s %d %s\n", pattern.c_str(), sec[i], cde[i].c_str());
    }
    while(1) {
        for (int i = 0; i < num; i++) {
            stop();
            transfer(cde[i]);
            if (!mthread_running) break;
            ThisThread::sleep_for(sec[i]*1000);
        }
        if (!mthread_running) break;
    }}
}

void leddy () {while(1){
    ThisThread::flags_wait_all(LED);
    lthread_running = true;
    string pattern = led_pattern;
    char num = 3;
    char led_out[num];
    char sec[num];
    //pc.printf("%s\n", pattern.c_str());
    for (int i = 0; i < num; i++) {
        led_out[i] = ((pattern.at(0)-0x30)<<2) + ((pattern.at(1)-0x30)<<1) + ((pattern.at(2)-0x30)<<0);
        sec[i] = pattern.at(3) - 0x30;
        if(i < num-1) pattern = pattern.substr(5);
        //pc.printf("%s %d %x\n", pattern.c_str(), sec[i], led_out[i]);
    }
    while(1) {
        for (int i = 0; i < num; i++) {
            leds = led_out[i];
            if (!lthread_running) break;
            ThisThread::sleep_for(sec[i]*1000);
        }
        if (!lthread_running) break;
    }}
}
