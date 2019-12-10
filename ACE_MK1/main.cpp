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

void fwd();
void rev();
void left();
void right();
void stop();
void waits(float x) {wait_us(x*1000000);};
void motor_test();

int main()
{
    pc.baud(115200);
    wifi.baud(115200);
	while(1)
	{
        if (pc.readable())
        {
            wifi.putc(pc.getc());
        }
        if (wifi.readable())
        {
            pc.putc(wifi.getc());
        }
        //motor_test();
	}
	
}

void motor_test()
{
    pc.printf("Forward\n\r");
    fwd();
    waits(2.0);
    stop();
    waits(0.5);
    
    pc.printf("Backward\n\r");
    rev();
    waits(2.0);
    stop();
    waits(0.5);
    
    pc.printf("Right\n\r");
    right();
    waits(2.0);
    stop();
    waits(0.5);
    
    pc.printf("Left\n\r");
    left();
    waits(2.0);
    stop();
    waits(0.5);
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
