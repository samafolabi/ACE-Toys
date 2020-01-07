#include "mbed.h"
#include "setup.h"
#include "motor.h"
#include "wifi.h"
#include "bluetooth.h"

string data_decipher(string data);
string read_bt();
//AnalogIn l_sensor(A0), r_sensor(A1);
string last_movement = "";

int main()
{
    wifi.baud(115200);
    pc.baud(115200);
    bt.baud(9600);

    pc.puts("Initialising");
    stop();

    green.attach(&tick, 0.2);
    
    bool wifi_active = wifi_setup();
    green.detach();
    if (!wifi_active) {
        init_led = 1;
        pc.printf("Wi-Fi not setup");
        wifi_active = false;
    }

	while(1)
	{
        if(bt.readable()) {
            string data = read_bt();
            pc.printf("%s\n", data.c_str());
            string res = data_decipher(data);
            bt.printf("%s\n",res.c_str());
        }
        if (wifi_active && wifi.readable()) {
            string res = read_wifi();
            
            size_t check = res.find("POST / HTTP/1.1");
            if (check == string::npos)
                {
                    pc.printf("%s\n---\n", res.c_str());
                }
            if (check != string::npos) {
                size_t d = res.find(ace_break) + ace_break_length + 1; //new line as well
                string data = trim(res.substr(d));
                pc.printf("%s\n", data.c_str());

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

string data_decipher(string data) {
    string res_data = "";
    if (data == "Test") {
        res_data = "Connected to Wi-Fi";
    } else if (data == "Connect?") {
        res_data = "Connected to BT";
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
        stop();
        res_data = "Stopping";
        last_movement = "";
    } else if (data.substr(0,5) == "LOOP_") {
        stop();
        string pattern = data.substr(5);
        stop_thread = false;
        motor_thread.start(callback(motor_loop, &pattern));
        res_data = "Performing Loop";
    } else {
        stop();
        res_data = "Error";
        ok_led = 0;
        err_led = 1;
    }
    pc.printf("%s\n", res_data.c_str());
    return res_data;
}




