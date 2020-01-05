#include "motor.h"

void transfer(string code, bool thread_stop = false) {
    if (code == "FWD") {fwd();}
    else if (code == "REV") {rev();}
    else if (code == "LFT") {left();}
    else if (code == "RGT") {right();}
    else if (code == "STP") {stop(thread_stop);}
}

void motor_loop(string* pattern) {
    stop_lock.lock();if (stop_thread) {stop_lock.unlock();return;}stop_lock.unlock();
    //pc.printf("Starting thread");
    char num = (*pattern).at(0) - '0';
    string data = (*pattern).substr(2);
    char counter = 0, num_counter = 0;
    //pc.printf("Num: %d\n", num);
    Timer timer;
    while(true) {
        for (int i = 0; i < num; i++) {
            stop(true);
            string code = data.substr(counter, 3);
            num_counter = counter+3;
            counter += data.substr(counter).find('_')+1;
            float timeout = 1000 * atof(data.substr(num_counter, counter-1-num_counter).c_str());
            //pc.printf("%s %f\n", code.c_str(), timeout);
            transfer(code, true);
            timer.start();
            float time = timer.read_ms();
            while(time + timeout > timer.read_ms()){
                stop_lock.lock();
                if (stop_thread == true) {
                    stop_lock.unlock();
                    //pc.printf("Stopping thread\n");
                    return;
                }
                stop_lock.unlock();
            }
            t.stop();
        }
        counter = 0;
    }
    
}


void waits(float x) {ThisThread::sleep_for(x*1000);}

void fwd()
{
    motor_lock.lock();
    A_FWD = 1;
    B_REV = 1;
    C = 1;
    motor_lock.unlock();
}

void rev()
{
    motor_lock.lock();
    A_REV = 1;
    B_FWD = 1;
    D = 1;
    motor_lock.unlock();
}

void left()
{
    motor_lock.lock();
    A_REV = 1;
    B_REV = 1;
    motor_lock.unlock();
}

void right()
{
    motor_lock.lock();
    A_FWD = 1;
    B_FWD = 1;
    C = 1;
    motor_lock.unlock();
}

void stop(bool stop_from_thread)
{
    string x = stop_from_thread ? "Thread":"Normal";
    if (!stop_from_thread && !stop_thread) {
        stop_lock.lock();
        stop_thread = true;
        stop_lock.unlock();
        //pc.printf("Stopping from function stop\n");
        motor_thread.join();
        //pc.printf("Joined\n");
        x = "Terminate";
    }
    motor_lock.lock();
    A_FWD = 0;
    B_REV = 0;
    A_REV = 0;
    B_FWD = 0;
    C = 0;
    D = 0;
    motor_lock.unlock();
    //pc.printf("%s Stop\n", x.c_str());
    ThisThread::sleep_for(500);
}
