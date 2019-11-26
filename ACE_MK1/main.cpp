#include "mbed.h"
#include <string>
#include "TextLCD.h"
#include "motordriver.h"

TextLCD lcd(D2, D3, D4, D5, D6, D7); // rs, e, d4-d7
Motor A(D10, D8, D9, 1);
Motor B(D12, D14, D15, 1);


int main()
{
	
	while(1)
	{
        lcd.cls();
		lcd.printf("Forward");
        A.speed(0.8);
        B.speed(0.8);
        wait_us(2000000);

        A.stop(0.5);
        B.stop(0.5);
        wait_us(50000);
        
        lcd.cls();
		lcd.printf("Backward");
        A.speed(-0.8);
        B.speed(-0.8);
        wait_us(2000000);

        A.stop(0.5);
        B.stop(0.5);
        wait_us(50000);
        
        lcd.cls();
		lcd.printf("Right");
        A.speed(0.8);
        B.speed(-0.8);
        wait_us(2000000);

        A.stop(0.5);
        B.stop(0.5);
        wait_us(50000);
        
        lcd.cls();
		lcd.printf("Left");
        A.speed(-0.8);
        B.speed(0.8);
        wait_us(2000000);

        A.stop(0.5);
        B.stop(0.5);
        wait_us(50000);
        
	}
	
}
