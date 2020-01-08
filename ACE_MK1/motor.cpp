#include "motor.h"

void transfer(string code, bool thread_stop) {
    if (code == "FWD") {fwd();}
    else if (code == "REV") {rev();}
    else if (code == "LFT") {left();}
    else if (code == "RGT") {right();}
    else if (code == "STP") {stop();}
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

void stop()
{
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
