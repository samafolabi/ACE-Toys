#include "bluetooth.h"

string read_bt() {
    string res = "";
    bt_timer.start();
    float time = bt_timer.read_ms();
    while(time + 500 > bt_timer.read_ms()){
        while (bt.readable()){
            res += bt.getc();
        }
    }
    bt_timer.stop();
    res = trim(res);
    return res;
}
