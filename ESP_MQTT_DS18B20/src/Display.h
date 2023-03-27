#include <TwiLiquidCrystal.h>
#include "Types.h"

TwiLiquidCrystal lcd(0x27);

namespace Display {

    void intro () {
        lcd.begin(16, 2);
        lcd.backlight();

        lcd.print("T1:");
        lcd.setCursor(0, 1);
        lcd.print("T2:");
    }

    void printTemp (sensorState_t &state) {
        lcd.setCursor(4, 0);
        lcd.print(state.temperature11, 1);
        lcd.setCursor(4, 1);
        lcd.print(state.temperature2, 1);
    }

    void error (const char* message) {
        lcd.setCursor(0, 0);
        lcd.print("Error:");
        lcd.setCursor(0, 1);
        lcd.print(message);
    }

}