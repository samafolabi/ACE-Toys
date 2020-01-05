#include "mbed.h"
#include "setup.h"
#include "motor.h"
#include "wifi.h"

string data_decipher(string data);
AnalogIn l_sensor(A0), r_sensor(A1);

int main()
{
    wifi.baud(115200);
    pc.baud(115200);

    pc.puts("Initialising");
    stop();

    green.attach(&tick, 0.2);
    
    bool active = true;
    if (!wifi_setup()) {
        pc.printf("Error");
        active = false;
    }
    green.detach();

	while(1)
	{
        /*if (active)
        {
            if (wifi.readable()) {
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
                    pc.printf("%s\n\n-----\n\n", data.c_str());
                    pc.printf("%s\n%d", res.c_str(),d);

                    response += to_string(res_data.length());
                    response += "\n\n";
                    response += res_data;

                    wifi_send("AT+CIPSEND=0,"+to_string(response.length()));
                    wifi.printf("%s",response.c_str());
                }
            }
        }else {
            sleep();
        }*/
        pc.printf("L: %5.2f R: %5.2f\n", l_sensor.read(), r_sensor.read());
        ThisThread::sleep_for(2500);
	}
	
}

string data_decipher(string data) {
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
    } else if (data.substr(0,5) == "LOOP_") {
        stop();
        string pattern = data.substr(5);
        stop_thread = false;
        motor_thread.start(callback(motor_loop, &pattern));
        res_data = data_set("Performing Loop", data, "Performing Loop");
    } else {
        stop();
        res_data = data_set("DATA Error", "Error");
        ok_led = 0;
        err_led = 1;
    }
    return res_data;
}




